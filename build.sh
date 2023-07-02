mkdir -p bin
python3 build_resources.py
g++ -c -m32 src/*.cpp src/gl/*.c src/drivers/*/*.cpp src/utils/*.cpp src/applications/*.cpp -fno-rtti -nostdlib -ffreestanding -mno-red-zone -fno-exceptions -nodefaultlibs -fno-builtin -fno-pic -msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -O0
nasm -f elf32 src/bootloader/boot.asm -o boot.o
ld -m elf_i386 *.o -T link.ld -o bin/boot.img
rm *.o

