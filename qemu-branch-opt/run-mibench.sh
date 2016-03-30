time build.qemu/i386-linux-user/qemu-i386 automotive/basicmath/basicmath_small \
	>/dev/null
time build.qemu/i386-linux-user/qemu-i386 automotive/basicmath/basicmath_large \
	>/dev/null
time build.qemu/i386-linux-user/qemu-i386 automotive/bitcount/bitcnts \
	10000000
time build.qemu/i386-linux-user/qemu-i386 automotive/qsort/qsort_small \
	automotive/qsort/input_small.dat >/dev/null
time build.qemu/i386-linux-user/qemu-i386 automotive/qsort/qsort_large \
	automotive/qsort/input_large.dat >/dev/null
time build.qemu/i386-linux-user/qemu-i386 automotive/susan/susan \
	automotive/susan/input_large.pgm /dev/null -s
