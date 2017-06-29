#!/bin/sh
# Invoke with option '--get' to actually download files
GET=${1:--no-get}

# Generic download function
get_file () # --[no-]get path/to/file
{
	FILE=`basename $2`
	REMOTE=`dirname $2`

	echo "cURLing $REMOTE/$FILE"

	if [ "$1" = "--get" ]
	then
		if [ -f $FILE ]
		then curl --remote-time $REMOTE/$FILE -z $FILE -o $FILE
		else curl --remote-time $REMOTE/$FILE -o $FILE
		fi
	else
		curl --head $REMOTE/$FILE
	fi
}

# Process extended list of files in 'programs/' directory
for FILE in `cat downloadable-programs.lst`
do
	get_file $GET http://www-cs-faculty.stanford.edu/~uno/programs/$FILE
done
