#!/bin/sh
REMOTE=http://www-cs-faculty.stanford.edu/~uno/programs

for FILE in `cat downloadable-programs.lst`
do
	echo "cURLing $FILE"

	if [ "$1" = "--get" ]
	then
		if [ -f $FILE ]
		then curl --remote-time $REMOTE/$FILE -z $FILE -o $FILE
		else curl --remote-time $REMOTE/$FILE -o $FILE
		fi
	else
		curl --head $REMOTE/$FILE
	fi
done
