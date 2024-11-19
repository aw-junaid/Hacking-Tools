# Awesome Linux Rootkits ![Awesome](https://cdn.rawgit.com/sindresorhus/awesome/d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg)

The following is a quote from [wikipedia](https://en.wikipedia.org/wiki/Rootkit).

> A rootkit is a collection of computer software, typically malicious, designed to enable access to a computer or an area of its software that is not otherwise allowed (for example, to an unauthorized user) and often masks its existence or the existence of other software. The term rootkit is a compound of "root" (the traditional name of the privileged account on Unix-like operating systems) and the word "kit" (which refers to the software components that implement the tool). The term "rootkit" has negative connotations through its association with malware.

Linux rookit has been published a lot on GitHub. This page is a summary of them.

## LD_PRELOAD rootkit

- [mempodippy/vlany: Linux LD_PRELOAD rootkit (x86 and x86_64 architectures)](https://github.com/mempodippy/vlany)
- [chokepoint/azazel: Azazel is a userland rootkit based off of the original LD_PRELOAD technique from Jynx rootkit. It is more robust and has additional features, and focuses heavily around anti-debugging and anti-detection.](https://github.com/chokepoint/azazel)
- [chokepoint/jynxkit: JynxKit is an LD_PRELOAD userland rootkit for Linux systems with reverse connection SSL backdoor](https://github.com/chokepoint/jynxkit)
- [chokepoint/Jynx2: JynxKit2 is an LD_PRELOAD userland rootkit based on the original JynxKit. The backdoor has been replaced with an "accept()" system hook.](https://github.com/chokepoint/Jynx2)
- [ChristianPapathanasiou/apache-rootkit: A malicious Apache module with rootkit functionality](https://github.com/ChristianPapathanasiou/apache-rootkit)
- [unix-thrust/beurk: BEURK is an userland preload rootkit for GNU/Linux, heavily focused around anti-debugging and anti-detection.](https://github.com/unix-thrust/beurk)

## Kernel Module rootkit

- [mncoppola/suterusu: An LKM rootkit targeting Linux 2.6/3.x on x86(_64), and ARM](https://github.com/mncoppola/suterusu)
- [m0nad/Diamorphine: LKM rootkit for Linux Kernels 2.6.x/3.x/4.x](https://github.com/m0nad/Diamorphine)
- [nurupo/rootkit: Linux rootkit for Ubuntu 16.04 and 10.04 (Linux Kernels 4.4.0 and 2.6.32), both i386 and amd64](https://github.com/nurupo/rootkit)
- [QuokkaLight/rkduck: Linux v4.x.x Rootkit](https://github.com/QuokkaLight/rkduck)
- [David-Reguera-Garcia-Dreg/enyelkm: LKM rootkit for Linux x86 with the 2.6 kernel. It inserts salts inside system_call and sysenter_entry.](https://github.com/David-Reguera-Garcia-Dreg/enyelkm)
- [trimpsyw/adore-ng: linux rootkit adapted for 2.6 and 3.x](https://github.com/trimpsyw/adore-ng)

## Ramdisk rootkit

- [r00tkillah/HORSEPILL: HORSEPILL rootkit PoC](https://github.com/r00tkillah/HORSEPILL)
- [us-16-Leibowitz-Horse-Pill-A-New-Type-Of-Linux-Rootkit.pdf](https://www.blackhat.com/docs/us-16/materials/us-16-Leibowitz-Horse-Pill-A-New-Type-Of-Linux-Rootkit.pdf)

## Rootkit checker
- [chkrootkit -- locally checks for signs of a rootkit](http://www.chkrootkit.org/)
- [The Rootkit Hunter project](http://rkhunter.sourceforge.net/)
- [ossec/ossec-hids: OSSEC is an Open Source Host-based Intrusion Detection System that performs log analysis, file integrity checking, policy monitoring, rootkit detection, real-time alerting and active response.](https://github.com/ossec/ossec-hids)

## Materials
- [Malware Memory Analysis of the Jynx2 Linux Rootkit (Part 1): Investigating a Publicly Available Linux Rootkit Using the Volatility Memory Analysis Framework](https://apps.dtic.mil/sti/citations/AD1004190)
- [SANS Institute: Rootkit Detection with OSSEC](https://www.sans.org/reading-room/whitepapers/detection/paper/34555)
- [The Horse Pill Rootkit vs. Forcepoint Threat Protection for Linux | Forcepoint](https://www.forcepoint.com/blog/x-labs/horse-pill-rootkit-vs-forcepoint-threat-protection-linux)
- [The magic of LD_PRELOAD for Userland Rootkits | FlUxIuS' Blog](http://fluxius.handgrep.se/2011/10/31/the-magic-of-ld_preload-for-userland-rootkits/)
- [Linux Rootkit Internals - Speaker Deck](https://speakerdeck.com/tkmru/linux-rootkit-internals)
