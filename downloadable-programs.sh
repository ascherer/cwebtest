#!/bin/sh
REMOTE=http://www-cs-faculty.stanford.edu/~uno/programs

for FILE in sham.w obdd.w.gz reflect.w.gz reflect.ch.gz hull.w hulls.w hullt.w \
	hulltr.w hulld-ngon.ch hulld-square.ch hulld-unif.ch tcalc.w.gz \
	decagon.w.gz decagon-star.ch.gz decagon-color.ch.gz \
	decagon-colorstar.ch.gz antislide.w.gz antislide-nocorner.ch.gz \
	antislide3.w.gz setset.w setset-all.w setset-random.w sliding.w \
	straighten.w domination.w fog2mf.w lagfib.w garsia-wachs.w \
	halftone.w dot-diff.w togpap.w dance.w polyominoes.w \
	polyiamonds.w polysticks.w queens.w sudoku.w dance-random.ch \
	gdance.w macmahon-triangles-sym-tile.w xgdance.w gdance-cutoff.ch \
	hamdance.w ham.w polynum.w polyslave.w polynum-restart.ch polyenum.w \
	polyslave-restart.ch advent.w.gz rost.w ran-prim.w strongchain.w \
	koda-ruskey.w li-ruskey.w spiders.w topswops.w topswops-fwd.w \
	co-debruijn.w posets0.w posets.w erection.w unavoidable.w \
	unavoidable2.w grayspan.w spspan.w grayspspan.w spgraph.w \
	sand.w zeilberger.w francon.w viennot.w linked-trees.w vacillate.w \
	embed.w lp.w horn-count.w 15puzzle-korf0.w 15puzzle-korf1.w \
	achain0.w achain1.w achain2.w achain3.w achain4.w achain-all.w \
	achain-decode.w hyperbolic.w fchains4x.w fchains4x-dontcares.ch \
	tictactoe.tgz prime-sieve.w prime-sieve-sparse.w prime-sieve-boot.ch \
	maxcliques.w ulam.w ulam-longlong.ch hwb-fast.w yplay.w bdd12.w \
	bdd14.w bddl-rgrowth.w bdd15.w simpath.w simpath-reduce.w \
	simpath-example.tgz simpath-cycles.ch simpath-ham.ch simpath-ham-any.ch \
	simpath-directed-cycles.w euler-trail.w celtic-paths.w dragon-calc.w \
	squaregraph.w squaregraph-rand.w treeprobs.w graph-sig-v0.w \
	skew-ternary-calc.w random-ternary.w random-ternary-quads.ch \
	skew-ternary-calc-raw.ch dimacs-to-sat.w sat-to-dimacs.w \
	sat0.w sat0w.w sat8.w sat9.w sat10.w sat11.w sat11k.ch sat12.w \
	sat12-erp.w sat13.w sat-life.w back-20q.w \
	back-20q-backmod9,15.ch back-20q-backmod9,15-indet.ch \
	back-mxn-words-new.w back-mxn-words-mxn.w back-pdi.w \
	commafree-eastman.w commafree-eastman-new.w sat-commafree.w \
	back-commafree4.w back-skeleton.w back-dissect.w \
	ulam-gibbs.w ulam-gibbs.1 dlx1.w dlx2.w dlx-pre.w dlx2-polyom.ch \
	dlx2-wordsearch.ch dlx2-sharp.ch dlx2-cutoff.ch dlx2-kludge.ch \
	dlx2-cutoff-kludge.ch dlx3.w dlx3-sharp.ch dlx3-sharp-wordcross.ch \
	dlx3-redrect.ch dlx3-motley.ch
do
	echo "cURLing $FILE"

	if [ "$1" = "--get" ]
	then
		curl --remote-time $REMOTE/$FILE -z $FILE -o $FILE
	else
		curl --head $REMOTE/$FILE
	fi
done
