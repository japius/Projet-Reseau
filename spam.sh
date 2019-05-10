#!/bin/bash
echo "r $1"
sleep 4
for (( i=0; i<100000; i++ ))
do
	echo "$2 $i"
	sleep 0.01
done
