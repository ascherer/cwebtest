# testbed for cweb

the purpose of this project is quality ensurance for variants of cweb.

* [original cweb](http://www-cs-faculty.stanford.edu/~uno/cweb.html) is the
  canonical version of this software system. on unix extract the 'latest'
  source tarball and invoke `make`. or use [cweb on
  github](https://github.com/ascherer/cweb).
* [texlive cweb](https://tug.org/svn/texlive/trunk/Build/source/texk/web2c/cwebdir/)
  is shipped with 'TeX Live'; I have not compiled this version myself.
* [extended version](https://github.com/ascherer/cwebbin) is a modified
  version of cweb aimed mostly at portability. it comes with a few extensions,
  but these are 'off' by default.
* [ansi compatible version](https://github.com/ascherer/cwebbin)
  is almost the same as the original, minus tons of compiler warnings.
  the former special branch **cweb-ansi** is deprecated and should no longer
  be used. instead, use the `cwebbin.spec` file and run
  `debbuild -ba --with-ansi SPECS/cwebbin.spec` or
  `rpmbuild -ba --with-ansi SPECS/cwebbin.spec` depending on your (GNU/Linux)
  system. either command will run `make -f Makefile.unix boot cautiously all`
  after modifying the `Makefile` so that only the minimal `ansi` changefiles
  are applied to the sources. (you can apply these modifications to `Makefile`
  manually with the `sed` command in `cwebbin.spec`.)
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

start with `git clone https://github.com/ascherer/cwebtest`. the
default branch **master** is used to set up the environment by extracting the
full set of knuthian example programmes.

now invoke `./runall.sh -p <PATH TO YOUR CWEB> -v <VERSION OF YOUR CWEB>`.
this will create a new branch with this commandline as its name
by applying **ctangle** and **cweave** on **all** `.w` sources
(ca. 250 commits at the time of writing). to see the
differences just compare the heads of such branches.

if you care about modified versions of some of the example programs, you can
add the `-c` option to the `./runall.sh` commandline, which applies (most of)
the changefiles available and adds `-changes` to the created test branch name.
