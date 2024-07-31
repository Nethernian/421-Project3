// File : tictactoe.c
// PROJ : CMSC 421 - Project 3
// Name : Nathan Wooddell - TC56612
// Desc : This will contain the C code for the Tic-Tac-Toe module as described in the project 3
// outline. This must be implemented as a device driver in /dev/tictactoe.


// ---- INCLUDE STATEMENTS ----
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/string.h>


// ---- DEVICE DEFINITIONS ----
#define DEVICE_NAME "tictactoe"
#define CLASS_NAME "tictacClass"


// ---- DEVICE INFO & DEFS ----
MODULE_LICENSE("GPL");										// Module License string
MODULE_AUTHOR("Nathan Wooddell");							// Author Name String
MODULE_DESCRIPTION("Project 3 - This is the Tic-Tac-Toe Module");
MODULE_VERSION("0.0.1");									// Module Version String
DEFINE_MUTEX(tictac);										// Definition for the Mutex


// ---- DEVICE GLOBAL VARIABLES ----
static struct class* TicTacToeClass = NULL;					// A class struct variable for the character driver device.
static struct device* TicTacToeDevice = NULL;				// A decvice struct variable for the character driver device.
static int MAJOR;											// A variable to hold the dynamically allocated Major value.
static char userMessage[256] = {0};							// Any message to be passed to the user.
static long len_Message;									// The Length of any message sent to the user.
static char* delimiter = " ";								// Used for parsing user input.
static int winner = 0;
static char g_b[3][3] = {									// The board to be edited as the game is played -> can be overwritten.
		{'-', '-', '-'},
		{'-', '-', '-'},
		{'-', '-', '-'}
};

// ---- DEVICE FUNCTION PROTOTYPES ----
static int tictac_open(struct inode *, struct file *);
static int tictac_release(struct inode *, struct file *);
static ssize_t tictac_read(struct file *, char *, size_t, loff_t *);
static ssize_t tictac_write(struct file*, const char *, size_t, loff_t *);


// ---- DEVICE DATA STRUCTURES ----

/* static struct file_operations fops 
This will be the file operations data structure for my device driver. This is how the I define the
macros for the device driver. This device is used to implement the following calls: open, read,
write, and release. */
static struct file_operations fops = {
		.owner = THIS_MODULE,
		.open = tictac_open,								//
		.read = tictac_read,								//
		.write = tictac_write,								//
		.release = tictac_release,							//
};


// ---- DEVICE FUNCTION DEFINITIONS ----

/* static int __init tictac_init(void)
This will be the initialization macro for the character device, and will need to dynamically
allocate the Major, and minor numbers of the device. This function will then create the character
device, and character class. Will return a 0 on success, and an error on failure. */
static int __init tictac_init(void) {
	printk(KERN_INFO "TicTacToe: Beginning Device Initialization\n");
	
	// Dynamic Major Number Allocation
	MAJOR = register_chrdev(0, DEVICE_NAME, &fops);
	if(MAJOR < 0){
		printk(KERN_ALERT "TicTacToe: Couldn't Register a Major Number\n");
		return MAJOR;
	}
	printk(KERN_INFO "TicTacToe: Device has been Registered with Number: %d\n", MAJOR);
	
	// Register Device Class
	TicTacToeClass = class_create(THIS_MODULE, CLASS_NAME);
	// TODO: Implement Error Checking
	
	// Register Device Object
	TicTacToeDevice = device_create(TicTacToeClass, NULL, MKDEV(MAJOR, 0), NULL, DEVICE_NAME);
	// TODO: Implement Error Checking
	
	printk(KERN_INFO "TicTacToe: Device Created Successfully.\n");
	return 0;
}


/* static void __exit tictac_exit(void)
This will serve as the exit macro for the Tic-Tac-Toe device driver. This will be called when the
module is uninstalled/removed. The module is responsible for destroying the devices, and classes
associated with the Tic-Tac-Toe driver. */
static void __exit tictac_exit(void) {
	device_destroy(TicTacToeClass, MKDEV(MAJOR, 0));
	class_unregister(TicTacToeClass);
	class_destroy(TicTacToeClass);
	unregister_chrdev(MAJOR, DEVICE_NAME);
	printk(KERN_INFO "TicTacToe: Device Successfully Unregistered\n");
}


