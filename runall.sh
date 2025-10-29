#!/bin/sh -e

PROG=$(basename $0)

usage ()
{
cat <<HELP
Invoke with option '-b|--branch' to override the 'testbranch'
Invoke with option '-c|--changes' to apply associated change files (if any)
Invoke with option '-e|--extras' to apply 'extra' change files
Invoke with option '-f|--files' to set the list of example sources
Invoke with option '-p|--path' to set the path to CWEB executables
Invoke with option '-t|--tex' to set the TeX engine
Invoke with option '-v|--version' to set the CWEB version
HELP
}

SHRTOPTS=b:cef:hp:t:v:
LONGOPTS=branch:,changes,extras,file:,help,path:,tex:,version:

# Set default values
branch= # Executive override for 'testbranch' below
changes=false # Apply associated change files (if any)
extras=false # Apply 'extra' change files off-scheme (inludes 'changes')
files=$(echo *.w) # List of example sources
path='/usr/bin' # Path to CWEB
tex= # TeX engine
version='3.64' # Version of CWEB

getopt -T >/dev/null && true

if [ $? -eq 4 ] # Check for Linux-getopt with extensions
then OPTS=$(getopt -n runall -o $SHRTOPTS -l $LONGOPTS -- "$@")
else OPTS=$(getopt $SHRTOPTS $*)
fi

if [ $? -eq 0 ] # Check return code from getopt
then eval set -- "$OPTS"
else
	>&2 echo "Failed to parse options."
	exit 1
fi

while true
do
	case "$1" in
		-b | --branch ) branch="$2"; shift 2 ;;
		-c | --changes ) changes=true; shift ;;
		-e | --extras ) extras=true; changes=true; shift ;;
		-f | --file ) files="$2" ; shift 2 ;;
		-h | --help ) usage; exit 0 ;;
		-p | --path ) path="$2"; shift 2 ;;
		-t | --tex ) tex="$2"; shift 2 ;;
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

# If we're running TeX, we'll need some Metapost graphics
if [ -n "$tex" ]
then
	for f in *.mp
	do
		mpost $f
	done
fi

# Take changes into account
testbranch="runall-$path-V$version"
[ $branch ] && testbranch=$branch # Executive override
$changes && testbranch="$testbranch-changes"
git checkout -b $testbranch || exit 1 # Avoid 'master' desaster

# Process all CWEB main sources
for i in $files
do
	# Ignore SGB @include file
	[ "$i" != "gb_types.w" ] || continue

	# CTANGLE
	bi=$(basename $i .w)
	$path/ctangle +bph $i
	git add $bi.c && \
	>&2 git commit -m "ctangle [$version] $i."

	# CWEAVE
	$path/cweave +bph $i
	git add $bi.idx $bi.scn $bi.tex && \
	>&2 git commit -m "cweave [$version] $i."
	[ -n "$tex" ] && $tex $bi

	# Process all associated CWEB change files ...
	$changes || continue
	for c in $bi*.ch
	do
		[ -f $c ] || continue # ... if any
		bc=$(basename $c .ch)

		# CTANGLE
		$path/ctangle +bph $i $c $bc
		git add $bc.c && \
		>&2 git commit -m "ctangle [$version] $i $c."

		if [ ham = $bc ]
		then
			# CTWILL
			$path/ctwill +bph $i $c $bc
			$path/ctwill +bph $i $c $bc
			[ -n "$tex" ] && $tex $bc
			ctwill-refsort < $bc.ref > $bc.sref
			git add $bc.idx $bc.ref $bc.sref $bc.tex && \
			>&2 git commit -m "ctwill [$version] $i $c."
		else
			# CWEAVE
			$path/cweave +bph $i $c $bc
			git add $bc.idx $bc.scn $bc.tex && \
			>&2 git commit -m "cweave [$version] $i $c."
		fi
		[ -n "$tex" ] && $tex $bc
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
			bc=$(basename $c .ch)
			$path/ctangle +bph hull$i $bc $bc-$i
			git add $bc-$i.c && \
			>&2 git commit -m "ctangle [$version] hull$i $c."

			# CWEAVE
			$path/cweave +bph hull$i $bc $bc-$i
			git add $bc-$i.idx $bc-$i.scn $bc-$i.tex && \
			>&2 git commit -m "cweave [$version] hull$i $c."
			[ -n "$tex" ] && $tex $bc-$i
		done
	done

	$path/ctangle +bph horn-count krom-count krom-count
	git add krom-count.c && \
	>&2 git commit -m "ctangle [$version] horn-count krom-count."

	$path/cweave +bph horn-count krom-count krom-count
	git add krom-count.idx krom-count.scn krom-count.tex && \
	>&2 git commit -m "cweave [$version] horn-count krom-count."
	[ -n "$tex" ] && $tex krom-count

	$path/ctangle +bph ssxcc2 ssxcc3 ssxcc3
	git add ssxcc3.c && \
	>&2 git commit -m "ctangle [$version] ssxcc2 ssxcc3."

	$path/cweave +bph ssxcc2 ssxcc3 ssxcc3
	git add ssxcc3.idx ssxcc3.scn ssxcc3.tex && \
	>&2 git commit -m "cweave [$version] ssxcc2 ssxcc3."
	[ -n "$tex" ] && $tex ssxcc3

	$path/ctangle +bph xccdc2 xccdc3 xccdc3
	git add xccdc3.c && \
	>&2 git commit -m "ctangle [$version] xccdc2 xccdc3."

	$path/cweave +bph xccdc2 xccdc3 xccdc3
	git add xccdc3.idx xccdc3.scn xccdc3.tex && \
	>&2 git commit -m "cweave [$version] xccdc2 xccdc3."
	[ -n "$tex" ] && $tex xccdc3
fi

git checkout master

exit 0

# vim: set noexpandtab:
