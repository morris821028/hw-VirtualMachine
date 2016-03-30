build:
	tar -xf material/qemu_virtual_machine.tar.bz2
	mkdir -p build.qemu
	cd build.qemu && \
		../qemu-0.13.0/configure --target-list=i386-linux-user && \
		make
rebuild:
	cp source/optimization.c qemu-0.13.0/
	cp source/optimization.h qemu-0.13.0/
	cp source/cpu-defs.h qemu-0.13.0/
	make -C build.qemu
buildtest:
	make -C test all
extract-benchmark:
	tar -xf  material/automotive.tar.gz
	tar zxvf material/coremark_v1.0.tgz
clean:
	rm -rf qemu-0.13.0 build.qemu
	make -C test clean
