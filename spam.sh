#!/bin/bash
echo "r $1"
sleep 2
for (( i=10; i>0; i-- ))
do
	echo "$2 $i"
	sleep 0.01
done
