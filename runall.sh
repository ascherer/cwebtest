#!/bin/sh

if OPTS=$(getopt -n runall.sh -o cef:p:v: \
	--long changes,extras,file:,path:,version: -- "$@")
then eval set -- "$OPTS"
else >&2 echo "Failed to parse options."; exit 1; fi

changes=false # Apply associated change files (if any)
extras=false # Apply 'extra' change files off-scheme (inludes 'changes')
files=`echo *.w` # List of example sources
path='/usr/bin' # Path to CWEB
version='3.64' # Version of CWEB

while true
do
	case "$1" in
		-c | --changes ) changes=true; shift ;;
		-e | --extras ) extras=true; changes=true; shift ;;
		-f | --file ) files="$2" ; shift 2 ;;
		-p | --path ) path="$2"; shift 2 ;;
		-v | --version ) version="$2"; shift 2 ;;
		-- ) shift; break ;;
		* ) break ;;
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
$changes && testbranch="$testbranch-changes"
git checkout -b $testbranch || exit 1 # Avoid 'master' desaster

# Process all CWEB main sources
for i in $files
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
	$changes || continue
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
if $extras
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
