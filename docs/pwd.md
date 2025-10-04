# pwd - For FogOSv2

## Overview

The print working directory (pwd) command retrieves the user's current working directory and prints it out for the user to see. 
It is useful and allows the user to see where they are in their file locations and directories.

As the user uses cd to travel into directories, the shell will automatically update the prompt to showcase your location. On startup 
our current working directory would be root (/) so you will see /$, before typing into the operating system.

## Usage

- pwd can be executed at anytime by typing 'pwd' into the prompt. Here are the steps to ensure the OS will compile and boot as normal

- When preparing the OS follow these steps

-- make clean
-- make
-- make qemu

*Once Booted*

-- /$ pwd
-- / --> output

### Examples

*On Startup*
/$ pwd
/

/$ mkdir temp
/$ cd temp
/temp$ pwd
/temp$

/$ mkdir temp
/$ cd temp
/temp$ /mkdir temp2
/temp$ cd ../temp/temp2
/temp/temp2$ pwd
/temp/temp2

## Testing

On startup the shell will initialize 6 total directories. There will be 2 main parent directories titled 'test1' and 'test2' respectively.
Inside 'test1' we have a child directory called 'show'. Inside of 'show' we have a child directory of 'done'
Inside 'test2' we have 2 children directories named 'show1' and 'show2'

The user must manually cd into these directories and then do pwd on their own.

Expected outcomes will look like

/test1/show/done --> when the user cd's test1, show, and done, then executes pwd
/test1/show --> when the user cd's test1 and show, then executes pwd
/test1 --> when the user cd's into test1 and executes pwd

/test2/show1 --> when the user cd's test2 and show1, then executes pwd
/test2/show2 --> when the user cd's test2 and show2, then executes pwd
/test2 --> when the user cd's into test2 and executes pwd

/ --> if the executes pwd on bootup

When the user executes cd, the current working directory will update at the same time so special cases such as including will still show the correct
current working directory.

Examples of this would look like:

/$ cd test1/show/done/../../../test1
/test1$ pwd
/test1

/$ cd test2
/test2$ cd show
/test2/show$ cd ../show/..
/test2$ pwd
/test2
