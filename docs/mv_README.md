# `mv` Command Documentation

## Overview

The `mv` command moves or renames files.
It can move files between directories, rename files, or overwrite existing ones (depending on flags).

* If the destination exists, behavior depends on flags (`-f`, `-i`).
* If the destination is a directory, the source file is placed inside that directory.
* With the `-v` flag, `mv` prints a confirmation message describing each move.

This behavior matches typical `mv`.

---

## Build Instructions
To build:
```sh
$ make clean
$ make qemu
```
or:
```sh
$ make clean && make qemu
```
This will compile both commands (`mv` and `rmdir`) and their corresponding test programs (`mvtest`, `rmdirtest`).
You can verify the binaries are included by running:
```sh
$ ls
```
You should see entries like:
```
mv
mvtest
rmdir
rmdirtest
```

---

## Usage

```sh
mv [-f] [-i] [-v] SRC DST
```

### Options

* `-f` – force overwrite without prompting.
* `-i` – interactive mode; ask before overwriting an existing file.
* `-v` – verbose mode; print what was moved.

---

## Examples

```sh
# Rename a file
$ echo "old data" > old.txt
$ mv old.txt new.txt
$ cat new.txt
old data

# Move a file into a directory
$ mkdir mydir
$ mv new.txt mydir
$ ls mydir
new.txt

# Move a file out again
$ mv mydir/new.txt .
$ ls
mydir  
new.txt

# Verbose mode (-v)
$ mv -v new.txt renamed.txt
'new.txt' -> 'renamed.txt'

# Move between directories
$ mkdir dir1 dir2
$ echo hi > dir1/x.txt
$ mv dir1/x.txt dir2
$ ls dir2
x.txt

# Move into directory with a trailing slash
$ mkdir ds
$ echo "hi" > file1.txt
$ mv file1.txt ds/
$ ls ds
file1.txt

# Overwrite a file without asking (-f)
$ echo old > a.txt
$ echo new > b.txt
$ mv -f a.txt b.txt
# 'b.txt' now contains "old"

# Interactive mode (-i)
$ echo "A" > one.txt
$ echo "B" > two.txt
$ mv -i one.txt two.txt
mv: overwrite 'two.txt'? (y/Y to confirm)
# 'two.txt' now contains "A"

# Same file detection
$ echo "same" > s.txt
$ mv s.txt ./s.txt
mv: 's.txt' and './s.txt' are the same file
```

---

## Test Program

The `mvtest` program automatically verifies correct behavior.
**Please run it before you try anything manually!**

Run it with:

```sh
$ mvtest
```

Expected output:

```
Test: simple move... OK
Test: -f overwrite... OK
Test: -v verbose output... OK
Test: move into directory... OK
Test: same file detection... mv: 's.txt' and './s.txt' are the same file
OK
Test: move into dir with trailing slash... OK
Test: move file between directories... OK
Manual test: run `mv -i src dst` and try typing y/n yourself.
```

---

## Notes
* mv can move files both into and out of directories.
* Moving a file to itself (like `mv file ./file`) prints an error and does nothing.
* Moving files into existing directories works with or without a trailing slash.
* Interactive (`-i`) mode requires manual input.
* Force (`-f`) mode skips confirmation.
* Verbose (`-v`) mode prints every rename in `'SRC' -> 'DST'` format.
