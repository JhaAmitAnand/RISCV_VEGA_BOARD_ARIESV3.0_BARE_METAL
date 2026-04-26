To create the .ELF files or the .BIN files from the .header, .c and .S files execute the 
below commands in the Bitbash Window. But to do that, we would need cross compilers installed

To install the cross compiler - read the README in the direcotry "Executables_Post_Linking_and_Assembling"

Once the Cross Compiler is installed and sourced, run the below commands.

# 1. Assemble
riscv-none-elf-gcc -c -march=rv32im -mabi=ilp32 startup.S -o startup.o

# 2. Compile (Added -fno-stack-protector to prevent hidden library calls)
riscv-none-elf-gcc -c -march=rv32im -mabi=ilp32 -ffreestanding -O0 main.c -o main.o

# 3. Link (Removed the flags causing the error)
riscv-none-elf-ld -T linker.ld --undefined=_start startup.o main.o -o firmware.elf

# 4. Create Binary
riscv-none-elf-objcopy -O binary -S firmware.elf firmware.bin


Above could be very well a part of a python or a Bash script (automated), but since this is the 
beginner level stuff here, I wanted to capture step by step detail of each of the process like
Linking, assembling etc.


We can now check the size and start and end addresses of each sections from the linker
# 5. Checks:
riscv-none-elf-objdump -h firmware.elf  
riscv-none-elf-objdump -d firmware.elf | head -n 20

NOTE: Here in the linker.ld we dont have a separate FLASH (Non-volatile) and RAM (Volatile) regions.
This is designed so that the code on the device where the code is flashed, is removed from  the chip once power is lost.

If we were to write code to be "burned" onto the chip permanently using a JTAG programmer, then our startup.S would also 
need to be much more complex to handle the migration of the .data section from Flash to RAM. 

We leave all those complex things for future endeavors.

How to Flash the .BIN finally generated is something we will find in the README inside the direcotry "Executables_Post_Linking_and_Assembling"
