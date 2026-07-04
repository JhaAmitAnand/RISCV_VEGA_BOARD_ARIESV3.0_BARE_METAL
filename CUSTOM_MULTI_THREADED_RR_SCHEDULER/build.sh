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

# ====================================================================================================================

#! /bin/bash



# Set IFS (Internal Field Separator) to only break on newlines to correctly parse paths with spaces
# But before changing the IFS, save the original IFS used by BASH for safety 
ORIGINAL_IFS="$IFS"
IFS=$'\n'


SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
echo -e "\n\n===================== Directory where build.sh is located:=====================\n"
echo -e "$SCRIPT_DIR"
echo -e "=====================================================================================================\n"

# ====================================================================================================================


echo -e "\n\n===================== Please check the Root Folder below and make sure the Project is located inside it ====================="
# The directory where the Project sits
ROOT_FOLDER="C:\Users\Amit Anand Jha\OneDrive\Documents\ARIES_BARE_METAL"
PROJECT_NAME="MULTI_THREADED_SCHEDULER"
PROJECT_ROOT="$ROOT_FOLDER/$PROJECT_NAME"


echo -e "\nProject being compiled is insde : $PROJECT_ROOT\n"
echo -e "=====================================================================================================\n"

# Exit immediately if any command returns a non-zero exit status
CHDIR_TO_PROJECT=("cd" "$PROJECT_ROOT")
echo -e "Moving to Project Directory with command : ${CHDIR_TO_PROJECT[@]}"
cd "$PROJECT_ROOT" || exit 1
set -e


# TOOLS INFORMATION FOR COMPILATION LINKING AND ELF GENERATION
COMPILE_TOOL="riscv-none-elf-gcc"

# ====================================================================================================================
#                                          STAGE 0

# Create the object files directory to keep the compiled object files
OBJECT_FILE_DIRECTORY_NAME="object_files"
mkdir -p "$OBJECT_FILE_DIRECTORY_NAME"
OBJECT_FILES=()

ASSEMBLY_FILES_DIRECTORY="asm"
ASSEMBLY_ROOT="$PROJECT_ROOT/$ASSEMBLY_FILES_DIRECTORY"

# We need flags as elements in array for each string to be treated as single individual argument 
# rather than the entire string being treated like a single argument which would be wrong and result in error
# i.e. we cant have ARCHITECTURE_FLAGS="-march=rv32im -mabi=ilp32 -ffreestanding -O0"
ARCHITECTURE_FLAGS=("-march=rv32im" "-mabi=ilp32" "-ffreestanding" "-O0")


echo -e "\n\n===================== Running STAGE 0 Object Files from Assembly Files =====================\n"

# STAGE 1: Assemble
for asm_file_name in $(find "$ASSEMBLY_ROOT" -type f \( -name "*.S" -o -name "*.s" -o -name "*.asm" \)); do
    echo -e "\n\nPath : $ASSEMBLY_ROOT \nAssembly File Found : $asm_file_name"
    
    OBJECT_FILE_NAME="${OBJECT_FILE_DIRECTORY_NAME}/$(basename "${asm_file_name%.*}.o")"

    ASSEMBLE_CMD=("$COMPILE_TOOL" "-c" "${ARCHITECTURE_FLAGS[@]}" "$asm_file_name" "-o" "$OBJECT_FILE_NAME")
    echo -e "\n\nASSEMBLE CMD : ${ASSEMBLE_CMD[@]}\n\n"
    ${ASSEMBLE_CMD[@]}
    
    echo -e "\n\nThe object file is : $OBJECT_FILE_NAME successfully created for assembly file: $asm_file_name" 
     
    OBJECT_FILES+=("$OBJECT_FILE_NAME")
    
done

echo -e "************************************* STAGE 0 DONE **************************************************\n"


# ====================================================================================================================
#                                          STAGE 1

echo -e "\n\n===================== Running STAGE 1 to fetch all the include paths =====================\n"

# Loop across directories to include the path for all headers which 
# reside inside the "inc" folder as per the Project Design itself
INCLUDE_PATHS=()


# Set the globbing off to ensure no wildcard characters (*, ? [...]) are taken for regex
# Globbing: It looks at each piece and checks if it contains a * or ?. If it does Bash
# tries to replace that string with a list of matching files in your current directory.
# If find discovers a directory named inc_v*, and we happen to have files in current folder 
# named inc_v1 and inc_v2, Bash will "expand" that directory name. Then INCLUDE_PATHS would
# contain -Iinc_v1 and -Iinc_v2 instead of the actual directory name inc_v*.
set -f


for directory_name in $(find "$PROJECT_ROOT" -type d \( -name "inc" -o -name "include" \));do
    echo -e "\n\nFound Include Path : $directory_name"
    # Append the path with the -I in the INCLUDE_PATH array
    INCLUDE_PATHS+=("-I$directory_name")    # Readable but inefficient : INCLUDE_PATHS=("${INCLUDE_PATHS[@]}" "-I$directory_name")
