# More Documentation

## Overview

The `more` command is a file pager utility that displays the contents of a file one screen at a time. It allows users to navigate through large files efficiently by showing a limited number of lines per page and waiting for user input before continuing.

## Usage

```bash
more <filename>
```

### Command Line Options

- `more <filename>` - Display the specified file 
- `more --help` - Show help information and available commands

## Features

### Pagination
- Displays **24 lines per page** by default
- Automatically pauses after each screenful
- Shows `--More--` prompt when waiting for user input

### Navigation Commands

| Command | Action |
|---------|--------|
| `<Space> + <Enter>` | Display next page of file |
| `<Enter>` | Display next line of file |
| `q + <Enter>` | Exit/quit from more |
| Any other key + `<Enter>` | Display help information |


#### Data Structures
```c
struct {
    int space;    // Next page flag
    int enter;    // Next line flag  
    int quit;     // Quit program flag
} more_flags;
```

#### Core Functions

1. **`read_line(int file, char *buffer, int max_len)`**
   - Reads one line from the file descriptor
   - Handles end-of-file conditions
   - Returns 1 for successful read, 0 for end of file

2. **`User_command(void)`**
   - Waits for user input from keyboard
   - Sets flags based on user input
   - Handles help display for unknown commands

3. **`more(int file)`**
   - Main pagination logic
   - Manages line counting and page breaks
   - Handles user navigation commands

4. **`print_help(void)`**
   - Displays help information
   - Shows available commands and usage instructions


### Error Handling
- Checks file open status
- Provides clear error messages for invalid usage


# Show help
```
more --help
```

## Interactive Navigation
1. File opens showing first 24 lines
2. User sees `--More--` prompt
3. User presses `<Space> + <Enter>` to see next page
4. User presses `<Enter>` to see next line
5. User presses `q + <Enter>` to quit


## Error Messages

- `Usage: more <filename>` - Incorrect number of arguments
- `Try 'more --help' for more information.` - Suggestion for help
- `more: cannot open <filename>` - File not found or permission denied
- `Unknown command. Available commands:` - Invalid navigation command


## Limitations

- Fixed page size of 24 lines 
- No backward navigation (forward-only)
- No search functionality
- No line numbering
- Limited to single file display

### Implementation Reference

See `user/more.c` for the implementation details.


