# CMSC 421 - Project 3
This is my repository for the third CMSC 421 project. We have been tasked with creating a few linux kernel modules. Specifically we are required to create an inspiration module, and a Tic-Tac-Toe Module.


## Part 1 : Inspirational Quote Device  
This will be a character device driver, located at */dev/inspiration/*. The MAJOR, and MINOR for this device will be generated dynamically for the sake of simplicity. All files related to this device are stored within the */part1/* directory here on GitHub. When compiled using the instructions in the part1 readme, a file called *inspiration.ko* will be created. 

### Included Files
**inspiration.c**
- This is the main code body for the driver, this contains all code needed by the device driver.
- When compiled, this will create the inspiration.ko file which will become the device driver.  
  
**Makefile**
- There are 2 ways to run the *make* command on this driver.
- Using this file, the directory can be cleaned, or the file can be made.
- *make all*
- *make clean*
- More data in the part 1 Readme.  
  
**README.MD**
- Part 1 specific readme
- Contains all information about the first part of the project.
- Instructions on compilations, and installation.

## Part 2 : Tic-Tac-Toe Kernel Module  
This will recreate the game of Tic-Tac-Toe as a kernel module. It must display the 3x3 board to the player when called. The module will be called tictactoe.ko, and will create the character device called /dev/tictactoe, which must be readable, and writable by all users. All code, and files for this module will be included in the /part2/ directory. Currently this includes tictactoe.c, Makefile, and a readme.md which gives more detail about the module.

### Included Files
**tictactoe.c**  
- This is the main code base for the character device, this contains all of the code required for the driver.
- When this is compiled, it will create tictactoe.ko.
  
**Makefile**  
- There are 2 ways to run the make command on my driver.
- Using the makefile commands you may clean, or compile the driver file.
- *make all*
- *make clean*
- More data in the part 2 Readme.
  
**README.MD**  
- Markdown readme for part 2
- Contains all of the information about the second part of the project.
- Instructions on how to compile and install.
