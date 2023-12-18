To compile the program, run:

g++ *.cpp -o cpusim

Then, to run the program, type the following command:

./cpusim [file_name]

where [file_name] is the input machine code file representing instructions. Some test files are provided in the repository, such as "23instMem-sw.txt". The output is the tuple (a0, a1),
where a0 and a1 represents the contents in register 11 and register 12 after running the instructions specified in [file_name]. For example, after running

./cpusim "23instMem-sw.txt"

I get the output (9,17), meaning register 11 stores the value 9, and register 12 stores the value 17 after the execution of 23instMem-sw.txt. If you want to create your own files to
run the simulator, each text file is the machine code translation of assembly files -- so you must translate your assembly file to machine code using the RISC-V instruction set.