/* static int tictac_open(struct inode *inodep, struct file *filep)
This function is called whenever the device driver file is opened. At the moment this function only
sends a message to KERN_INFO, however this may change in future iterations of the code. This 
function will return a 0 on a success, and an error on failure. */
static int tictac_open(struct inode *inodep, struct file *filep) {
	printk(KERN_INFO "TicTacToe: Device Driver Has Been Opened.\n");
	return 0;
}


/* static int tictac_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
This function is called whenever the device is read from. Ideally this will be used to print the
state of the board to Userspace/pass a buffer to the user. This function will return a 0 on a
success, and an error code on a failure. */
static ssize_t tictac_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
	printk(KERN_INFO "TicTacToe: Device Driver Is Being Read from.\n");
	// A super longwinded way to create the board.
	if(winner == 0){
		sprintf(userMessage, "--- A B C\n----------\n1 | %c %c %c\n2 | %c %c %c\n3 | %c %c %c\n", g_b[0][0], g_b[0][1], g_b[0][2], g_b[1][0], g_b[1][1], g_b[1][2], g_b[2][0], g_b[2][1], g_b[2][2]);
	}else if(winner == 1){
		sprintf(userMessage, "You Have Won! - Congratulations!\nNow use the RESET command to play again.\n");
	}else if(winner == 2){
		sprintf(userMessage, "You Have Lost! - Shame on You!\nNow use the RESET command to play again.\n");
	}
	
	printk(KERN_INFO "%s", userMessage);
	len_Message = strlen(userMessage);
	
	// Send the board to the user
	return simple_read_from_buffer(buffer, len, offset, userMessage, len_Message);
}


