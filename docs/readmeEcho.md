## echo

Print arguments to standard output.

### Synopsis

```text
echo [OPTION]... [STRING]...
```

### Description

Writes each `STRING` to standard output separated by a single space. By default a trailing newline is printed.

### Options

- **-n**: Do not output the trailing newline.
- **--help**: Display help message and exit.

Unknown options cause an error and a non-zero exit status.

### Notes

- With no arguments, `echo` prints a newline and exits with status 0.
- `echo -n` with no strings prints nothing (no newline) and exits with status 0.
- `--` is not treated as an end-of-options marker; it is parsed as an option and results in an error.

### Examples

```sh
# Basic usage
echo hello world
# Output: "hello world" followed by a newline

# Suppress the trailing newline
echo -n "no newline"
# Output: "no newline"$ -- without new line

# Shows help and exits
echo --help

# Attempting to use "--" as end-of-options is not supported
echo -- -not-an-option
# Output:
# echo: unknown option --
# Try 'echo --help' for more information.
```

### Implementation Reference

See `user/echo.c` for the implementation details.


### See Also

For example cases and expected outputs, see `user/echo-tesing.txt`.



