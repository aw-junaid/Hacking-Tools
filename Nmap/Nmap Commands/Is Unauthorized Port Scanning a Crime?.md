# Is Unauthorized Port Scanning a Crime?

The best way to avoid controversy when using Nmap is to always secure written authorization from the target
network representatives before initiating any scanning. There is still a chance that your ISP will give you
trouble if they notice it (or if the target administrators accidentally send them an abuse report), but this is
usually easy to resolve. When you are performing a penetration test, this authorization should be in the
Statement of Work. When testing your own company, make certain that this activity clearly falls within your
job description. Security consultants should be familiar with the excellent Open Source Security Testing
Methodology Manual (OSSTMM), which provides best practices for these situations.

While civil and (especially) criminal court cases are the nightmare scenario for Nmap users, these are very
rare. After all, no United States federal laws explicitly make port scanning illegal. A much more frequent
occurrence is that the target network will notice a scan and send a complaint to the network service provider
where the scan initiated (your ISP). Most network administrators do not seem to care or notice the many
scans bouncing off their networks daily, but a few complain. The scan source ISP may track down the user
corresponding to the reported IP address and time, then chide the user or even kick them off the service. Port
scanning without authorization is sometimes against the provider's acceptable use policy (AUP). For example,
the AUP for the huge cable-modem ISP Comcast presently says:

> Network probing or port scanning tools are only permitted when used in conjunction with
a residential home network, or if explicitly authorized by the destination host and/or
network. Unauthorized port scanning, for any reason, is strictly prohibited.

Even if an ISP does not explicitly ban unauthorized port scanning, they might claim that some “anti-hacking”
provision applies. Of course this does not make port scanning illegal. Many perfectly legal and (in the United
States) constitutionally protected activities are banned by ISPs. For example, the AUP quoted above also
prohibits users from transmitting, storing, or posting “any information or material which a reasonable person
could deem to be objectionable, offensive, indecent, pornographic, ... embarrassing, distressing, vulgar,
hateful, racially or ethnically offensive, or otherwise inappropriate, regardless of whether this material or its
dissemination is unlawful.” In other words, some ISPs ban any behavior that could possibly offend or annoy
someone. Indiscriminate scanning of other people's networks/computers does have that potential. If you
decide to perform such controversial scanning anyway, never do it from work, school, or any other service
provider that has substantial control over your well-being. Use a dialup or commercial broadband provider
instead. Losing your DSL connection and having to change providers is a slight nuisance, but it is
immeasurably preferable to being expelled or fired.

