# malware-gems

## NOTE: WORK IN PROGRESS! (Updated 19 July 2022)


### What's this all about?
This page contains a list of mostly malware analysis / reverse engineering related tools, training, podcasts, blog posts, literature and just about anything else closely related to the topic. This page serves as a catalog of sorts, containing "gems", some of which you may have stumbled across, and many others that you may not have.  


### Who is this page aimed at?

#### Myself:
When first starting out, I was overwhelmed by how malware/RE related material was somewhat scattered all over the Internet. With a limited availability of books and training, I started to collect my go-to sites for certain resources and tools in order to achive certain tasks. 


#### Beginners: 
I often get asked "how do you get started in malware analysis / RE". I'm hoping this list will provide a starting point at least. Anyone who has been practicing malware analysis for even a small amount of time, knows that there really is no single resource or location that will simply teach the art of malware analysis / RE. Plain and simple. That said, having a useful list of links is at least a starting point. However, one caveat is that this list should NOT replace your OWN time spent researching and learning by yourself. This is very much part of "the journey" towards becomming a better malware analyst / RE, similar to that of becomming a l33t h4x0r! ;)


#### Anyone else: 
Regardless of skill/experience level, even the more experienced malware analyst / RE may hopefully find one or two useful gems on this page that they haven't yet stumbled across. This is where the name "malware-gems" originated from... Original, I know.. ;) 


### Isn't this similar to other "awesome" lists that exist on Github?
Perhaps. While the various awesome "awesome" lists (as awesome as they are) gave me inspiration, I wanted to centralise my own tools/links etc due to growing my own malware analysis skills, in the hope that once I have things in one page, things may hopefully become a bit clearer in my head! In some ways, as awesome as the other various "awesome" lists are, I hope that this list will in itself be just as awesome, due to the fact that the this reflects a true and current representation of a malware analyst such as myself, who is building up their own knowledge with active links to tools, reading material etc! 


### Anything else?

If you have any feedback or would like your site listed, feel free to reach out via Twitter.
Twitter handle: [0x4143](https://twitter.com/0x4143)


###### Disclaimer: 
* Full credits/props/respect to all the respective authors for their content.
* I suspect that this list may morph gradually over time to possibly include other infosec related tools/links that aren't directly related to malware or RE, but I will try my very best to stay on topic!  =)
* The links contained in each section are currently in no particular order. 
* I may clean up the order at some point e.g. alphabetize, or order by preference. 
* Some tools/links may likely be in the wrong category, I will review this as time goes on. 
* This is a work-in-progress so bare with me! 
* Sharing is caring, so feel free to forward this link around. 
* "Haters gonna hate"!
* And last but not least, **enjoy! =)




# Books:
* Intelligence Driven Incident Response - http://shop.oreilly.com/product/0636920043614.do
* Practical Malware Analysis - https://www.nostarch.com/malware
* Reversing: Secrets of Reverse Engineering - http://eu.wiley.com/WileyCDA/WileyTitle/productCd-0764574817.html
* Practical Reverse Engineering - http://eu.wiley.com/WileyCDA/WileyTitle/productCd-1118787315,subjectCd-CSJ0.html
* Malware Analyst Cookbook - http://eu.wiley.com/WileyCDA/WileyTitle/productCd-0470613033.html
* IDA Pro Book - https://www.nostarch.com/idapro2.htm
* Art of Assembly - http://www.plantation-productions.com/Webster/www.artofasm.com/index.html
* The Art of Memory Forensics - http://eu.wiley.com/WileyCDA/WileyTitle/productCd-1118825098.html
* Windows Internals, Part 1 (6th Edition) - https://www.microsoftpressstore.com/store/windows-internals-part-1-9780735648739
* Windows Internals, Part 2 (6th Edition) - https://www.microsoftpressstore.com/store/windows-internals-part-2-9780735665873 
* Windows Internals, Part 1 (7th Edition):
https://www.microsoftpressstore.com/store/windows-internals-part-1-system-architecture-processes-9780735684188
* Windows Internals, Part 2 (7th Edition):
https://www.microsoftpressstore.com/store/windows-internals-part-2-9780135462409
* Hacking. The Art of Exploitation - https://www.nostarch.com/hacking2.htm
* The Shellcoder's Handbook: Discovering and Exploiting Security Holes - http://eu.wiley.com/WileyCDA/WileyTitle/productCd-047008023X.html
* Rootkits: Subverting the Windows Kernel - https://dl.acm.org/citation.cfm?id=1076346
* Rootkits and Bootkits - https://www.nostarch.com/rootkits
* The Cuckoo's Egg: Tracking a Spy Through the Maze of Computer Espionage - http://www.simonandschuster.com/books/The-Cuckoos-Egg/Cliff-Stoll/9781416507789
* Rootkits: Subverting the Windows Kernel - https://dl.acm.org/citation.cfm?id=1076346
* The Rootkit Arsenal: Escape and Evasion in the Dark Corners of the System - https://www.safaribooksonline.com/library/view/the-rootkit-arsenal/9781449626365/
* Learning Malware Analysis - https://www.amazon.co.uk/Learning-Malware-Analysis-techniques-investigate/dp/1788392507/ref=sr_1_1?ie=UTF8&qid=1534162748&sr=8-1&keywords=malware+analysis
* Sandworm - https://www.penguinrandomhouse.com/books/597684/sandworm-by-andy-greenberg/



