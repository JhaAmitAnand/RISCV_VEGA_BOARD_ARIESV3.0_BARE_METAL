If you are stuck with how to build / link / assemble the .ld, .c, .S and .h files 
to create the .ELF or .BIN, please go through below very carefully and patiently.

If you are on Windows - I recommend to work on Gitbash (Install it)

Step 1: 
One needs a tool that runs on Windows but speaks RISC-V architecture language.
Download: The xPack GNU RISC-V Embedded GCC. It’s the most stable "bare-metal" toolchain for Windows

Link to download : https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases
(Above link might have been upodated, depeding on when you are checking this)

Download the .zip for win32-x64.

Extract: Unzip it to C:\riscv_gcc.  The riscv_gcc is a user made directory, give it any name you want.

Step 2: 
Link to Git Bash: Open Git Bash and run:  (Actually after unzipping, make sure to go to the bin directory and just paste its path below)

NOTE: Its better to cut and paste the entire content inside riscv_gcc - to have smaller path length (in windows path length issues are common)

Now just verify with below bash command
echo 'export PATH=$PATH:C:\riscv_gcc\<whatever_path_it_got_unzipped_at>\bin' >> ~/.bashrc

For those unaware of Bash scripting, what each thing in above line does is below -

echo '...'       : Takes the text inside the single quotes and prepares to "say" or output it.
export PATH=XYZ  : Tells the system you want to update the PATH variable, which is the list of directories the computer searches through when you run a command.
$PATH:/c/riscv.. : This keeps your current PATH list (that's the $PATH part) and adds a new folder to the end of it (separated by a colon).
>>               : This is the append operator. It takes the output and adds it to the very end of a file without deleting what's already there.~/.bashrc.

.bashrc is a hidden script file in your home directory that runs every time you open a new terminal window.

In Plain English by running this command, you are telling your system: "Hey, every time I start a new terminal session, 
please add the folder /c/riscv-gcc/bin to my list of known program locations. 
That way, if we type riscv64-unknown-elf-gcc, OS knows where to find it."

Restart the terminal and type: riscv-none-elf-gcc --version.

Crucial: If it says "command not found," check that your path in .bashrc matches the bin folder location exactly.


Step 3: Make sure you have Tera Term software installed as well on your system
This is the software which will be used to flash the .BIN finally onto the HW board
We could very well write a python script to do so, but for a beginning that would be way too much
