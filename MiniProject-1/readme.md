# Assumptions

## General
 Max length of input is 4096.  

 Entering exit as command will close the terminal.

## spec - 3

 If hop dir1 dir2 is given and dir2 are invalid, directory will first change to dir1 and then prints error message for dir2.

 If only hop is given,it will directly go to home directory i.e where shell is invoked.

 IF 'hop dir1 -' is given,it will return to same directory it started.

 Only hop - will work.Not handled 'hop -/dir' cases.


## spec - 4

 If reveal - as first prompt(i.e prev_dir is not set),It will print a warning "OLD_PWD not set".

 Assuming Max no.of entries in a directory is 1024.

 If reveal -l is given,the colour change is reflected only on file/dir name.


## spec - 5

 Erroneous commands will also get stored in log.

 Always an absolute path in the home directory is given to store commands in history.txt file.

 Any input having "log" as substring in it wont be stored even if it is a file name.

 
## spec - 6

 If Input has multple foreground process with execution time more than 2 sec,
 then in the next prompt,it will display the most recent one.

 For an Invalid background command, it will just print pid and then 'execvp() failed' and 'exited normally'.

 Background wont work for custom functions.If given it will print Execvp() failed.

 Max number of background process that can be done 1024.


## spec -10

 All the error will be printed to stdout.

 Assuming Filename only at the end of input.

## spec - 11

 Assuming hop .. | wc wont change the directory.will count the words from the output of hop ..

## spec - 12

 If & is given it will do background only for the preceding command. 

## spec - 13
 
 If error command is happened,still it will be stored in activties.

## spec - 14
 
 If ctrcl + c is given when no foreground process are running then ,it will remain in same line and still accept input.(not new prompt)

 If ctrl + d is given,then it will print all the pids and print them as killed.

## spec - 15

 If sleep 5 is in foreground then after 5 sec only ctrl + d will be accepted and then terminal closes.

 After converting background to foreground from them if it takes more than 2 seconds prompt will change.


## spec - 16

 If neonate with time interval is given ,it will print invalid command.






