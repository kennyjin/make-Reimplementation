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

## Program Structure

This program can be divided into 5 parts.
* Main File
* Build Specification
* Command line Execution
* Specification Graph
* Text Parsing

### Main file

The main function of the program is in it.

### Build Specification

A typical makefile may look like this:

```

all: Build.o main.o progexec.o specgraph.o text_parsing.o
	gcc -o 537make Build.o main.o progexec.o specgraph.o text_parsing.o
Build.o: Build.c Build.h
	gcc -Wall -Wextra -c Build.c
main.o: main.c specgraph.h progexec.h text_parsing.h
	gcc -Wall -Wextra -c main.c
progexec.o: progexec.c progexec.h
	gcc -Wall -Wextra -c progexec.c
specgraph.o: specgraph.c specgraph.h progexec.h
	gcc -Wall -Wextra -c specgraph.c
text_parsing.o: text_parsing.c text_parsing.h
	gcc -Wall -Wextra -c text_parsing.c

clean:
	rm Build.o main.o progexec.o specgraph.o text_parsing.o
	rm 537make

```

A *build specification* is a struct with 3 fields.
* Target
* Dependence
* Command Line

Take a snippet of the makefile above:

```
Build.o: Build.c Build.h
	gcc -Wall -Wextra -c Build.c
```
In this case, 
* The target is "Build.o".
* The dependence is "Build.c" and "Build.h".
* The command line is "gcc -Wall -Wextra -c Build.c"

### Command Line Execution

This parts executes a command line in the C program by forking a child process.

The command line that is to be executed are gcc commands, such as:

```
gcc -Wall -Wextra -c Build.c
```

### Specification Graph

Sometimes the *make* program needs to deal with bad *makefile*.
The *makefile* might contain cycles in dependencies. For example:

```
main1.c: main2.c
    echo building main1.c!
main2.c: main1.c
    echo building main2.c!
```
To deal with the situation, a graph of *build specification* is created, and cycle detection algorithm is applied to the graph. If a cycle is detected, the program stops making.

### Text Parsing

This part parses the text in the *makefile* and returns an "ArrayList" of all the *build specifications*.


## Development Procedure

Ying Zhang coded the part of reading makefile and constructing build specifications, 
Kenny Jin finished the part of program execution and graph operations. 
