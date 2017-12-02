# FindFido

Files Included:
getword.h
getword.c
findFido.c
sendAlert.py
masterscript.txt

File Descriptions:

__getword.c__
This file contains a function getword() which recieves the output from ping and parses eachword until an EOF (end of file) signal is recieved.

__findFido.c__
This file contains the driver function for all the C code in this project. It repeativly calls getword() and stuffs each word into a character array called "buffer". Once the string "100%" is found, this function will return "FAIL" to stdout

__sendAlert.py__
This file will send an SMS message to my phone with a message to alert the FindFido device is no longer visible to the network

__masterscript.txt__
This script continously trys to ping the ipaddress associated with the computer located on the "dog". The data sent back from this ping command gets sent to findFido.c and if the result of the ping is "100% packet loss" sendAlert.py is invoked. Otherwise these steps continue to repeat