done


# Debug print to check that correct header paths were greped
echo -e "\n\nINCLUDE PATHS : \n${INCLUDE_PATHS[*]}\n\n"
echo -e "************************************* STAGE 1 DONE **************************************************\n"


# ====================================================================================================================


# Exit immediately if any command returns a non-zero exit status
set -e

#Loop across directories to include the path for all the source files
# which reside in the "src" folder as per the Project Design itself
SOURCE_FILES=()

echo -e "\n\n===================== Running STAGE 2 COMPILING command to generate the object files =====================\n"

for file_name in $(find "${PROJECT_ROOT[@]}" -type f \( -name "*.c" -o -name ".cpp" \));do
     echo -e "\n\nFound Source File : $file_name : compiling it to form object file"
     SOURCE_FILES+=($file_name)     # Readable but inefficient : SOURCE_FILES=("${SOURCE_FILES[@]}" "$file_name")
     
     OBJECT_FILE_NAME="${OBJECT_FILE_DIRECTORY_NAME}/$(basename "${file_name%.c*}.o")"
     
     # Cant have COMPILE_CMD="$COMPILE_TOOL -c ${ARCHITECTURE_FLAGS[@]} ${INCLUDE_PATHS[@]} $file_name -o $OBJECT_FILE_NAME"
     # We need to save as an array so that shell treats each argument individually rather than a single argument
     COMPILE_CMD=("$COMPILE_TOOL" "-c" "${ARCHITECTURE_FLAGS[@]}" "${INCLUDE_PATHS[@]}" "$file_name" "-o" "$OBJECT_FILE_NAME")
     
     echo -e "\n\nCOMPILE CMD : ${COMPILE_CMD[@]}\n\n"
     ${COMPILE_CMD[@]}
     
     echo -e "\n\nObject File: $OBJECT_FILE_NAME successfully created for source file: $file_name"
     
     OBJECT_FILES+=("$OBJECT_FILE_NAME")
done

# Debug print to check that correct header paths were greped
echo -e "\n\nSOURCE FILES which were compiled are :\n${SOURCE_FILES[*]}\n\n"
echo -e "\n\nOBJECT FILES which were created are :\n${OBJECT_FILES[*]}\n\n"

echo -e "******************************** STAGE 2 DONE *******************************************************\n"

# Restoring the default/orginal Internal Field Separator (which is space for bash)
IFS=$ORIGINAL_IFS

# Set globbing back on again
set +f


# ====================================================================================================================
#                                          STAGE 3

# Exit immediately if any command returns a non-zero exit status
set -e

echo -e "\n\n===================== Running STAGE 3 to generate the ELF FILE =====================\n"

# STAGE 3: LINKING
LINKER_FILE_DIRECTORY_NAME="linkers"
ELF_FILE_NAME="firmware.elf"
ELF_DIRECTORY="elf_and_bin"

LINKING_FLAGS=("-mno-relax" "-nostartfiles" "-nodefaultlibs" "-nostdlib")
LINKER_CMD=("$COMPILE_TOOL" "${ARCHITECTURE_FLAGS[@]}" "-O0" "-ffreestanding" "${LINKING_FLAGS[@]}" "-T" "$LINKER_FILE_DIRECTORY_NAME/linker.ld" "${OBJECT_FILES[@]}" "-o" "$ELF_DIRECTORY/$ELF_FILE_NAME" "-lgcc")
echo -e "\n\n===================== Command Used =====================\n"
echo -e "${LINKER_CMD[@]}\n"
echo -e "=====================================================================================================\n"
${LINKER_CMD[@]}
echo -e "\n\nThe ELF $ELF_DIRECTORY/$ELF_FILE_NAME is successfully generated\n\n"
echo -e "******************************** STAGE 3 DONE *******************************************************\n"


# ====================================================================================================================
#                                          STAGE 4

set -e

echo -e "\n\n===================== Running STAGE 4 to generate the BIN FILE =====================\n"
# STAGE 4: FINAL OBJECT (BIN) FILE STICHING
BIN_FILE_NAME="firmware.bin"
BIN_DIRECTORY="elf_and_bin"

BIN_CREATE_CMD=("riscv-none-elf-objcopy" "-O" "binary" "-S" "$BIN_DIRECTORY/$ELF_FILE_NAME" "$BIN_DIRECTORY/$BIN_FILE_NAME")
echo -e "\n\n===================== Command Used =====================\n"
echo -e "${BIN_CREATE_CMD[@]}\n"
echo -e "=====================================================================================================\n"
${BIN_CREATE_CMD[@]}
echo -e "\n\nThe Executable BINARY $BIN_DIRECTORY/$BIN_FILE_NAME is successfully generated\n\n"
echo -e "******************************** STAGE 4 DONE *******************************************************\n"