# CheatSheets/Tables:
* IDA Cheat Sheet - https://securedorg.github.io/idacheatsheet.html
* Cheat Sheets - https://highon.coffee/blog/cheat-sheet/
* File Signatures - http://www.garykessler.net/library/file_sigs.html
* APT Groups and Operations - https://docs.google.com/spreadsheets/d/1H9_xaxQHpWaa4O_Son4Gx0YOIzlcBWMsdvePFX68EKU/pubhtml#
* Ransomware Overview - https://docs.google.com/spreadsheets/d/1TWS238xacAto-fLKh1n5uTsdijWdCEsGIM0Y0Hvmc5g/pubhtml#
* Intel Assembler code table - http://www.jegerlehner.ch/intel/
* ARM Assembly Cheatsheet - https://azeria-labs.com/assembly-basics-cheatsheet/
* APTnotes - https://github.com/kbandla/APTnotes
* PE 101 - https://github.com/corkami/pics/blob/master/binary/pe101/pe101.pdf
* PDF 101 - https://github.com/corkami/docs/blob/master/PDF/PDF.md
* PDF analysis - https://github.com/zbetcheckin/PDF_analysis
* Digital Forensics and Incident Response - https://www.jaiminton.com/cheatsheet/DFIR/#



# CTF's:
* Flare-On - http://flare-on.com/
* LabyREnth - https://labyrenth.com/mud/
* Facebook CTF - https://github.com/facebook/fbctf
* CTF Field Guide - https://trailofbits.github.io/ctf/
* RootMe - https://www.root-me.org
* RPISEC CSCI 4968 - http://security.cs.rpi.edu/courses/binexp-spring2015/
* Crackmes - https://crackmes.one/



# Decoders:
* CyberChef - https://gchq.github.io/CyberChef/
* KevtheHermit RAT decoders - https://github.com/kevthehermit/RATDecoders



# Debuggers:
* OllyDbg - http://www.ollydbg.de/
* Immunity Debugger - https://www.immunityinc.com/products/debugger/
* X64dbg - https://x64dbg.com/#start
* Rvmi - https://github.com/fireeye/rvmi
* WinDBG - https://docs.microsoft.com/en-gb/windows-hardware/drivers/debugger/debugger-download-tools



# Disassemblers: 
* IDA Pro - https://www.hex-rays.com/products/ida/
* Binary Ninja - https://binary.ninja/
* Radare2 - https://github.com/radare/radare2
* Cutter - https://github.com/radareorg/cutter
* BinNavi - https://github.com/google/binnavi
* Hopper - https://www.hopperapp.com/
* medusa - https://github.com/wisk/medusa
* Disassembler.io - https://www.onlinedisassembler.com/static/home/
* Ghidra - https://ghidra-sre.org/



# Document Analysis Tools: 
* OfficeMalScanner/DisView - http://www.reconstructor.org/
* AnalyzePDF - https://github.com/hiddenillusion/AnalyzePDF
* BiffView - https://www.aldeid.com/wiki/BiffView
* oletools - https://www.decalage.info/python/oletools
* Origami Framework - https://github.com/cogent/origami-pdf
* PDF Stream Dumper - http://sandsprite.com/blogs/index.php?uid=7&pid=57
* CERMINE - https://github.com/CeON/CERMINE
* pdfid - https://blog.didierstevens.com/programs/pdf-tools/
* PDFwalker - https://www.aldeid.com/wiki/Origami/pdfwalker
* Peepdf - http://eternal-todo.com/tools/peepdf-pdf-analysis-tool
* pev - http://pev.sourceforge.net/
* FOCA - https://www.elevenpaths.com/labstools/foca/index.html
* LuckyStrike - https://github.com/curi0usJack/luckystrike
* RTF Cleaner - https://github.com/nicpenning/RTF-Cleaner
* RTFScan - http://www.reconstructer.org/ 



# Dynamic/Behavioural Analysis Tools: 
* CaptureBAT - https://www.honeynet.org/node/315
* Sysinternals Suite - https://docs.microsoft.com/en-us/sysinternals/downloads/sysinternals-suite
* ProcDOT - http://www.procdot.com/
* Process Hacker - http://processhacker.sourceforge.net/
* Sysmon - https://docs.microsoft.com/en-us/sysinternals/downloads/sysmon
* API Monitor - http://www.rohitab.com/apimonitor
* Regshot - https://sourceforge.net/projects/regshot/
* SwiftonSecurity Sysmon Config - https://github.com/SwiftOnSecurity/sysmon-config
* Capture-Py - https://github.com/fbruzzaniti/Capture-Py
* Windows Kernel Explorer - https://github.com/AxtMueller/Windows-Kernel-Explorer



# Funny/Random: 
* Win95 defrag - http://hultbergs.org/defrag/
* Little Bobby - http://www.littlebobbycomic.com/
* Dilbert - http://dilbert.com/
* XKCD - https://xkcd.com/
* Why the fuck was i breached - https://whythefuckwasibreached.com/
* VIM Adventures - https://vim-adventures.com/



# Honeypots:
* Modern Honey Network - https://github.com/threatstream/mhn



# ICS: 
* Graphical Realism Framework for Industrial Control Simulations - https://github.com/djformby/GRFICS
* ꓘamerka - https://woj-ciech.github.io/kamerka-demo/kamerka.html



