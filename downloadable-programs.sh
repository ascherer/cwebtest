#!/bin/sh
# Invoke with option '--get' to actually download files
GET=${1:--no-get}

# Generic download function
get_file () # path/to/file
{
	FILE=`basename $1`
	REMOTE=`dirname $1`

	echo "cURLing $REMOTE/$FILE"

	if [ "$GET" = "--get" ]
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
	get_file http://www-cs-faculty.stanford.edu/~uno/programs/$FILE
done