/* static ssize_t tictac_write(sturct file *filep, const char *buffer, size_t len, loff_t *offset)
This function will be called whenever the file is opened to be written to. This function will need 
to take an argument from the user, and process it to */
static ssize_t tictac_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
	// VARIABLE DECLARATION
	char** arguments = kmalloc(128*sizeof(char*), GFP_KERNEL);
	char* message;
	char* argument;
	int x,y, tolres, index1;
	long int index2;
	int placed = 0;
	int i = 0;
	
	message = (char*)kmalloc(256*sizeof(char), GFP_KERNEL);
	printk(KERN_INFO "TicTacToe: Device Driver is Being Written to with message: %s\n", buffer);
	
	// Copy from buffer to message
	if(copy_from_user(message, buffer, len)){
		return -EFAULT;
	}
	
	// Handle Parsing of User Command	
	while((argument = strsep(&message, delimiter)) != NULL){
		printk(KERN_INFO "TicTacToe: ARG: %s", argument);
		arguments[i] = argument;
		i++;
	}
	
	//  Handle User Input
	if(strcmp(arguments[0], "TURN")==0){
		// Handle User Choices
		if(arguments[1] == NULL || arguments[2] == NULL){
			kfree(arguments);
			kfree(message);
			return len;
		}else if(!strcmp(arguments[1], "A")){
			printk(KERN_INFO "TicTacToe: in A");
			index1 = 0;
			tolres = kstrtol(arguments[2], 10, &index2);
			index2--;
		}else if(!strcmp(arguments[1], "B")){
			printk(KERN_INFO "TicTacToe: in B");
			index1 = 1;
			tolres = kstrtol(arguments[2], 10, &index2);
			index2--;
		}else if(!strcmp(arguments[1], "C")){
			printk(KERN_INFO "TicTacToe: in C");
			index1 = 2;
			tolres = kstrtol(arguments[2], 10, &index2);
			index2--;
		}else{
			kfree(arguments);
			kfree(message);
			return len;
		}
		
		// Process user input 
		if(g_b[index1][index2] != '-'){
			printk(KERN_INFO "TicTacToe: Changing: %d,%ld\n", index1, index2);
			printk(KERN_INFO "TicTacToe: Tile Already Used - No Turn was taken\n");
			kfree(arguments);
			kfree(message);
			return len;
		}else{
			printk(KERN_INFO "TicTacToe: Changing: (%d,%ld)\n", index1, index2);
			g_b[index1][index2] = 'X';
		}
		
	// Check win condition
		// Horizontal Check
		if(g_b[0][0] == g_b[0][1] && g_b[0][1] == g_b[0][2] && g_b[0][2] != '-'){
			if(g_b[0][0] == 'X'){
				winner = 1;
				kfree(arguments);
				kfree(message);
				return len;
			}else{
				winner = 2;
				kfree(arguments);
				kfree(message);
				return len;
			}
		}else if(g_b[1][0] == g_b[1][1] && g_b[1][1] == g_b[1][2] && g_b[1][2] != '-'){
			if(g_b[1][0] == 'X'){
				winner = 1;
				kfree(arguments);
				kfree(message);
				return len;
			}else{
				winner = 2;
				kfree(arguments);
				kfree(message);
				return len;
			}
		}else if(g_b[2][0] == g_b[2][1] && g_b[2][1] == g_b[2][2] && g_b[2][2] != '-'){
			if(g_b[0][0] == 'X'){
				winner = 1;
				kfree(arguments);
				kfree(message);
				return len;
			}else{
				winner = 2;
				kfree(arguments);
				kfree(message);
				return len;
			}
		}
		// Vertical Check
		if(g_b[0][0] == g_b[1][0] && g_b[1][0] == g_b[2][0] && g_b[2][0] != '-'){
			if(g_b[0][0] == 'X'){
				winner = 1;
				kfree(arguments);
				kfree(message);
				return len;
			}else{
				winner = 2;
				kfree(arguments);
				kfree(message);
				return len;
			}
		}else if(g_b[0][1] == g_b[1][1] && g_b[1][1] == g_b[2][1] && g_b[2][1] != '-'){
			if(g_b[0][0] == 'X'){
				winner = 1;
				kfree(arguments);
				kfree(message);
				return len;
			}else{
				winner = 2;
				kfree(arguments);
				kfree(message);
				return len;
			}
		}else if(g_b[0][2] == g_b[1][2] && g_b[1][2] == g_b[2][2] && g_b[2][2] != '-'){
			if(g_b[0][0] == 'X'){
				winner = 1;
				kfree(arguments);
				kfree(message);
				return len;
			}else{
				winner = 2;
				kfree(arguments);
				kfree(message);
				return len;
			}
		}
		// Diagonal Check
		if(g_b[0][0] == g_b[1][1] && g_b[1][1] == g_b[2][2] && g_b[2][2] != '-'){
			if(g_b[0][0] == 'X'){
				winner = 1;
				kfree(arguments);
				kfree(message);
				return len;
			}else{
				winner = 2;
				kfree(arguments);
				kfree(message);
				return len;
			}
		}else if(g_b[0][2] == g_b[1][1] && g_b[1][1] == g_b[2][0] && g_b[2][0] != '-'){
			if(g_b[0][1] == 'X'){
				winner = 1;
				kfree(arguments);
				kfree(message);
				return len;
			}else{
				winner = 2;
				kfree(arguments);
				kfree(message);
				return len;
			}
		}
		
		// Handle AI choice
		while(placed == 0){
			index1 = get_random_int();
			// Generate 1st Index
			if(index1 < 0){
				index1 *= -1;
			}
			index1 = index1 % 3;
			index2 = get_random_int();
			// Generate 2nd Index
			if(index2 < 0){
				index1 *= -1;
			}
			index2 = index2 % 3;
			// Check tile occupancy
			if(g_b[index1][index2] == '-'){
				printk(KERN_INFO "TicTacToe: AI placed at: (%d,%ld)\n", index1, index2);
				placed = 1;
				g_b[index1][index2] = 'O';
			}
		}
		
		
	}else if(strcmp(arguments[0], "RESET") == 0){
		for(x = 0; x < 3; x++){
			for(y = 0; y < 3; y++){
				g_b[x][y] = '-';
			}
		}
		winner = 0;
		printk(KERN_INFO "TicTacToe: Reset Board\n");
	}else{
		printk(KERN_INFO "TicTacToe: NO RESOLUTION: Bad 1st Argument Passed.\n");
	}
	
	// Free Before Return
	kfree(arguments);
	kfree(message);
	return len;
}


/* staict int tictac_release(struct inode *inodep, struct file *filep)
This function will serve as a cleanup function for the module, when it is done being interacted
with from userspace. At the moment it will simply send a message to the kernel detailing that the
kernel has been released. */
static int tictac_release(struct inode *inodep, struct file *filep) {
	printk(KERN_INFO "TicTacToe: Device Driver Has Been Released.\n");
	return 0;
}

// ---- Device Driver Macros ----
module_init(tictac_init);
module_exit(tictac_exit);