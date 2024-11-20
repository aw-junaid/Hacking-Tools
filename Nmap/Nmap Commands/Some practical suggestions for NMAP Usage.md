# Some practical suggestions for NMAP Usage.md

Probably at least 90% of network scanning is non-controversial. You are rarely badgered for scanning
your own machine or the networks you administer. The controversy comes when scanning other networks.
There are many reasons (good and bad) for doing this sort of network exploration. Perhaps you are scanning
the other systems in your dorm or department to look for publicly shared files (FTP, SMB, WWW, etc.).
Or maybe you are just trying to find the IP of a certain printer. You might have scanned your favorite web
site to see if they are offering any other services, or because you were curious what OS they run. Perhaps
you are just trying to test connectivity, or maybe you wanted to do a quick security sanity check before
handing off your credit card details to that e-commerce company. You might be conducting Internet
research. Or are you performing initial reconnaissance in preparation for a break-in attempt? The remote
administrators rarely know your true intentions, and do sometimes get suspicious. The best approach is
to get permission first. I have seen a few people with non-administrative roles land in hot water after
deciding to “prove” network insecurity by launching an intrusive scan of the entire company or campus.
Administrators tend to be more cooperative when asked in advance than when woken up at 3AM by an
IDS alarm claiming they are under massive attack. So whenever possible, obtain written authorization
before scanning a network. Adrian Lamo would probably have avoided jail if he had asked the New York
Times to test their security rather than telling reporters about the flaws afterward. Unfortunately they
would likely have said no. Be prepared for this answer.

Probably at least 90% of network scanning is non-controversial. You are rarely badgered for scanning
your own machine or the networks you administer. The controversy comes when scanning other networks.
There are many reasons (good and bad) for doing this sort of network exploration. Perhaps you are scanning
the other systems in your dorm or department to look for publicly shared files (FTP, SMB, WWW, etc.).
Or maybe you are just trying to find the IP of a certain printer. You might have scanned your favorite web
site to see if they are offering any other services, or because you were curious what OS they run. Perhaps
you are just trying to test connectivity, or maybe you wanted to do a quick security sanity check before
handing off your credit card details to that e-commerce company. You might be conducting Internet
research. Or are you performing initial reconnaissance in preparation for a break-in attempt? The remote
administrators rarely know your true intentions, and do sometimes get suspicious. The best approach is
to get permission first. I have seen a few people with non-administrative roles land in hot water after
deciding to “prove” network insecurity by launching an intrusive scan of the entire company or campus.
Administrators tend to be more cooperative when asked in advance than when woken up at 3AM by an
IDS alarm claiming they are under massive attack. So whenever possible, obtain written authorization
before scanning a network. Adrian Lamo would probably have avoided jail if he had asked the New York
Times to test their security rather than telling reporters about the flaws afterward. Unfortunately they
would likely have said no. Be prepared for this answer.

As noted previously, do not do anything controversial from your work or school connections. Even though
your intentions may be good, you have too much to lose if someone in power (e.g. boss, dean) decides
you are a malicious cracker. Do you really want to explain your actions to someone who may not even
understand the terms packet or port scanner? Spend $40 a month for a dialup, shell, or residential broadband
account. Not only are the repercussions less severe if you offend someone from such an account, but target
network administrators are less likely to even bother complaining to mass-market providers. Also read
the relevant AUP and choose a provider accordingly. If your provider (like Comcast discussed above)
bans any unauthorized port scanning and posting of “offensive” material, do not be surprised if you are
kicked off for this activity. In general, the more you pay to a service provider the more accommodating
they are. A TI provider is highly unlikely to yank your connection without notice because someone reported
being port scanned. A dialup or residential DSL/cable provider very well might. This can happen even
when the scan was forged by someone else.

Nmap offers many options for stealthy scans, including source-lP spoofing, decoy scanning, and the more
recent idle scan technique. These are discussed in the IDS evasion chapter. But remember that there is
always a trade-off. You are harder to find if you launch scans from an open WAP far from your house,
with 17 decoys, while doing subsequent probes through a chain of nine open proxies. But if anyone does
track you down, they will be mighty suspicious of your intentions.

Always have a legitimate reason for performing scans. An offended administrator might write to you first
(or your ISP might forward his complaint to you) expecting some sort of justification for the activity. In
the Scott Moulton case discussed above, VC3 first emailed Scott to ask what was going on. If they had
been satisfied with his answer, matters might have stopped there rather than escalating into civil and
criminal litigation. Groups scanning large portions of the Internet for research purposes often use a
reverse-DNS name that describes their project and run a web server with detailed information and opt-out
forms.
