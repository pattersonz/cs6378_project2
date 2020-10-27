#!/bin/bash
# Change this to your netid
netid=zjp170130

# Root directory of your project
PROJDIR=/people/cs/z/zjp170130/cs6378_project2

# Directory where the config file is located on your local system
CONFIGLOCAL=config.txt

# Directory your java classes are in
BINDIR=$PROJDIR/build/unit/

# Your main project class
PROG=unit

n=0

cat $CONFIGLOCAL | sed -e "s/#.*//" | sed -e "/^\s*$/d" |
(
    read x
    echo $x
	xs=($x)
	i="${xs[0]}"
    while [[ $n -lt $i ]]
    do
    	read line
    	p=$( echo $line | awk '{ print $1 }' )
        host=$( echo $line | awk '{ print $2 }' )
		
		if [ "$#" = "1" ] && [ "$1" = "gdb" ]
		then
			gnome-terminal -e "ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no $netid@$host echo $BINDIR$PROG; gdb -ex run $BINDIR$PROG; exec bash" &
			n=$(( n + 1 ))
		else
			gnome-terminal -e "ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no $netid@$host echo $BINDIR$PROG; $BINDIR$PROG; exec bash" &
			n=$(( n + 1 ))
		fi
    done
)
