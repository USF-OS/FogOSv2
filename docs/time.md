# Time

This branch aims to implement the Time program from the original Linux OS to FogOS. Time aims to measure the time it takes for other programs to run.

## Usage
### General:
- time [program here] [other arguments]
### Examples:
- time ls
- time ploop 1550000000
- time mkdir myfolder

## Flag
- -p : Prints out the time in POSIX format

## Output
### Explanation
It should print out the real, user, and sys time in that order. Real time (real) is the total elapsed time that passed since running your program of choice. It includes bits of time that the CPU does not capture. User space time (user) is the total amount of time the CPU spends in the user space. Kernel space time (sys) is the total amount fo time the CPU spends in the kernel space.
### Examples
#### Default
real	0m0.000s

user	0m0.000s

sys		0m0.000s
#### POSIX (-p)
real	0.00

user	0.00

sys		0.00
### Note
Kernel Space Time + User Space Time does NOT equate to Real Time and, by extension, any equivalent algebraic expressions of that is also not true

## Tests
So you're looking to code review this. Awesome. I need the help
### What you can do
You can start by doing basic commands like:
- time			(Should give 0's across the board)
- time -p 		(Should give 0's across the board)
- time ls		(Compare it with C's time ls. ls in the OS shold be slower)
- time mkdir
### PLOOP
ploop.c is another utility that I added that just does a for loop.
#### Usage
ploop [number]

This argument sets the amount of times the program loops

To save you the trouble of a number too big, 1550000000 gives around 5 seconds. You can go ahead and tweak that number to your liking
#### In Conclusion
time ploop 1550000000 is the example usage case

Have fun!
