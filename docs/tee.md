# FogOS tee

### Build

1. make clean

2. make 

3. make qemu

After running these commands, run `ls` to make sure the `tests` directory is listed so you can run the tests below!

## Purpose

`tee` reads from standard input and writes it to both standard output and up to 10 files at a time.

The command `tee [file]` will read from standard input and write the contents of ***what was just added*** to both the standard output and the specified file. If the file does not exist, it will be created. If it *does* exist, it will be overwritten by default.

The **-a** flag can be used as well. When `tee -a [file]` is used, it will append the input to the **end** of the given file(s), without overwriting it.

- **Note:** If using standard input (e.g. keyboard) to write to these files, every line entered will be written to standard output and to the file(s) until `ctrl+d` is pressed.

<br>

If you would like to see if it worked, use the **cat** command!

        cat file.txt

<br>

Examples
---

*Check the results using* `cat [file]`

<br>

1. Creates a file `test.txt` and reads from standard input. Once you press `enter` and `ctrl+d`, what you wrote is written to the terminal and to the file!

        tee test.txt 
    
    - Your input will be printed to the terminal and written to the newly created file `test.txt`
    - See result: `cat test.txt`

<br>

2. Appends to `test.txt` using the **-a** flag.
        
        tee -a test.txt

    - Your input will be printed to the terminal and appended to the existing `test.txt`
    - See result: `cat test.txt`

<br>

3. Using `echo` to add *Hello* to file1.txt
        
        echo Hello | tee file1.txt 

    - Only *Hello* will be printed to the terminal and written to the newly created file `file1.txt`
    - See result: `cat file1.txt`

<br>

4. Copying the contents of `test.txt` into `copied.txt` 

        cat test.txt | tee copied.txt

    - Only the contents of `test.txt` will be printed to the terminal and written into the newly created file `test.txt`
    - See result: `cat copied.txt`


<br>

5. Using the test files, if you append `sqd_btm.txt` to `sqd_top.txt` hit enter and `cat tests/sqd_top.txt` you will get an ASCII drawing of squidward! 

        cat tests/sqd_btm.txt | tee -a tests/sqd_top.txt

    - Only the contents of `tests/sqd_btm.txt` will be printed to the terminal and appended to the existing file `tests/sqd_top.txt`
    - See result: `cat tests/sqd_top.txt`

<br>

6. This examples demonstrates appending to multiple files. It appends the first fish ascii art into the other 2 fish files.
        
        cat tests/fish.txt | tee -a tests/fish1.txt tests/fish2.txt

    - Only the contents of `tests/fish.txt` will be printed to the terminal and appended to the existing files
    - See results: `cat tests/fish1.txt` & `cat tests/fish2.txt`

<br>

7. This shows a more useful way to use this command. If you would like to pull out any error messages from one file and put it in another file, you can! In this case, `grep` searches for the word `The` in `brwn_fox.txt` and prints out and writes the lines that contain `The` to `find.txt`. 

        grep The tests/brwn_fx.txt | tee find.txt

    - All 6 lines in the file will be printed to the terminal and written to the newly created file `find.txt`
    - See result: `cat find.txt`

<br>

8. Here is another one! For this example, all the lines that include `can't` in `gingerb.txt` is appended to the `find.txt` that was created above. 
    
        grep can't tests/gingerb.txt | tee -a find.txt

    - The lines 2, 3, 4, and 6 will be printed to the terminal and appended to the existing file `find.txt`
    - See result: `cat find.txt`

