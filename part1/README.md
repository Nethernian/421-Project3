# Inspiration Module
This is part 1 of the project, it will contain a linux kernel module called inspiration, the character driver for which will be located at /dev/inspiration. When this module is called by a user, it should return a random "Inspirational" quote.

## How It Works
*/dev/inspiration/* is implemented as a **READ ONLY** module, and any attempts to write to the module will return an *(EPERM)* error. Within the C code for this module, there is an array of 20 strings, each of which contains an inspirational quote and the attributed author of the quote. When the user requests some inspiration using the *cat /dev/inspiration* command (or any other method of calling a module), the module should return a randomly chosen quote from the 20 quotes in the array. When compiled, the kernel module will be called *inspiration.ko*.

## Included Files
### inspiration.c
This is the main body of code for this part of the project, this includes all functions necessary for the initialization, exit, release, and reading of the character device driver. Additionally this file handles denying attempts to write to the device, by returning -EPERM and making no operations.

### Makefile
This is the makefile for the module, it can be made as all, and as clean.
- **All** ccompiles all of the files that are needed to use and install the device driver.
- **Clean** removes all of the extra files (.ko, .o, etc.).

## How To Install The Module
The module must be compiled using the included makefile, and can be compiled through either the command *make* or *make all*. Once the module has compiled there should be a file named *inspiration.ko* and several other files.  
Once these files are generated, you will need to run the command *insmod inspiration.ko* as root. From there the module can be read from, and interacted with.
When you are done with the module you will need to run the command *rmmod inspiration.ko*.  
Additional information about the status of the module will be sent to the terminal console, which can be viewed through *dmesg* or similar.

## Known Issues
- To recieve data from this driver it must be read from as root/sudo.
- Currently the insnod function is not run when the file is compiled, I am not sure if it should be, or if instead this should be left up to the user.
- Error handling (-EPERM) needed for write attempts.
- I am not currently using the copy_to_user() function as I ran into a few issues with getting the information to be returned by cat. Instead of copy_to_user() I have decided to use the simple_read_from_buffer() function instead.

## Sources Used
http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/  
https://linux-kernel-labs.github.io/refs/heads/master/labs/device_drivers.html  
https://www.kernel.org/doc/Documentation/filesystems/vfs.txt  
