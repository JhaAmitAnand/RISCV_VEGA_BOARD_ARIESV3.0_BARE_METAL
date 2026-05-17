: << 'MULTI_LINE_COMMENT_DELIMETER'
This is a multi-line comment block.

 * File Name   : build.sh
 * File Type   : Shebang Script (Shell Script)
 * File Author : Amit Anand Jha
 
 * File Usage  : This is to automate the building of the entire project
 * to generate the .o files and then finally get the ELF file and from 
 * there get the .bin file. This is a very raw level automation where almost
 * everything is harcoded to give an idea of how and what commands to run.
 * This file can be turned to more better automation script by using for loops
 * and other such stuff - which requires syntax/keyword knowledge of shell.
 * We leave it for the users curiosity to further optimize automation  

The shell will ignore everything until it hits the matching delimiter - which 
was used to start the Multi-line comment

MULTI_LINE_COMMENT_DELIMETER

#! /bin/bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
echo -e "\n\nDirectory where build.sh is located: $SCRIPT_DIR"


# Harrcoded stuff for low-level (very basic) automation
# Think of using loop to avoid these and automate like a pro
# This script is just to give a beginning idea on Shell scripting
INCLUDE_PATHS="-I ./64_bits_maths/ -I ./hw_drivers/"
SOURCE_FILES="hw_drivers.c 64_bits_maths.c main.c"
OBJECT_FILES="hw_drivers.o 64_bits_maths.o main.o"


# Exit immediately if any command returns a non-zero exit status
set -e


# STAGE 1: Assemble
ASSEMBLE_CMD="riscv-none-elf-gcc -c -march=rv32im -mabi=ilp32 startup.S -o startup.o"
echo -e "\n\nRunning STAGE1 ASSEMBLING command $ASSEMBLE_CMD"
$ASSEMBLE_CMD


# STAGE 2: COMPILE HEADERS and SOURCE 
COMPILE_SOURCE_CMD1="riscv-none-elf-gcc -c -march=rv32im -mabi=ilp32 $INCLUDE_PATHS -ffreestanding -O0 ./hw_drivers/hw_drivers.c -o hw_drivers.o"
COMPILE_SOURCE_CMD2="riscv-none-elf-gcc -c -march=rv32im -mabi=ilp32 $INCLUDE_PATHS -ffreestanding -O0 ./64_bits_maths/64_bits_maths.c -o 64_bits_maths.o"
COMPILE_SOURCE_CMD3="riscv-none-elf-gcc -c -march=rv32im -mabi=ilp32 $INCLUDE_PATHS -ffreestanding -O0 main.c -o main.o"
echo -e "\n\nRunning STAGE2 COMPILING command $COMPILE_SOURCE_CMD1"
$COMPILE_SOURCE_CMD1
echo -e "\n\nRunning STAGE2 COMPILING command $COMPILE_SOURCE_CMD2"
$COMPILE_SOURCE_CMD2
echo -e "\n\nRunning STAGE2 COMPILING command $COMPILE_SOURCE_CMD3"
$COMPILE_SOURCE_CMD3


# STAGE 3: LINKING
ELF_FILE_NAME="firmware.elf"
LINKER_CMD="riscv-none-elf-gcc -march=rv32im -mabi=ilp32 -O0 -ffreestanding -mno-relax -nostartfiles -nodefaultlibs -nostdlib -T linker.ld startup.o $OBJECT_FILES -o $ELF_FILE_NAME -lgcc"
echo -e "\n\nRunning STAGE3 LINKING command $LINKER_CMD"
$LINKER_CMD

# STAGE 4: FINAL OBJECT (BIN) FILE STICHING
BIN_FILE_NAME="firmware.bin"
BIN_CREATE_CMD="riscv-none-elf-objcopy -O binary -S $ELF_FILE_NAME $BIN_FILE_NAME"
echo -e "\n\nRunning STAGE4 OBJECT FILE CREATION command $BIN_CREATE_CMD"
$BIN_CREATE_CMD
