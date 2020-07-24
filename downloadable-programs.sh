#!/bin/sh

PROG=$(basename $0)

usage ()
{
cat <<HELP
Invoke with option '-t|--debug' to check the 'curl' invocations
Invoke with option '-d|--get' to actually download files
Invoke with option '-f <FILE>|--file <FILE>' to handle singular file(s)
Invoke with option '-r <URL>|--remote <URL>' to change server address
HELP
}

SHRTOPTS=df:hr:t
LONGOPTS=debug,file:,get,help,remote:

# Set default values
FILES=$(cat downloadable-programs.lst)
DEBUG=false
GET=false
REMOTE='https://www-cs-faculty.stanford.edu/~knuth/programs'

getopt -T >/dev/null

if [ $? -eq 4 ] # Check for Linux-getopt with extensions
then OPTS=$(getopt -n $PROG -o $SHRTOPTS -l $LONGOPTS -- "$@")
else OPTS=$(getopt $SHRTOPTS $*)
fi

if [ $? -eq 0 ] # Check return code from getopt
then eval set -- "$OPTS"
else echo "Failed to parse options." >&2; exit 1; fi

while true
do
	case "$1" in
		-d | --get ) GET=true; shift ;;
		-f | --file ) FILES="$2"; shift 2 ;;
		-h | --help ) usage; exit 0 ;;
		-r | --remote ) REMOTE="$2"; shift 2 ;;
		-t | --debug ) DEBUG=true; shift ;;
		-- ) shift; break ;;
		* ) break ;;
	esac
done

# Generic download function
get_file () # path/to/file
{
	FILE=$(basename $1)
	DIR=$(dirname $1)

	$DEBUG || echo "cURLing $DIR/$FILE"

	if $GET
	then
		if [ -f $FILE ]
		then CURL="curl -k -R $DIR/$FILE -z $FILE -o $FILE"
		else CURL="curl -k -R $DIR/$FILE -o $FILE"
		fi
	else
		CURL="curl -k -I $DIR/$FILE"
	fi

	if $DEBUG
	then
		echo $CURL
	else
		$CURL
	fi
}

# Process extended list of files in 'programs/' directory
for FILE in $FILES
do
	get_file "$REMOTE/$FILE"
done

exit 0
