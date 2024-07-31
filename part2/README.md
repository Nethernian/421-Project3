# Tic-Tac-Toe Module
All code contained in the */part2/* directory is code relevant to project 3 - part2 as described in the project documents. When compiled the code will produce a *tictactoe.ko* file, as well as it's required auxilliaries. This tictactoe file will then need to be installed. Detailed instructions on how to compile, and install this module are included later on in the readme. This module should allow for the user to play tictactoe against a random 'AI' opponent.

## How it Works
*/dev/tictactoe* is implemented as a module that can be both written to and read from through the use of read(), and write() or through the terminal through the use of the **cat** and **echo** commands. Specific instructions for formatting these instructions is included later in the documentation. The tictactoe game currently only supports a 3x3 board, and the player is always assumed to be placing X's while the computer is expected to place the O's. 

## Included Files
### tictactoe.c  
This file is the main codebase for the project, and facilitates the creation, insertion, removal, read from, and write to methods required by the class. When created from this file, the module will be dynamically assigned a major, and a minor based on the other moudles already present on the system. Please not that when created, the module currently requires *chmod* to be run on it for write access.  

### Makefile  
This makefile will compile the device driver, and by default come with two commands. **make clean** and **make all**.  
**make all**  
- Running this command in terminal will compile all the objects required to create the device driver.  
  
**make clean**  
- This will remove all of the object files compiled by the make all command.  

## How to Install the Device.
In order to install this device, you must first compile all required files using the *make all* command, as detailed in the description of the makefile. Once these object files have been created, you must run the following command: *sudo insmod tictactoe.ko*, this will install the device driver module at */dev/tictactoe*. Once this has been installed, you will need to set the file permissions using *chmod 666 /dev/tictactoe*. I understand that this is not the desired behaviour for the module assignment, however I could not find a way to work around it.

## Known Issues
- Currently there is an issue with giving the module proper permissions
- ~~The Tic-Tac-Toe game loop is not currently implemented~~

## Sources Used 
- https://www.kernel.org/doc/Documentation/filesystems/vfs.txt  
- https://linux-kernel-labs.github.io/refs/heads/master/labs/device_drivers.html  
- http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/  
- https://tldp.org/LDP/lkmpg/2.6/html/lkmpg.html#AEN121  
- https://stackoverflow.com/  
- And of course the slides provided in the lectures.
