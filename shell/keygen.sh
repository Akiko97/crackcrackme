#!/bin/bash

function usage {
	echo "Usage: "
	echo "	    check: $0 chk name key"
	echo "	generator: $0 gen name"
}

function check {
	echo "Name: $1"
	echo "Key: $2"
	key07_o=`echo $2 | awk '{print substr($0,1,8)}'`
	key8F_o=`echo $2 | awk '{print substr($0,9,8)}'`
	key_tail_o=`echo $2 | awk '{print substr($0,17,4)}'`
	key07=`md5 -s "$1" | awk '{print $4}' | awk '{print substr($0,1,8)}' | tr 'a-z' 'A-Z'`
	if [[ $key07 != $key07_o ]]; then
		echo "****** WRONG KEY! ******"
		exit 0;
	fi
	subkey=${key07_o}${key8F_o}
	key_tail=`md5 -s "$subkey" | awk '{print $4}' | awk '{print substr($0,4,4)}' | tr 'a-z' 'A-Z'`
	if [[ $key_tail != $key_tail_o ]]; then
		echo "****** WRONG KEY! ******"
		exit 0;
	fi
	tmp=$(( 0x$key8F_o ^ 0x$key07_o ))
	xor=`echo "obase=16; $tmp" | bc`
	if [[ $xor != "C30F5E81" ]]; then
		echo "****** WRONG KEY! ******"
		exit 0;
	fi
	echo "****** RIGHT KEY! ******"
}

function generator {
	echo "Name: $1"
	key07=`md5 -s "$1" | awk '{print $4}' | awk '{print substr($0,1,8)}' | tr 'a-z' 'A-Z'`
	xor=$(( 0xC30F5E81 ^ 0x$key07 ))
	key8F=`echo "obase=16; $xor" | bc`
	subkey=${key07}${key8F}
	key_tail=`md5 -s "$subkey" | awk '{print $4}' | awk '{print substr($0,4,4)}' | tr 'a-z' 'A-Z'`
	key=${key07}${key8F}${key_tail}
	echo "Key: $key"
}

if [ -n "$1" ]; then
	case "$1" in
		chk ) check $2 $3
			;;
		gen ) generator $2
			;;
		* ) usage
			;;
	esac
else
	usage
fi