----FogOSv2 Cat Flags Implentation----

****Running****
	To run:
		1. Run make 
		2. Run make qemu
		3. Have fun!

****Makefile addins****
	- Added a tester.txt in the fs.img section

****Usage****
	cat allows users to print out the contents of files

	Flags implemented:
		-n (numbers all lines)
		-b (numbers nonempty line)
		-e (display non-printing characters (see the -v option), and display a dollar sign (‘$’) at the end of each line)
		-t (display non-printing characters (see the -v option), and display tab characters as ‘^I’)
		-s (squeeze multiple blank lines)
		-v (show non-printing characters visibly)
		--help (prints out what cat can do and what it's flags does)

****Testing****
	Here are some tests to run and their expected outputs:
_________________________________________
/cat tester.txt
Hello world

	This line starts with a tab
This line has a DEL char: 
This line has a BEL char: 

Another blank line follows:


End.
_________________________________________
/cat --help
Usage: cat [OPTION]... [FILE]...
Concatenate FILE(s) to standard output.

Options:
  -n        number all lines
  -b        number non-empty lines
  -e        display non-printing characters (see the -v option), and display a dollar sign (‘$’) at the end of each line
  -t        display non-printing characters (see the -v option), and display tab characters as ‘^I’.
  -s        squeeze multiple blank lines
  -v        show non-printing characters visibly
  --help    display this help
_________________________________________
/cat -n tester.txt
1	Hello world
2	
3		This line starts with a tab
4	This line has a DEL char: 
5	This line has a BEL char: 
6	
7	Another blank line follows:
8	
9	
10	End.
_________________________________________
/cat -b tester.txt
1	Hello world

2		This line starts with a tab
3	This line has a DEL char: 
4	This line has a BEL char: 

5	Another blank line follows:


6	End.
_________________________________________
/cat -e tester.txt
Hello world$
$
	This line starts with a tab$
This line has a DEL char: ^?$
This line has a BEL char: ^G$
$
Another blank line follows:$
$
$
End.$
_________________________________________
/cat -t tester.txt
Hello world

^IThis line starts with a tab
This line has a DEL char: ^?
This line has a BEL char: ^G

Another blank line follows:


End.
_________________________________________
/cat -s tester.txt
Hello world

	This line starts with a tab
This line has a DEL char: 
This line has a BEL char: 

Another blank line follows:

End.
_________________________________________
/cat -v tester.txt
Hello world

	This line starts with a tab
This line has a DEL char: ^?
This line has a BEL char: ^G

Another blank line follows:


End.
_________________________________________
/cat -n -e tester.txt
1	Hello world$
2	$
3		This line starts with a tab$
4	This line has a DEL char: ^?$
5	This line has a BEL char: ^G$
6	$
7	Another blank line follows:$
8	$
9	$
10	End.$
_________________________________________
/cat -n tester.txt README.md
1	Hello world$
2	$
3		This line starts with a tab$
4	This line has a DEL char: ^?$
5	This line has a BEL char: ^G$
6	$
7	Another blank line follows:$
8	$
9	$
10	End.$
_________________________________________
$ /cat -n tester.txt README.md
1	Hello world
2	
3		This line starts with a tab
4	This line has a DEL char: 
5	This line has a BEL char: 
6	
7	Another blank line follows:
8	
9	
10	End.
1	# FogOS
2	
3	Fall 2025 Edition
4	
5	![FogOS](docs/fogos.gif)
6	
7
_________________________________________
/cat -x tester.txt
unknown option -x
_________________________________________
