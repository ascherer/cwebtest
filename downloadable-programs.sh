#!/bin/sh
# Invoke with option '-d|--get' to actually download files
# Invoke with option '-f <FILE>|--file <FILE>' to handle singular file(s)
# Invoke with option '-r <URL>|--remote <URL>' to change server address

if OPTS=$(getopt -n downloadable-programs.sh -o df:r: \
	--long file:,get,remote: -- "$@")
then eval set -- "$OPTS"
else echo "Failed to parse options." >&2; exit 1; fi

FILES=`cat downloadable-programs.lst`
GET=false
REMOTE='https://www-cs-faculty.stanford.edu/~knuth/programs'

while true
do
	case "$1" in
		-d | --get ) GET=true; shift ;;
		-f | --file ) FILES="$2"; shift 2 ;;
		-r | --remote ) REMOTE="$2"; shift 2 ;;
		-- ) shift; break ;;
		* ) break ;;
	esac
done

# Generic download function
get_file () # path/to/file
{
	FILE=`basename $1`
	DIR=`dirname $1`

	echo "cURLing $DIR/$FILE"

	if $GET
	then
		if [ -f $FILE ]
		then curl -k -R $DIR/$FILE -z $FILE -o $FILE
		else curl -k -R $DIR/$FILE -o $FILE
		fi
	else
		curl -k -I $DIR/$FILE
	fi
}

# Process extended list of files in 'programs/' directory
for FILE in $FILES
do
	get_file "$REMOTE/$FILE"
done
