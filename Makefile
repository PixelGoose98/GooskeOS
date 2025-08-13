# Build and run gooseos
all:
	gcc -m32 -fno-stack-protector -fno-builtin -ffreestanding -nostdlib -c KERNEL.c -o kernel.o
	nasm -f elf32 BOOT.S -o boot.o
	ld -m elf_i386 -T linker.ld -o kernel boot.o kernel.o
	mv kernel os/boot/kernel
	grub-mkrescue -o os.iso os/
	qemu-system-i386 os.iso
