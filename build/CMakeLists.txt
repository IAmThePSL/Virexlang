cmake_minimum_required(VERSION 3.10)

# Project name and version
project(VirexLang VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Add the executable
add_executable(virex src/main.c src/repl.c)

# Add the lexer source files
add_executable(virex src/lexer/lexer.c src/lexer/token.c)

# Add the parser source files
add_executable(virex src/parser/parser.c src/parser/malloc.c)

# Add the statement source files
add_executable(virex
    src/statements/ifstatement.c
    src/statements/printstatement.c
    src/statements/variabledeclaration.c
    src/statements/whilestatement.c
)

# Add the test executable
enable_testing()
add_executable(test_runner tests/test_runner.c)

# Add the test source files
add_executable(test_runner
    tests/test_helpers.c
    tests/test_lexer.c
    tests/test_main.c
    tests/test_parser.c
    tests/test_statements.c
)

# Link the test executable with the main executable
target_link_libraries(test_runner virex)

# Add tests
add_test(NAME test_runner COMMAND test_runner)
