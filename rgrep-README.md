# RGREP

A recursive Version of grep that searches through the entire
directory trees. It finds the thing you are looking for. 

Makefile: 
Make sure to add rgrep.c to your user folder, and update your UPROGS
section to include -rgrep\. 

Main Info:
rgrep takes the pattern matching code from regular grep and
adds recursive directory traversal. When it hits a directory,
it opens it, reads all the entries, and recursively searches 
subdirectories. Files get searched using the same line-by-line 
matching as regular grep.

# -V 
Flag: 
Show all lines that DON'T start with a comment '-v'
rgrep [-v] pattern [directory]


# Extras
Note: 
Implements custom strcat since xv6's user library doesn't have it

Side Note: 
Buffer size: 1024 bytes - Not 512 :'(


# TESTING
Tests:
To test I did wget on the time-machine.txt and the used fogOS file.
My series of tests would run as follows

To test flags:

rgrep fog
rgrep -v fog

To Test Case Sensitivity: 

rgrep save
rgrep Save

This works when you do the time-machine.txt rm tm.txt and 
add to Makefile. 


![PENGUIN](pingu.png)
