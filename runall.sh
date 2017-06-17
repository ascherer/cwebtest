#!/bin/sh

path= # Path to CWEB, e.g., /usr/bin
version= # Version of CWEB, e.g., 3.64b-22p
changes=0 # Apply associated change files (if any)

while getopts chp:v: opt
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
	>&2 echo "Can't find CWEB at path '$path'."
	exit 1
fi

# Take changes into account
testbranch="runall-$path-V$version"
[ $changes -ne 0 ] && testbranch="$testbranch-changes"
git checkout -b $testbranch || exit 1 # Avoid 'master' desaster

# Process all CWEB main sources
for i in *.w
do
	# Ignore SGB @include file
	[ $i != "gb_types.w" ] || continue

	# CTANGLE
	bi=`basename $i .w`
	$path/ctangle $i
	if [ -f $bi.c ]
	then
		git add $bi.c
		>&2 git commit -m "ctangle [$version] $i."
	fi

	# CWEAVE
	$path/cweave $i
	git add $bi.idx $bi.scn $bi.tex
	>&2 git commit -m "cweave [$version] $i."

	# Process all associated CWEB change files ...
	[ $changes -ne 0 ] || continue
	for c in $bi*.ch
	do
		[ -f $c ] || continue # ... if any
		bc=`basename $c .ch`

		# CTANGLE
		$path/ctangle $i $c $bc
		git add $bc.c
		>&2 git commit -m "ctangle [$version] $i $c."

		# CWEAVE
		$path/cweave $i $c $bc
		git add $bc.idx $bc.scn $bc.tex
		>&2 git commit -m "cweave [$version] $i $c."
	done
done

# Apply 'special' changefiles that do not quite fit into the standard scheme
if [ $changes -ne 0 ]
then
	for i in s t tr
	do
		for c in hulld-*.ch
		do
			# CTANGLE
			bc=`basename $c .ch`
			$path/ctangle hull$i $bc $bc-$i
			git add $bc-$i.c
			>&2 git commit -m "ctangle [$version] hull$i $c."

			# CWEAVE
			$path/cweave hull$i $bc $bc-$i
			git add $bc-$i.idx $bc-$i.scn $bc-$i.tex
			>&2 git commit -m "cweave [$version] hull$i $c."
		done
	done

	$path/ctangle horn-count krom-count krom-count
	git add krom-count.c
	>&2 git commit -m "ctangle [$version] horn-count krom-count."

	$path/cweave horn-count krom-count krom-count
	git add krom-count.idx krom-count.scn krom-count.tex
	>&2 git commit -m "cweave [$version] horn-count krom-count."
fi

git checkout master
