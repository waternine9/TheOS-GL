mkdir -p bin
<<<<<<< HEAD
g++ -c -m32 src/*.cpp src/gl/*.c -fno-rtti -nostdlib -ffreestanding -mno-red-zone -fno-exceptions -nodefaultlibs -fno-builtin -fno-pic -O0
=======
g++ -c -m32 src/*.cpp -Wall -Werror -fno-rtti -nostdlib -ffreestanding -mno-red-zone -fno-exceptions -nodefaultlibs -fno-builtin -fno-pic -O0
>>>>>>> 906d37524159d95c3d0557e1759f58181caea97e
nasm -f elf32 src/bootloader/boot.asm -o boot.o
ld -m elf_i386 *.o -T link.ld -o bin/boot.img
rm *.o
qemu-img resize bin/boot.img 32M

