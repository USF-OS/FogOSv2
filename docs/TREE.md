clear Utility
Purpose
The clear command is used to clear all text from the terminal screen and move the cursor to the top-left corner (first row, first column).

Implementation
This utility works by writing specific ANSI escape sequences to standard output. Specifically, it sends the following two sequences:

- \x1b[2J: This sequence clears the entire screen.

- \x1b[H: This sequence moves the cursor to the top-left corner of the screen.

The code, located in the user/clear.c file, writes these sequences in a loop to ensure the full sequence is sent, even if the write operation is interrupted.

Expected Output

Running the clear command does not produce any text output. Instead, it has a visual effect:

Before clear:
```
$ ls
. .. README.md cat clear echo forktest grep init kill ln ls mkdir rm sh stressfs usertests wc zombie 
$ echo "Hello, World!"
Hello, World!
$ 
```
After clear:
The terminal screen will be completely blank, with the cursor positioned at the very top-left, ready for a new command.

tree Utility
Purpose
The tree command recursively displays the contents of a specified directory (or the current directory by default) in a tree-like format. After the traversal is complete, it prints a summary of the total number of directories and files found.

Implementation
The core of the tree utility is a recursive function named tree, located in the user/tree.c file.

- Directory Traversal: The function opens a directory and reads its entries one by one.

- Recursion: If an entry is a subdirectory, it increments the directory counter and calls itself with an updated prefix for indentation, performing a depth-first traversal. It explicitly skips the . and .. entries to avoid infinite loops.

- File Counting: If the entry is a file, it increments the file counter.

- Output Formatting: To create the tree structure, the function adds |--  to the prefix string at each level of recursion, which produces the visual indentation.

- Command-Line Arguments: If the user provides a path as an argument when calling the tree command, the program will start its traversal from that path. If no argument is provided, it defaults to the current directory (.).

Expected Output

If you run the tree command from the root directory of a simple file system, the output would look something like this:
```
$ tree
.
|-- README.md
|-- kernel
|   |-- console.c
|   |-- main.c
|   |-- vm.c
|-- user
|   |-- cat.c
|   |-- clear.c
|   |-- echo.c
|   |-- tree.c

2 directories, 8 files
```

Testing for Tree:

Run tree /tests/testtree:
the expected output should be:

```
|-- file1.txt
|-- file2.txt
|-- file3.txt
|-- testdir
|   |-- test4.txt

2 directories, 4 files
```

