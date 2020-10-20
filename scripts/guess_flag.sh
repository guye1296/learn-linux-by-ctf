#!/bin/bash


CHALLENGE=$(/usr/bin/_get-next-challenge)

if [ $# -gt 3 ]; then
	echo USAGE: $0 [CHALLENGE]
fi

if [ $# -eq 1 ]; then
	CHALLENGE=$1
fi

exec /usr/bin/_guess-flag $CHALLENGE
