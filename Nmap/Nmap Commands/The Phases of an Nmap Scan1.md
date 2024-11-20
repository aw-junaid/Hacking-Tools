# The Phases of an Nmap Scan

Target enumeration. In this phase, Nmap researches the host specifiers provided by the user, which may
be a combination of host DNS names, IP addresses, CIDR network notations, and more. You can even use
(-iR) to ask Nmap to choose your targets for you! Nmap resolves these specifiers into a list of IPv4 or IPv6
addresses for scanning. This phase cannot be skipped since it is essential for further scanning, but you can
simplify the processing by passing just IP addresses so Nmap doesn't have to do forward resolution. If you
pass the - s L - n options (list scan with no reverse-DNS resolution), Nmap will print out the targets and
perform no further scanning.

Host discovery (ping scanning). Network scans usually begin by discovering which targets on the network
are online and thus worth deeper investigation. This process is called host discovery or ping scanning. Nmap
offers many host discovery techniques, ranging from quick ARP requests to elaborate combinations of TCP,
ICMP, and other types of probes. This phase is run by default, though you can skip it (simply assume all
target IPs are online) using the - P N (no ping) option. To quit after host discovery, specify - s P -n.

Reverse-DNS resolution. Once Nmap has determined which hosts to scan, it looks up the reverse-DNS
names of all hosts found online by the ping scan. Sometimes a host's name provides clues to its function,
and names make reports more readable than providing only IP numbers. This step may be skipped with the - n (no resolution) option, or expanded to cover all target IPs (even down ones) with - R (resolve all).

Port scanning. This is Nmap's fundamental operation. Probes are sent, and the responses (or non-responses)
to those probes are used to classify remote ports into states such as o p en , c l o s e d , or f i l t e r e d . That brief description doesn't begin to encompass Nmap's many scan types, configurability of scans, and algorithms
for improving speed and accuracy. Detailed information on algorithms and command-line options, Port scanning is performed by default, though
you can skip it and still perform some of the later traceroute and partial Nmap Scripting Engine phases by
specifying their particular command-line options (such as — t r a c e r o u t e and — s c r i p t ) along with a
ping scan (-sP).

Version detection. If some ports are found to be open, Nmap may be able to determine what server software
is running on the remote system. It does this by sending a variety of probes and matching the responses
against a database of thousands of known service signatures. Version detection is enabled by the - s V option.

OS detection. If requested with the -O option, Nmap proceeds to OS detection. Different operating systems
implement network standards in subtly different ways. By measuring these differences it is often possible
to determine the operating system running on a remote host. Nmap matches responses to a standard set of
probes against a database of more than a thousand known operating system responses.

Traceroute. Nmap contains an optimized traceroute implementation, enabled by the — t r a c e r o u t e
option. It can find the network routes to many hosts in parallel, using the best available probe packets as
determined by Nmap's previous discovery phases. Traceroute usually involves another round of reverse-DNS
resolution for the intermediate hosts. More information is found in Section 15.4, “Host Discovery”.
Script scanning. The Nmap Scripting Engine (NSE) uses a collection of special-purpose scripts to gain even
more information about remote systems. NSE is powered by the Lua programming language and a standard
library designed for network information gathering. Among the facilities offered are advanced version
detection, notification of service vulnerabilities, and discovery of backdoors and other malware. NSE is not executed unless you request it with options such
as — s c r i p t or -sC.

Output. Finally, Nmap collects all the information it has gathered and writes it to the screen or to a file.
Nmap can write output in several formats. Its default, human-readable format (interactive format) is usually
presented in this book. Nmap also offers an XML-based output format, among others.
