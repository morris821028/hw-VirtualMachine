build:
	tar -xf material/qemu_virtual_machine.tar.bz2
	mkdir -p build.qemu
	cd build.qemu && \
		../qemu-0.13.0/configure --target-list=i386-linux-user && \
		make
clean:
	rm -rf qemu-0.13.0 build.qemu
