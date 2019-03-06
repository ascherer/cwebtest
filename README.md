# testbed for cweb

the purpose of this project is quality ensurance for variants of cweb.

## major applications

cweb is a major representative of the [literate
programming](http://www-cs-faculty.stanford.edu/~knuth/lp.html) paradigm. its
creator, [donald ervin
knuth](http://www-cs-faculty.stanford.edu/~knuth/index.html), used it mainly for
two large sets of well-documented software systems:

* [stanford graphbase](http://www-cs-staff.stanford.edu/~knuth/sgb.html)
* [mmix](http://www-cs-faculty.stanford.edu/~knuth/mmix.html)

## individual example programs

in addition, don knuth uses cweb for many more smaller programs that are
used as exemplary implementations of algorithms, mostly from
[TAOCP](http://www-cs-faculty.stanford.edu/~knuth/taocp.html).

* [tons of cweb code](http://www-cs-faculty.stanford.edu/~knuth/programs.html)

cweb itself is 'frozen' (unless catastrophic bugs are found), but developers
are encouraged to work with the sources and improve or extend them. however,
don knuth requests that such modified versions of cweb "don't screw up
[_these example programs_] any more than they would screw up today!"

and some of these example programs are "really OLD indeed. [...] they should be
read but not executed!"

# how to use this project

start with `git clone https://github.com/ascherer/cwebtest`. the default branch
**master** is used to set up the environment by extracting the full set of
knuthian example programs. if you want to cross-check with stanford, use the
local `./downloadable-programs.sh --get` and `gzip -d` on all gzipped master
sources.

now invoke `./runall.sh -p <PATH TO YOUR CWEB> -v <VERSION OF YOUR CWEB>`.
this will create a new branch with this commandline as its name by applying
**ctangle** and **cweave** on **all** `.w` sources (ca. 250 commits at the time
of writing). to see the differences just compare the heads of such branches.

if you care about modified versions of some of the example programs, you can
add the `-c` option to the `./runall.sh` commandline, which applies (most of)
the changefiles available and adds `-changes` to the created test branch name.
there is an accompanying option `-e` that applies some "extra" change files
(and activates `-c` as well).

if you want to run tests with a subset of one (on systems with "classic"
`getopt`) or more (on systems with enhanced linux-`getopt`) cweb source files,
you can use option `-f` with a single file name or a stringified list of
several file names.
