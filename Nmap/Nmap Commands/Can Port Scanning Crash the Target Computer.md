# Can Port Scanning Crash the Target Computer

Nmap does not have any features designed to crash target networks. It usually tries to tread lightly. For
example, Nmap detects dropped packets and slows down when they occur in order to avoid overloading the
network. Nmap also does not send any corrupt packets. The IP, TCP, UDP, and ICMP headers are always
appropriate, though the destination host is not necessarily expecting the packets. For these reasons, no
application, host, or network component should ever crash based on an Nmap scan. If they do, that is a bug
in the system which should be repaired by the vendor.

Reports of systems being crashed by Nmap are rare, but they do happen. Many of these systems were probably
unstable in the first place and Nmap either pushed them over the top or they crashed at the same time as an
Nmap scan by pure coincidence. In other cases, poorly written applications, TCP/IP stacks, and even operating
systems have been demonstrated to crash reproducibly given a certain Nmap command. These are usually
older legacy devices, as newer equipment is rarely released with these problems. Smart companies use Nmap
and many other common network tools to test devices prior to shipment. Those who omit such pre-release
testing often find out about the problem in early beta tests when a box is first deployed on the Internet. It
rarely takes long for a given IP to be scanned as part of Internet white noise. Keeping systems and devices
up-to-date with the latest vendor patches and firmware should reduce the susceptibility of your machines to
these problems, while also improving the security and usability of your network.

In many cases, finding that a machine crashes from a certain scan is valuable information. After all, attackers
can do anything Nmap can do by using Nmap itself or their own custom scripts. Devices should not crash
from being scanned and if they do, vendors should be pressured to provide a patch. In some usage scenarios,
detecting fragile machines by crashing them is undesirable. In those cases you may want to perform very
light scanning to reduce the risk of adverse effects. **Here are a few suggestions**:

- Use SYN scan (- s S ) instead of connect scan (-sT ). User-mode applications such as web servers can
rarely even detect the former because it is all handled in kernel space (some older Linux kernels are an
exception) and thus the services have no excuse to crash.

- Version scanning (-sV ) risks crashing poorly written applications. Similarly, some pathetic operating
systems have been reported to crash when OS fingerprinted (-0 ). Omit these options for particularly
sensitive environments or where you do not need the results.
-  Using -T2 or slower (-T 1, -TO) timing modes can reduce the chances that a port scan will harm a system,
though they slow your scan dramatically. Older Linux boxes had an identd daemon that would block
services temporarily if they were accessed too frequently. This could happen in a port scan, as well as
during legitimate high-load situations. Slower timing might help here. These slow timing modes should
only be used as a last resort as they can slow scans by an order of magnitude or more.

- Limit the number of ports and machines scanned to the fewest that are required. Every machine scanned
has a minuscule chance of crashing, and so cutting the number of machines down improves your odds.
Reducing the number of ports scanned reduces the risks to end hosts as well as network devices. Many
NAT/firewall devices keep a state entry for every port probe. Most of them expire old entries when the
table fills up, but occasional (pathetic) implementations crash instead. Reducing the ports/hosts scanned
reduces the number of state entries and thus might help those sorry devices stay up.
