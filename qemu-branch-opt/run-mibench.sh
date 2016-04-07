#!/bin/bash

test=("basicmath/basicmath_small" \
		"basicmath/basicmath_large" \
		"bitcount/bitcnts 10000000" \
		"qsort/qsort_small automotive/qsort/input_small.dat" \
		"qsort/qsort_large automotive/qsort/input_large.dat" \
		"susan/susan automotive/susan/input_large.pgm /dev/null -s")
for i in "${test[@]}";
do
	echo -e "\033[35m Test ${i} Qemu \033[0m";
	time build.qemu/i386-linux-user/qemu-i386 automotive/${i} >/dev/null;
done
