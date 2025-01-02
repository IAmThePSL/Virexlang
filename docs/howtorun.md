# How to Run Virex

This guide explains how to build and run the Virex programming language.

## Prerequisites
* GCC compiler
* Make build system
* Terminal/Command Line interface

## Building from Source
* Open a terminal window
* Navigate to the Virex project directory:
* ```bash
	cd path/to/Virex
	```

Build the project using Make:
```bash
make
```

## Usage Options
### Running Files
To execute a Virex source file:
```bash
./Virex <path_to_vix_file>
```
Example:
```bash
./Virex examples/test.vix
```

### Interactive REPL
The REPL (Read-Eval-Print Loop) provides an interactive environment for testing Virex code.

1. Build the REPL:
```bash
gcc -o repl src/repl.c src/lexer.c src/token.c src/parser.c -Iinclude
```

2. Start the REPL:
```bash
./repl
```

## Important Notes

All commands should be executed from the project root directory
Make sure you have appropriate permissions to execute the compiled binaries
The REPL provides immediate feedback for testing language features and expressions

## Troubleshooting
If you encounter build errors:

Ensure all dependencies are installed
Verify you're in the correct directory
Check file permissions