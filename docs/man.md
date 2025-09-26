                  [ ‘man’ Command-Line-Utility Documentation ]


'man' is a command-line-utility (CLU) that displays pages / documentation for commands
or c-lib functions, drawing from a library of txt files in the man_docs
directory.


The man_docs directory itself contains various subdirectories, each of which are
dedicated to a specific topic. Currently, two directories are included, "1" for
command-line-utilities and "2" for C-Library functions.

- Adding new directories and man pages is as simple as creating a new
subdirectory in man_docs and some .txt files within it. 

- Subdirectory naming follows a numbering convention, from 1-9. Double digit or
non-numeric names are not supported currently (how many libraries do we need).





'man' CLU has various flags to aid in displaying or searching for specific
man documentation pages within its database. Here is a tentative list as follows
below.


'-h': Displays the help page. Self-explanatory.

Ex. "man -h"



'-p': The "buffered print" flag, will display the pages by a certain numnber of
lines at a time. Not providing a valid number of
lines after -p will cause a default 10 lines to be printed at a time.

Ex. "man adoc -p 2"



'-c': The "clear after print flag", typically used with buffered print when
viewing the page in chunks, the previous displayed output will be cleared when
printing the next chunk of lines.

Ex. "man adoc -p 4 -c"



'-d': The "debug" flag, enabling this will have the program print out various
variables during execution, useful in surface-level debugging.

Ex. "man adoc -p 1 -d"



'-s' The "search" flag, enabling this will have the 'man' utility search all
directories for the command or function. Will display the first result. Will
override any directory number that has been passed to the command.

Ex. "man adoc -s"




                              [ ‘man’ Test Cases ]

Implementing automatic testing for 'man' can be problematic, to say the least. 
However there are a variety of commands that can be run to stress test the
utility. 

'man' flags, save for the '-p' flag which interprets the single or double digit
number after it as the number of lines, can generally be typed in out of order.
There are some exceptions to keep in mind though.

- 'man' does not support displaying more than one doc at a time, this means that
  passing more than one filename to the utility will result in only the first
filename being opened. (e.g. "man 2 malloc -p ls") will only display malloc.

- If not given an actual flag, 'man' will assume that the command or function
  you are looking for to be in the "1" directory of man_docs or the
command-line-utilities. 

- The '-c' flag has no effect if '-p' buffered print is not inputted.


Now for the actual test cases... 
Expected behavior is that the page is printed and displayed in the correct
manner. Additional man pages can be added to "man_docs" as needed.

"man 1 mpat -p 2 -c" 
"man mpat -p -c 1"
"man 2 malloc -c"
"man mstat -p 20 -c 2"
"man malloc -s -c -p 1"
"man"
"man -h"





