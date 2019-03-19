# Make Reimplementation

This is a reimplementation of the Linux command "make" and is a course project of CS537.

~~Detailed description of the command can be found on the webpage:~~
http://pages.cs.wisc.edu/~bart/537/programs/program3.html

Now that the webpage is no longer accessible.

This program is written in C and can only be executed on Linux machines.

## Compiling

This program uses GCC compiler and the pthread library. Since there is a makefile, just use "make" to compile the program.

## Running

Here are 2 examples of running:
```bash
./537make

./537make main.o
```

Since this program implements the basic functionalities of make, it can be used just like the Linux command "make" to compile normal C programs.




## Development Procedure

Ying Zhang coded the part of reading makefile and constructing build specifications, 
Kenny Jin finished the part of program execution and graph operations. 
