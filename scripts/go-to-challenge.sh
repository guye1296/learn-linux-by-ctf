#!/bin/bash

echo "Enter Flag:"
read FLAG

# get next user
NEXT_USER=$(get-next-challenge)

# see if flag is valid
su - $NEXT_USER -c true <<< $FLAG > /dev/null 2>&1

if [ $? -ne 0 ]; then
    >&2 "Wrong flag"
    exit 1
fi

echo "You got the right flag! Moving to the next challenge..."

exec su -l $NEXT_USER -c "exec cat < /tmp/foo > /tmp/bar" <<< $FLAG > /dev/null 2>&1 