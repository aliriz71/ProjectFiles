#!/bin/bash
#File: lab2_3.sh

#check for arguements
if [ $# -eq 0 ]; then
	echo "No arguements provided"
	exit 1
fi

#ask for user inputs
read -p "Please enter an integer: " x

#Check if integer is within range of passed arguements
if [ $x -le $# ]; then 
	echo "The arguement at position $x is: ${!x}"
else 
	echo "No arguement has been provided at position $x."
fi