# IDA: 
* stackstring_static.py - https://github.com/TakahiroHaruyama/ida_haru/tree/master/stackstring_static
* emotet_payload_decryption.py - https://gist.github.com/levwu/23751fe47f83d42ed6a63280a4f2aaaa
* VB IDC - https://www.hex-rays.com/products/ida/support/freefiles/vb.idc
* Diaphora - https://github.com/joxeankoret/diaphora
* BinDiff - https://www.zynamics.com/bindiff.html
* fnfuzzy - https://github.com/TakahiroHaruyama/ida_haru/tree/master/fn_fuzzy
* BinDiff wrapper - https://github.com/TakahiroHaruyama/ida_haru/tree/master/bindiff
* simpliFiRE.IDAscope - https://bitbucket.org/daniel_plohmann/simplifire.idascope/src/master/
* IDA Plugins - http://www.openrce.org/downloads/browse/IDA_Plugins
* FindCrypt - https://github.com/you0708/ida/tree/master/idapython_tools/findcrypt



# IOT:
* Binwalk - https://github.com/devttys0/binwalk
* JTAG Explained - http://blog.senr.io/blog/jtag-explained
* Firmware Analysis Toolkit - https://github.com/attify/firmware-analysis-toolkit
* Saleae Logic Analyzer software - https://www.saleae.com/downloads/



# IR:
* Detecting Lateral Movement through Tracking Event Logs - https://www.jpcert.or.jp/english/pub/sr/20170612ac-ir_research_en.pdf
* Incident Response Methodologies - https://github.com/certsocietegenerale/IRM
* MITRE ATT&CK Framework - https://attack.mitre.org/wiki/Main_Page



# JavaScript Deobfuscation Tools: 
* SpiderMonkey (js) - https://blog.didierstevens.com/programs/spidermonkey/
* Malzilla - http://malzilla.sourceforge.net/
* Malware-Jail - https://github.com/HynekPetrak/malware-jail



# LNK File Analysis:
* https://lifeinhex.com/analyzing-malicious-lnk-file/



# MAC:
* MacOS Papers, Slides and Thesis Archive - https://papers.put.as/macosx/macosx/
* norimaci - https://github.com/mnrkbys/norimaci
* DTrace: [even better than] strace for OS X - https://8thlight.com/blog/colin-jones/2015/11/06/dtrace-even-better-than-strace-for-osx.html



# Malware Repo's:
* MalwareBazaar - https://bazaar.abuse.ch/
* VXVault - http://vxvault.net/ViriList.php
* MalShare - https://malshare.com/
* CyberCrime Tracker - http://cybercrime-tracker.net/index.php
* TheZoo - https://github.com/ytisf/theZoo
* Endgame Ember - https://github.com/endgameinc/ember
* Global ATM Malware Wall - http://atm.cybercrime-tracker.net/index.php
* What is this C2 - https://github.com/misterch0c/what_is_this_c2
* Connect Trojan - https://www.connect-trojan.com/
* ViriBack C2 Tracker - http://tracker.viriback.com/
* VirusBay - https://beta.virusbay.io/



# Maps / Stats (eye candy):
* ThreatButt - https://threatbutt.com/map/
* BitDefender - https://threatmap.bitdefender.com/
* FireEye - https://www.fireeye.com/cyber-map/threat-map.html
* Global Incident Map - http://www.globalincidentmap.com/
* Tor Flow - https://torflow.uncharted.software/
* Kaspersky Cybermap - https://cybermap.kaspersky.com/
* Security Wizardry - http://www.securitywizardry.com/radar.htm
* Norse Attack Map - http://map.norsecorp.com/#/
* Digital Attack Map - http://www.digitalattackmap.com/#anim=1&color=0&country=ALL&list=0&time=16938&view=map
* Stats - http://breachlevelindex.com/
* Current Cyber Attacks - http://community.sicherheitstacho.eu/start/main
* FSecure - http://worldmap3.f-secure.com/
* Talos - https://talosintelligence.com/
* Security Wizardry - https://radar.securitywizardry.com/
* Ransomware Attack Map - https://statescoop.com/ransomware-map/



# Memory Forensics: 
* Volatility - http://www.volatilityfoundation.org/
* Memoryze - https://www.fireeye.com/services/freeware/memoryze.html
* DumpIt - https://blog.comae.io/your-favorite-memory-toolkit-is-back-f97072d33d5c
* Hibr2Bin - https://blog.comae.io/your-favorite-memory-toolkit-is-back-f97072d33d5c
* Rekall Memory Forensic Framework - https://github.com/google/rekall
* Clonezilla - http://clonezilla.org/
* dd - https://linux.die.net/man/1/dd
* Fog - https://fogproject.org/
* Forensic Toolkit (FTK) - http://www.accessdata.com/product-download
* Redline - https://www.fireeye.com/services/freeware/redline.html
* MemLabs - https://github.com/stuxnet999/MemLabs



