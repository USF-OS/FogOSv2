# cmp overview

'cmp' will compare two files byte by byte and alert user when there is a difference. 

There are 2 possible results
1. No output = files are the same
2. No output = files differ

It is important to note that cmp is whitespace inclusive.

## How to use
1. Run  ```make qemu```
2. Run ``` cmp <path/FILE1> <path/FILE> ```
3. Review results

## Built in Testing Files 
- "cmp same1.txt same2.txt"
    - Will output nothing as files are the same
- "cmp dif1.txt dif2.txt"
    - files differ in wording
    - Output: ```tests/dif1.txt tests/dif2.txt differ: byte 41, line 1```
- "cmp dif1.txt eof.txt"œ
    - eof.txt ends before the other but content is the same
    - Output: ```tests/dif1.txt tests/dif2.txt differ: byte 26, line 1```
