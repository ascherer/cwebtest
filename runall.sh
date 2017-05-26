#!/bin/sh

path= # Path to CWEB, e.g., /usr/bin
version= # Version of CWEB, e.g., 3.64b-22p
changes=0 # Apply associated change files (if any)

while getopts cp:v: opt
do
	case $opt in
		c) changes=1 ;;
		p) path=$OPTARG ;;
		v) version=$OPTARG ;;
		?) exit 1 ;;
	esac
done

# Check for a useful path
if [ ! -e "$path/ctangle" ]
then
	echo "Can't find CWEB at path '$path'."
	exit 1
fi

git checkout -b "runall-$path-V$version"

# Process all CWEB main sources
for i in *.w
do
	# CTANGLE
	bi=`basename $i .w`
	$path/ctangle $i
	git add $bi.c
	git commit -m "ctangle [$version] $i."

	# CWEAVE
	$path/cweave $i
	git add $bi.idx $bi.scn $bi.tex
	git commit -m "cweave [$version] $i."

	# Process all associated CWEB change files ...
	[ $changes -ne 0 ] || continue
	for c in $bi-*.ch
	do
		[ -f $c ] || continue # ... if any
		bc=`basename $c .ch`

		# CTANGLE
		$path/ctangle $i $c $bc
		git add $bc.c
		git commit -m "ctangle [$version] $i $c."

		# CWEAVE
		$path/cweave $i $c $bc
		git add $bc.idx $bc.scn $bc.tex
		git commit -m "cweave [$version] $i $c."
	done
done

git checkout master
