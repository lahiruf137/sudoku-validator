## Compile Instructions

### Processes
gcc -std=c99 sudoku_validate.c -o sudoku-processes -lpthread

### Threads
gcc -std=c99 sudoku_pthread.c -o sudoku-threads -lpthread

Usage
Usage: sudoku-processes <input_file> <max_delay>
Usage: sudoku-threads <input_file> <max_delay>

### Example
./sudoku-processes input.txt 1
