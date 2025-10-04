---- FogOSv2 Birthday, Date, Time Help Guide ----

-- How to Run --
Run commands 'make' and 'make qemu', then try one of the following commands. 

-- Birthday Commands --
birthday 
If the birthday is set, then it will tell the user "happy 
happy", otherwise it will say not yet!

birthday '-s' [MM/DD] 
birthday with -s flag will set the birthday. Must be in MM/DD format,
where both are integers.

birthday '--help' 
birthday with --help flag will deliver helpful information about the birthday command.

-- Date Command --
date  
This will return the date in format: MMM DD HH:MM:SS PDT YYYY
Note: MMM is a string. The rest are integers besides PDT.

-- Testing --
To test, run make qemu and type 'birthdaytest'.
Optionally, set the date to today and check if you get the happy birthday message.
