# Less Documentation
## Overview
The `less` command is a file pager utility designed for UNIX and similar operating systems to display the contents of a user-specified file one screen at a time. Unlike the `more` command, `less`' allows for both forward and backward scrolling throughout the file This allows the user to have more flexibility and efficiency while reading a file, particularly larger ones. For enhanced readibility, the interface displays the first **24** lines in the file, unless the user enters the respective command(s) to scroll forward and/or backward.

## Usage
``` 
less <filename>
```
### Command Line Options
`less <filename>` - Display the specified file 

`less --help` - Show help information and available commands

`<Enter>`- Move to the next line 

`<Space>` Move to the next page

`g`- Redirect to beginning/top of the file

`G`- Redirect to the bottom of the file

`b`- Redirect to the previous page 

`q`- Quit

## Features
### Pagination
* Displays **24** lines per page by default
* Automatically pauses after each screenful
* Shows --Less-- prompt when waiting for user input

### Navigation Commands
| Command | Action |
|---------|--------|
| `<Space>` | Move to the next page |
| `b` | Move to the previous page |
| `<Enter>` | Move to the next line |
| `g` | Go to the beginning/top of the file |
| `G` | Go to the bottom of the file |
| `q` | Quit `less` |
| `Up arrow` | Move up one line |
| `Down arrow` | Move down one line |




### Data Structures

#### File Storage 
```
char *lines[MAX_LINES];     // Array that stores each line of the file

int total_lines;        // Number of lines that are currently loaded
```
#### User Commands
```
char cmd;       // Current command input by the user
                // 'q'  -> quit
                // ' '  -> next page
                // '\n' -> next line
                // 'b'  -> previous page
                // 'g'  -> top of file
                // 'G'  -> bottom of file
                // '⬆️'  -> up arrow (previous line)
                // '⬇️'  -> down arrow (next line)

```

#### Constants
```
#define LINES_PER_PAGE 24  // Number of lines being displayed
#define MAX_LINES 100      // Maximum number of lines from file
#define LINE_BUF 128       // Maximum characters per line in file

```

### Core Functions
1. `int read_line(int fd, char *buf, int size)`
  
   * Reads one line from the file descriptor(FD)
   * Handles conditions and operations at the end of the program
   * Returns 1 if the file was loaded and read as expected, and 0 for the end of a file

2. `void load_file(int fd)`
    * Loads the file by each line, and into the `lines` array
    * Allocates memory dynamically for each line

3. `char get_command(void)`
    * Reads the command given by the user
    * Detects escape sequences for the arrow keys
    * Returns a character for the navigation command

4. `void less(int fd)`
    * Displays lines onto the screen and handles the user input accordingly
    * Frees the memory when quitting

### Error Handling
* Checks file open status
* Provides clear error messages for invalid usage of `less`

# Show help
less --help
```
### Interactive Navigation
1. File opens showing first 24 lines
2. User sees `--Less--` prompt
3. User presses `<Space>` to see next page
4. User presses `<Enter>` to see next line
5. User presses `q` to quit
```

## Error Messages
* `Usage: less <filename> `- Incorrect number of arguments
* `Try 'less --help' for more information.` - Suggestion for help
* `less: cannot open <filename>` - File not found or permission denied
`Unknown command. Available commands:`- Invalid navigation command

## Limitations
* Fixed page size of **24** lines
* Limited to single file display
