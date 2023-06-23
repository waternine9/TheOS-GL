mkdir -p bin
g++ -c -m32 src/*.cpp src/gl/*.c -fno-rtti -nostdlib -ffreestanding -mno-red-zone -fno-exceptions -nodefaultlibs -fno-builtin -fno-pic -O3
nasm -f elf32 src/bootloader/boot.asm -o boot.o
ld -m elf_i386 *.o -T link.ld -o bin/boot.img
rm *.o
qemu-img resize bin/boot.img 32M

