#!/bin/sh

path=.
version=

while getopts p:v: opt
do
	case $opt in
		p) path=$OPTARG ;;
		v) version=$OPTARG ;;
		?) exit 1 ;;
	esac
done

git checkout -b "runall-$path-V$version"

for i in *.w
do
	$path/ctangle $i
	git add `basename $i .w`.c
	git commit -m "ctangle [$version] $i."

	$path/cweave $i
	git add `basename $i .w`.idx `basename $i .w`.scn `basename $i .w`.tex
	git commit -m "cweave [$version] $i."
done

git checkout master