# Misc Tools: 
* File Signature Analysis - https://filesignatures.net/index.php?page=all
* EKFiddle - https://github.com/malwareinfosec/EKFiddle
* XMind - http://www.xmind.net/
* ExamDiff - http://www.prestosoft.com/edp_examdiff.asp
* 7zip - http://www.7-zip.org/download.html
* Visual Studio - https://www.visualstudio.com/
* WinSCP - https://winscp.net/eng/download.php
* Putty - https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html
* TreeSizeFree - https://www.jam-software.com/treesize_free/
* OneNote - https://www.onenote.com/
* KeePass - https://keepass.info/
* ExifTool - https://www.sno.phy.queensu.ca/~phil/exiftool/
* RegEx 101 - https://regex101.com/
* Byte Counter - https://mothereff.in/byte-counter
* Utilu IE Collection - http://utilu.com/IECollection/
* UserAgentString - http://www.useragentstring.com/
* Maltego - https://www.paterva.com/web7/buy/maltego-clients/maltego-ce.php
* Cmder - http://cmder.net/
* MalPull - https://github.com/ThisIsLibra/MalPull
* StringSifter - https://github.com/mandiant/stringsifter



# .Net Debuggers/Decompilers:
* ILSpy - http://ilspy.net/
* dnSpy - https://github.com/0xd4d/dnSpy
* dotPeek - https://www.jetbrains.com/decompiler/
* de4dot - https://github.com/0xd4d/de4dot
* Reflector - https://www.red-gate.com/products/dotnet-development/reflector/index



# Network Analysis: 
* Wireshark - https://www.wireshark.org/ 
* Network Miner - http://www.netresec.com/?page=NetworkMiner
* LogRhythm Network Monitor Freemium - https://logrhythm.com/network-monitor-freemium/
* dig - https://linux.die.net/man/1/dig
* curl - https://curl.haxx.se/docs/manpage.html
* ApateDNS - https://www.fireeye.com/services/freeware/apatedns.html
* NetCat - http://netcat.sourceforge.net/
* Nslookup - https://linux.die.net/man/1/nslookup
* PDF Stream Dumper - http://sandsprite.com/blogs/index.php?uid=7&pid=57
* Robtex - https://www.robtex.com/
* Belati - https://github.com/aancw/Belati
* Ostinato - http://ostinato.org/
* Burp Suite - https://portswigger.net/burp/
* Hak5 - https://hakshop.com/
* Fiddler - https://www.telerik.com/fiddler
* Shodan - https://www.shodan.io/
* FakeNet-NG - https://github.com/fireeye/flare-fakenet-ng
* Netzob - https://github.com/netzob/netzob
* DShell - https://github.com/USArmyResearchLab/Dshell
* SecurityOnion - https://securityonion.net/
* Reverse engineering network protocols - Reverse Engineering Network Protocols
* MITMProxy - https://mitmproxy.org/
* DNSChef - https://github.com/iphelix/dnschef



# Operating Systems: 
* Remnux - https://remnux.org/
* SIFT - https://digital-forensics.sans.org/community/downloads 
* Kali - https://www.kali.org/
* CAINE - http://www.caine-live.net/
* Metasploitable 3 - https://github.com/rapid7/metasploitable3
* DVWA - http://www.dvwa.co.uk/
* Security Onion - https://securityonion.net/
* FLARE VM - https://www.fireeye.com/blog/threat-research/2017/07/flare-vm-the-windows-malware.html
* OWASP WebGoat - https://www.owasp.org/index.php/WebGoat_Installation#Installing_to_Windows
* OWASP Bricks - https://www.owasp.org/index.php/OWASP_Bricks
* OWASP Mantra - http://www.getmantra.com/
* Tails - https://tails.boum.org/
* Whonix - https://www.whonix.org/
* Santoku - https://santoku-linux.com/about-santoku/



# OSINT Online Tools:
* OSINT Gathering - https://posts.specterops.io/gathering-open-source-intelligence-bee58de48e05
* Automating OSINT Blog - http://www.automatingosint.com/blog/
* SpiderFoot - https://www.spiderfoot.net/
* Buscador - https://inteltechniques.com/buscador/



# Password Cracking:
* Hashcat - https://github.com/hashcat/hashcat
* Crack.sh - https://crack.sh/
* Mimikatz - https://github.com/gentilkiwi/mimikatz
* Ophcrack - http://ophcrack.sourceforge.net/



# Podcasts:
* Security Now - https://www.grc.com/securitynow.htm
* SANS Stormcast - https://isc.sans.edu/podcast.html
* Down the Security Rabbithole - http://podcast.wh1t3rabbit.net/
* Defensive Security - https://defensivesecurity.org/category/podcast/
* Paul's Security Weekly - https://wiki.securityweekly.com/Show_Notes
* RunAs Radio - http://www.runasradio.com/
* Defensive Security Podcast - https://defensivesecurity.org/
* Darknet Diaries - https://darknetdiaries.com/
* Risky Business Podcast - https://risky.biz/
* Security Nation Podcast - https://podcasts.apple.com/gb/podcast/security-nation/id1124543784
* Smashing Security - https://www.smashingsecurity.com/



# PowerShell decoding:
* PSDecode - https://github.com/R3MRUM/PSDecode
* PyPowerShellXray - https://github.com/JohnLaTwC/PyPowerShellXray
* PowerShellRunBox: Analyzing PowerShell Threats Using PowerShell Debugging - https://darungrim.com/research/2019-10-01-analyzing-powershell-threats-using-powershell-debugging.html



# Ransomware:
* No More Ransomware - https://www.nomoreransom.org/en/index.html
* ID Ransomware - https://id-ransomware.malwarehunterteam.com/
* Emisoft decrypters - https://www.emsisoft.com/ransomware-decryption-tools/



# Reading Material: 
* Reverse Engineering for Beginners - https://beginners.re/
* Phrack - http://phrack.org/
* Crypto 101 - https://www.crypto101.io/
* Hacker Manifesto - http://phrack.org/issues/7/3.html
* How to Become a Hacker - http://www.catb.org/esr/faqs/hacker-howto.html
* Zines - https://github.com/fdiskyou/Zines
* Hackaday - https://hackaday.com/blog/
* Hacktress - http://www.hacktress.com/
* Reddit - https://www.reddit.com/r/ReverseEngineering/
* Windows API Index - https://msdn.microsoft.com/en-gb/library/windows/desktop/hh920508(v=vs.85).aspx
* Raw Hex - https://rawhex.com/
* DigiNinja - https://digi.ninja/
* Team Cymru - http://www.team-cymru.org/index.html
* Lenny Zeltser - https://zeltser.com/malicious-software/
* OverAPI - http://overapi.com/
* HackBack - https://pastebin.com/0SNSvyjJ
* FlexiDie - https://pastebin.com/raw/Y1yf8kq0
* DefCon archive - https://media.defcon.org/
* Malwology - https://malwology.com/
* Stuxnet's Footprint in memory with Volatility - http://mnin.blogspot.co.uk/2011/06/examining-stuxnets-footprint-in-memory.html
* AtomBombing - https://breakingmalware.com/injection-techniques/atombombing-brand-new-code-injection-for-windows/
* Malware Archaeology - https://www.malwarearchaeology.com/cheat-sheets
* ShinoLocker - https://shinolocker.com/
* A crash course in x86 assembly for reverse engineers - https://sensepost.com/blogstatic/2014/01/SensePost_crash_course_in_x86_assembly-.pdf
* Zero Days, Thousands of Nights - https://www.rand.org/pubs/research_reports/RR1751.html
* Shadow Brokers Exploit Reference Table - https://docs.google.com/spreadsheets/d/1sD4rebofrkO9Rectt5S3Bzw6RnPpbJrMV-L1mS10HQc/edit#gid=1602324093
* GracefulSecurity - https://www.gracefulsecurity.com/infrastructure-security-articles/
* Cybersecurity ain't easy. Let's talk about it - https://itspmagazine.com/itsp-chronicles/cybersecurity-ain-t-easy-lets-talk-about-it
* How to become the best malware analyst e-v-e-r - http://www.hexacorn.com/blog/2018/04/14/how-to-become-the-best-malware-analyst-e-v-e-r/
* Definitive Dossier of Devilish Debug Details – Part One: PDB Paths and Malware - https://www.fireeye.com/blog/threat-research/2019/08/definitive-dossier-of-devilish-debug-details-part-one-pdb-paths-malware.html
* Dr Fu's Security Blog - http://fumalwareanalysis.blogspot.com/p/malware-analysis-tutorials-reverse.html
* Encoding vs. Encryption vs. Hashing vs. Obfuscation - https://danielmiessler.com/study/encoding-encryption-hashing-obfuscation/
* Introduction to reverse engineering and Assembly - https://kakaroto.homelinux.net/2017/11/introduction-to-reverse-engineering-and-assembly/
* Getting started with reverse engineering - https://lospi.net/developing/software/software%20engineering/reverse%20engineering/assembly/2015/03/06/reversing-with-ida.html
* Guide to x86 Assembly - http://www.cs.virginia.edu/~evans/cs216/guides/x86.html
* Nightmare (RE) - https://github.com/guyinatuxedo/nightmare
* PDB Files: What Every Developer Must Know - https://www.wintellect.com/pdb-files-what-every-developer-must-know
* BOLO: Reverse Engineering — Part 1 (Basic Programming Concepts) - https://medium.com/bugbountywriteup/bolo-reverse-engineering-part-1-basic-programming-concepts-f88b233c63b7
* BOLO: Reverse Engineering — Part 2 (Advanced Programming Concepts) - https://medium.com/@danielabloom/bolo-reverse-engineering-part-2-advanced-programming-concepts-b4e292b2f3e
* String Hashing: Reverse Engineering an Anti-Analysis Control - https://r3mrum.wordpress.com/2018/02/15/string-hashing-reverse-engineering-an-anti-analysis-control/
* Ground Zero: Part 1 – Reverse Engineering Basics – Linux x64 - https://0xdarkvortex.dev/index.php/2018/04/09/ground-zero-part-1-reverse-engineering-basics/
* Let's Build a Compiler - https://compilers.iecc.com/crenshaw/
* Static Malware Analysis with OLE Tools and CyberChef - https://newtonpaul.com/static-malware-analysis-with-ole-tools-and-cyber-chef/
* An Introduction to Reverse Engineering - https://www.muppetlabs.com/~breadbox/txt/bure.html
* VXUnderground - https://vx-underground.org/papers.html
* Tracking Advanced Persistent Threats (APTs) via Shared Code - https://medium.com/@arun_73782/tracking-apts-by-shared-code-5e88a2ae2363
* YARA Hunting for Code Reuse: DoppelPaymer Ransomware & Dridex Families - https://www.sentinelone.com/blog/yara-hunting-for-code-reuse-doppelpaymer-ransomware-dridex-families/
* Here We GO: Crimeware Virus & APT Journey From “RobbinHood” to APT28 - https://www.sentinelone.com/blog/here-we-go-crimeware-apt-journey-from-robbinhood-to-apt28/
* The mysterious case of CVE-2016-0034: the hunt for a Microsoft Silverlight 0-day - https://securelist.com/the-mysterious-case-of-cve-2016-0034-the-hunt-for-a-microsoft-silverlight-0-day/73255/
* Process Injection part 1 of 5 - https://3xpl01tc0d3r.blogspot.com/2019/08/process-injection-part-i.html
* OSINT : Chasing Malware + C&C Servers - https://medium.com/secjuice/chasing-malware-and-c-c-servers-in-osint-style-3c893dc1e8cb
* Daily dose of malware - https://github.com/woj-ciech/Daily-dose-of-malware
* Tracking Malware with Import Hashing - https://www.fireeye.com/blog/threat-research/2014/01/tracking-malware-import-hashing.html
* STOMP 2 DIS: Brilliance in the (Visual) Basics - https://www.fireeye.com/blog/threat-research/2020/01/stomp-2-dis-brilliance-in-the-visual-basics.html
* Advanced Binary Deobfuscation - https://github.com/malrev/ABD
* A Case Study Into Solving Crypters/packers in Malware Obfuscation Using an SMT Approach - https://vixra.org/abs/2002.0183
* ReCon Montreal Archives - https://recon.cx/2019/montreal/archives/
* FLARE IDA Pro Script Series: MSDN Annotations IDA Pro for Malware Analysis - https://www.fireeye.com/blog/threat-research/2014/09/flare-ida-pro-script-series-msdn-annotations-ida-pro-for-malware-analysis.html
* Analyzing Modern Malware Techniques - Part 1 (of 4) - https://0x00sec.org/t/analyzing-modern-malware-techniques-part-1/18663
* What Every Computer Programmer Should Know About Windows API, CRT, and the Standard C++ Library - https://www.codeproject.com/Articles/22642/What-Every-Computer-Programmer-Should-Know-About-W
* theForger's Win32 API Programming Tutorial - http://www.winprog.org/tutorial/start.html
* Unbreakable Cryptography in 5 Minutes - https://blog.xrds.acm.org/2012/08/unbreakable-cryptography-in-5-minutes/
* Let’s play (again) with Predator the thief - https://fumik0.com/2019/12/25/lets-play-again-with-predator-the-thief/
* VMProtect Introduction - https://shhoya.github.io/vmp_vmpintro.html
* Azorult loader stages - https://maxkersten.nl/binary-analysis-course/malware-analysis/azorult-loader-stages/
* Reversing Malware Command and Control: From Sockets to COM - https://www.fireeye.com/blog/threat-research/2010/08/reversing-malware-command-control-sockets.html
* Indicators of Compromise (IoCs) and Their Role in Attack Defence - https://tools.ietf.org/html/draft-paine-smart-indicators-of-compromise-00
* Zombieland CTF – Reverse Engineering for Beginners - https://mcb101.blog/2019/10/11/zombieland-ctf-reverse-engineering-for-beginners/
* Fu11Shade Windows Exploitation - https://fullpwnops.com/windows-exploitation-pathway.html



