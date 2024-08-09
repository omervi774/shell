# Mini Shell Program

## Author
**Omer Weinbrand**

## Description
This program implements a mini shell that accepts commands from the user, processes them, and executes them using the `fork` and `execvp` system calls. The shell supports basic commands, including history tracking, and handles simple error cases such as invalid inputs. The program counts the number of commands entered and the total number of characters used, displaying these statistics when the user exits the shell by typing "done".

## Prerequisites
- A Linux-based operating system.
- GCC compiler (`gcc`).
- Basic understanding of C programming and Linux shell commands.

## How to compile?

1. **Clone or Download the Source Code:**
   Ensure you have the source code available on your machine.

2. **Compile the Program:**
   Open your terminal, navigate to the directory containing the source code, and run the following commands:
   
   1. gcc shell.c -o shell

   2. run: ./ex2a

## Use the shell

- Type any command supported by your Linux shell.
- Use history to view the history of entered commands.
- Type done to exit the shell and see the statistics.

note: The history of commands is stored in a file named file.txt located on your Desktop.

   

