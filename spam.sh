#!/bin/bash
echo "r $1"
sleep 4
for (( i=1000; i>0; i-- ))
do
	echo "$2 $i"
	sleep 0.3
done
