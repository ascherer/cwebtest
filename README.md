# testbed for cweb

the purpose of this project is quality ensurance for variants of cweb.

* [original cweb](http://www-cs-faculty.stanford.edu/~uno/cweb.html) is the
  canonical version of this software system. on unix extract the 'latest'
  source tarball and invoke `make`. or use [cweb on
  github](https://github.com/ascherer/cweb).
* [ansi compatible version](https://github.com/ascherer/cwebbin/tree/cweb-ansi)
  is almost the same as the original, minus tons of compiler warnings. clone
  [cwebbin](https://github.com/ascherer/cwebbin), check out branch
  **cweb-ansi**, add the original source drop, and compile with
  `make -f Makefile.unix boot cautiously all`.
* [texlive cweb](https://tug.org/svn/texlive/trunk/Build/source/texk/web2c/cwebdir/)
  is shipped with 'TeX Live'; I have not compiled this version myself.
* [extended version](https://github.com/ascherer/cwebbin) is a modified
  version of cweb aimed mostly at portability. it comes with a few extensions,
  but these are 'off' by default.
* [hypertex version](https://github.com/ascherer/cwebbin/tree/hyper-sync-tex)
  is the latest extension of cweb/cwebbin. it extends **cweave** with the
  **r** option; switched 'on' with **+r**, cweave adds back-references to the
  TeX output that allow dvi/pdf viewers to jump back to the cweb code. this
  also affects 'cwebmac.tex', which is not tested in this project (yet).

## major applications

cweb is a major representative of the [literate
programming](http://www-cs-faculty.stanford.edu/~uno/lp.html) paradigm. its
creator, [donald ervin
knuth](http://www-cs-faculty.stanford.edu/~uno/index.html), used it mainly for
two large sets of well-documented software systems:

* [stanford graphbase](http://www-cs-staff.stanford.edu/~knuth/sgb.html)
* [mmix](http://www-cs-faculty.stanford.edu/~knuth/mmix.html)

## individual example programmes

in addition, don knuth uses cweb for many more smaller programs that are
used as exemplary implementations of algorithms, mostly from
[TAOCP](http://www-cs-faculty.stanford.edu/%7Euno/taocp.html).

* [tons of cweb code](http://www-cs-faculty.stanford.edu/~uno/programs.html)

cweb itself is 'frozen' (unless catastrophic bugs are found), but developers
are encouraged to work with the sources and improve or extend them. however,
don knuth requests that such modified versions of cweb "don't screw up
[_these example programmes_] any more than they would screw up today!"

# how to use this project

start with `git clone --mirror https://github.com/ascherer/cwebtest`. the
default branch **master** is only used to set up the environment. much more
interesting are the auxiliary branches for each of the cweb variants (see
`git branch` for a list). individual commits on these branches show the results
of applying **ctangle** and **cweave** on **all** `.w` sources. to see the
differences just compare the heads of the branches, e.g., `git diff plain-cweb
cwebbin` (do this on your local clone; Github can't show such large 'diffs').
