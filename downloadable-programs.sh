#!/bin/sh
# Invoke with option '--get' to actually download files
GET=${1:--no-get}
STANFORD=https://www-cs-faculty.stanford.edu/~knuth/programs

# Generic download function
get_file () # path/to/file
{
	FILE=`basename $1`
	REMOTE=`dirname $1`

	echo "cURLing $REMOTE/$FILE"

	if [ "$GET" = "--get" ]
	then
		if [ -f $FILE ]
		then curl -k -R $REMOTE/$FILE -z $FILE -o $FILE
		else curl -k -R $REMOTE/$FILE -o $FILE
		fi
	else
		curl -k -I $REMOTE/$FILE
	fi
}

# Process extended list of files in 'programs/' directory
for FILE in `cat downloadable-programs.lst`
do
	get_file $STANFORD/$FILE
done