# Sandbox Tools (Online):
* VirusTotal - https://www.virustotal.com
* Malwr - https://malwr.com/ 
* Reverse.it - https://www.reverse.it/
* Open Analysis - http://www.openanalysis.net/
* ANY.RUN - https://any.run/
* Hybrid Analysis - https://www.hybrid-analysis.com/
* Intezer Analyze - https://analyze.intezer.com/



# Sandbox Tools (Offline): 
* Noriben - https://github.com/Rurik/Noriben
* Cuckoo - https://www.cuckoosandbox.org/
* PyREBox - https://github.com/Cisco-Talos/pyrebox
* Viper - http://viper.li/
* MISP - http://www.misp-project.org/
* Sandboxie - https://www.sandboxie.com/
* Ph0neutria - https://github.com/phage-nz/ph0neutria
* FlareVM - https://www.fireeye.com/blog/threat-research/2017/07/flare-vm-the-windows-malware.html



# Shellcode Tools: 
* JMP2IT - https://github.com/adamkramer/jmp2it
* Shellcode2exe.py - https://github.com/MarioVilas/shellcode_tools
* ConvertShellCode - http://le-tools.com/ConvertShellcode.html
* scdbg - http://sandsprite.com/blogs/index.php?uid=7&pid=152



# Static Analysis Tools: 
* PEiD -https://www.aldeid.com/wiki/PEiD
* McAfee FileInsight - https://www.mcafee.com/uk/downloads/free-tools/fileinsight.aspx
* HashMyFiles - http://www.nirsoft.net/utils/hash_my_files.html 
* CFF Explorer - http://www.ntcore.com/exsuite.php
* AnalyzePESig - https://blog.didierstevens.com/2012/10/01/searching-for-that-adobe-cert/
* ByteHist - https://www.cert.at/downloads/software/bytehist_en.html
* Exeinfo - http://exeinfo.pe.hu/
* Scylla - https://github.com/NtQuery/Scylla
* MASTIFF - https://git.korelogic.com/mastiff.git/
* PEframe - https://github.com/guelfoweb/peframe
* PEscan - https://tzworks.net/prototype_page.php?proto_id=15
* PEstudio - https://www.winitor.com/
* PE-Bear - https://hshrzd.wordpress.com/2013/07/09/introducing-new-pe-files-reversing-tool/
* PE-sieve - https://github.com/hasherezade/pe-sieve
* Flare-Floss - https://github.com/fireeye/flare-floss
* PatchDiff2 - https://github.com/filcab/patchdiff2
* PE Insider - http://cerbero.io/peinsider/
* Resource Hacker - http://www.angusj.com/resourcehacker/
* DarunGrim - https://github.com/ohjeongwook/DarunGrim
* Mal Tindex - https://github.com/joxeankoret/maltindex
* Manalyze - https://github.com/JusticeRage/Manalyze
* PDBlaster - https://github.com/SecurityRiskAdvisors/PDBlaster
* ImpFuzzy - https://github.com/JPCERTCC/impfuzzy
* Florentino - https://github.com/0xsha/florentino/blob/master/README.md
* Viper - https://viper.li/en/latest/



# Text/hex Editor Tools:
* Notepad++ - https://notepad-plus-plus.org/
* 010 Editor - https://www.sweetscape.com/010editor/
* HxD - https://mh-nexus.de/en/hxd/
* BinText - https://www.aldeid.com/wiki/BinText
* Hexinator - https://hexinator.com/



# Threat Intelligence:
* ThreatMiner - https://www.threatminer.org/
* RiskIQ Community - https://community.riskiq.com/home
* PasteBin - https://pastebin.com/
* Shodan - https://www.shodan.io/
* Censys - https://censys.io/
* DNSdumpster - https://dnsdumpster.com/
* URLHaus - https://urlhaus.abuse.ch/
* AlienVault OTX - https://otx.alienvault.com/
* C2 Tracker - http://tracker.viriback.com/stats.php
* MISP - https://www.misp-project.org/
* The Hive - https://thehive-project.org/
* Yeti - https://yeti-platform.github.io/
* Using ATT&CK for CTI Training - https://attack.mitre.org/resources/training/cti/
* PasteScraper - https://github.com/PimmyTrousers/pastescraper



# Training: 
* Cybrary - https://www.cybrary.it/
* Corelan Team - https://www.corelan.be/
* Open Security Training - http://opensecuritytraining.info/Training.html
* Offensive Computer Security - http://www.cs.fsu.edu/~redwood/OffensiveComputerSecurity/lectures.html
* PentesterLab - https://pentesterlab.com/
* Malware Traffic Analysis - http://www.malware-traffic-analysis.net/training-exercises.html
* MIT Open Courseware - https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-858-computer-systems-security-fall-2014/video-lectures/
* OALabs - https://vimeo.com/oalabs
* OALabs - https://www.youtube.com/channel/UC--DwaiMV-jtO-6EvmKOnqg/videos
* MalwareAnalysisForHedgeHogs - https://www.youtube.com/channel/UCVFXrUwuWxNlm6UNZtBLJ-A
* Malware Unicorn - https://securedorg.github.io/
* Tuts4You - https://tuts4you.com/
* Lenas Reversing for Newbies - https://tuts4you.com/download.php?list.17
* Introduction to WinDBG - https://www.youtube.com/watch?list=PLhx7-txsG6t6n_E2LgDGqgvJtCHPL7UFu&time_continue=1&v=8zBpqc3HkSE
* Colin Hardy - https://www.youtube.com/channel/UCND1KVdVt8A580SjdaS4cZg/videos
* OWASP AppSec Tutorials - http://owasp-academy.teachable.com/p/owasp-appsec-tutorials
* Modern Binary Exploitation - https://github.com/RPISEC/MBE
* FuzzySecurity - http://www.fuzzysecurity.com/tutorials.html
* Linux Journey - https://linuxjourney.com/
* Pivot Project - http://pivotproject.org/
* Security Tube - http://www.securitytube-training.com/index.html
* Packet Life Cheat Sheets - http://packetlife.net/library/cheat-sheets/?_escaped_fragment_=#!
* SecurityXploded - http://securityxploded.com/
* MalwareMustDie - https://www.youtube.com/playlist?list=PLSe6fLFf1YDX-2sog70220BchQmhVqQ75
* Win32Assembly - http://win32assembly.programminghorizon.com/tutorials.html
* RPISEC - https://github.com/RPISEC/Malware/blob/master/README.md
* RPISEC - https://github.com/RPISEC/MBE
* Reverse Engineering Challenges - https://challenges.re/
* HackerOne - https://www.hackerone.com/
* Google Python Class - https://developers.google.com/edu/python/
* Guide to x86 Assembly - http://www.cs.virginia.edu/~evans/cs216/guides/x86.html
* Code Blocks - http://www.codeblocks.org/
* Wireshark Course - https://www.youtube.com/watch?v=XTSc2mPF4II&t=25s
* Maltrak Malware Analyst webinar - http://maltrak.com/webinar-registration
* Intro to ARM assembly basics - https://azeria-labs.com/writing-arm-assembly-part-1/
* Life in Hex - https://lifeinhex.com/category/reversing/
* The Cuckoo's Egg Decompiled Online Course - http://chrissanders.org/cuckoosegg/
* Creating Yara Rules for Malware Detection - https://www.real0day.com/hacking-tutorials/yara
* Windows Privilege Escalation Guide - https://www.sploitspren.com/2018-01-26-Windows-Privilege-Escalation-Guide/
* Amr Thabet shellcode training - https://www.youtube.com/channel/UCkY_8Hz8ojyQQ9S6bPnHa7g
* Hexacorn Converting Shellcode to Portable Executable (32- and 64- bit) - http://www.hexacorn.com/blog/2015/12/10/converting-shellcode-to-portable-executable-32-and-64-bit/
* Learn Forensics with David Cowen - https://www.youtube.com/user/LearnForensics/featured
* Raphael Mudge (various, In-memory evasion/detection) - https://www.youtube.com/user/DashnineMedia/videos
* Assembly programming tutorial - https://www.tutorialspoint.com/assembly_programming/index.htm
* RPISec Training - https://github.com/RPISEC/Malware
* Intro to Computer Science - https://www.edx.org/course/introduction-to-computer-science-and-programming-7
* Ringzer0 - https://www.ringzer0.training/
* Reversing Hero - https://www.reversinghero.com/
* MIT Open Courseware - https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-00-introduction-to-computer-science-and-programming-fall-2008/video-lectures/
* Reverse Engineering and malware analysis 101 - https://github.com/abhisek/reverse-engineering-and-malware-analysis
* Reverse engineering intel x64 - https://github.com/0xdidu/Reverse-Engineering-Intel-x64-101
* C++ Tutorial for Beginners - Full Course - https://www.youtube.com/watch?v=vLnPwxZdW4Y
* ELF Reversing Tutorial - https://www.youtube.com/playlist?list=PLsNNY-Xea3ra42GZDnvTB46G4p-5oUpFf
* Adversary Tactics: PowerShell - https://github.com/specterops/at-ps
* Malware Unicorn Reverse Engineering 101 - https://malwareunicorn.org/workshops/re101.html#0
* Modern Binary Exploitation - http://security.cs.rpi.edu/courses/binexp-spring2015/
* Ghidra Courses - https://ghidra.re/online-courses/
* Technical Writing Courses - https://developers.google.com/tech-writing
* Introduction to Malware Analysis and Reverse Engineering - https://class.malware.re/
* Binary Analysis Course - https://maxkersten.nl/binary-analysis-course/
* Josh Stroschein - https://www.youtube.com/user/jstrosch/videos
* How to hack together your own CS degree online for free - https://www.freecodecamp.org/news/how-to-hack-your-own-cs-degree-for-free/
* Zero 2 Automated - https://courses.zero2auto.com/adv-malware-analysis-course



# Unpacking:
* UnpacMe - https://www.unpac.me/#/
* Unipacker - https://github.com/unipacker/unipacker



# VBA Deobfuscation Tools: 
* pcodedmp - https://github.com/bontchev/pcodedmp
* vba-dynamic-hook - https://github.com/eset/vba-dynamic-hook
* ViperMonkey - https://github.com/decalage2/ViperMonkey



# Video:
* Teach Yourself Computer Science - https://teachyourselfcs.com/
* CS50 at Harvard - https://cs50.harvard.edu/
* J4vv4D - https://www.j4vv4d.com/videos/
* Movies for Hackers - https://github.com/k4m4/movies-for-hackers
* Can You Hack It - https://www.youtube.com/watch?v=GWr5kbHt_2E
* Chris Nickerson talk - http://www.irongeek.com/i.php?page=videos/derbycon5/teach-me14-started-from-the-bottom-now-im-here-how-to-ruin-your-life-by-getting-everything-you-ever-wanted-chris-nickerson
* Zoz - Don't Fuck it Up - https://www.youtube.com/watch?v=J1q4Ir2J8P8
* Rob Joyce (NSA) - Disrupting Nation State Hackers - https://www.youtube.com/watch?v=bDJb8WOJYdA
* Movies for Hackers - https://github.com/k4m4/movies-for-hackers
* Wannacry: The Marcus Hutchins Story - All 3 Chapters - https://www.youtube.com/watch?v=vveLaA-z3-o&t=451s
* DEF CON 23 - Chris Domas - Repsych: Psychological Warfare in Reverse Engineering - https://www.youtube.com/watch?v=HlUe0TUHOIc
* SAS2018: Finding aliens, star weapons and ponies with YARA - https://www.youtube.com/watch?v=fbidgtOXvc0



# XOR Decoding Tools:
* bbcrack - https://www.decalage.info/python/balbuzard
* Brutexor - https://www.aldeid.com/wiki/Brutexor-iheartxor
* ConverterNET - http://www.kahusecurity.com/2017/converternet-v0-1-released/
* NoMoreXOR - https://github.com/hiddenillusion/NoMoreXOR



# Yara Related:
* Yara - https://virustotal.github.io/yara/
* Stringless Yara Rules - https://inquest.net/blog/2018/09/30/yara-performance
* YarGen - https://github.com/Neo23x0/yarGen
* Yara-Rules - https://github.com/Yara-Rules/rules
* CONFidence 2019: "Utilizing YARA to Find Evolving Malware" - Jay Rosenberg - https://www.youtube.com/watch?v=XMZ-c2Zwzjg
* SANS Webcast - YARA - Effectively using and generating rules - https://www.youtube.com/watch?v=5A_O8X_JljI
* Klara - https://github.com/KasperskyLab/klara
* Open Source Yara Rules - https://github.com/mikesxrs/Open-Source-YARA-rules
