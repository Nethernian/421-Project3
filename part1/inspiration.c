// File : inspiration.c
// PROJ : CMSC 421 - Project 3
// Name : Nathan Wooddell - TC56612
// Desc : This will be C code for the character device driver as described in the inspiration.c 
// prompt from project 3. This must be called from /dev/inspiration.


// ---- DEVICE INCLUDE BLOCK ----
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/random.h>
#include <linux/errno.h>


// ---- DEVICE DEFINE BLOCK ----
#define DEVICE_NAME "inspiration"
#define CLASS_NAME "inspoClass"


// ---- DEVICE INFO & DEFS ----

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nathan Wooddell");
MODULE_DESCRIPTION("Project 3 - This should Inspire you!");
MODULE_VERSION("0.0.1");


// ---- DEVICE VARIABLES ----
static int MAJOR;											// Holds the Major Number of the Device.
static struct class* InspoClass = NULL;						// Device-Driver class struct pointer.
static struct device* InspoDevice = NULL;					// Device-Driver Device struct pointer.
static int random_int;										// A random integer used to find the appropriate quote.
static const char *random_str = NULL;						// A buffer to hold the quote.
static int size_of_random;									// The size of the random quote.


// ---- DEVICE FUNCTION DECLARATIONS ----
static int open_inspiration(struct inode *, struct file *);
static int release_inspiration(struct inode *, struct file *);
static ssize_t read_inspiration(struct file *, char *, size_t, loff_t *);
static ssize_t write_inspiration(struct file *, const char *, size_t, loff_t *);


// ---- QUOTE ARRAY DEFINITION ----
static const char *quotes[] = {
		"\"I came, I saw, I conquered\" - Julius Caesar \n",
		"\"Los Mexicanos sin perros!\" - Michael\n",
		"\"I do not fear computers. I fear the lack of them.\" - Asimov\n",
		"\"Computer Science is no more about computers than astronomy is about telescopes.\" - Dijkstra\n",
		"\"Standards are always out of date. That's what makes them standards\" - Alan Bennett\n",
		"\"The more you know, the more you realize you know nothing.\" - Socrates\n",
		"\"Where is the 'any' key?\" - Homer (Simpson - not the other one)\n",
		"\"There is only one problem with common sense; it's not very common.\" - Milt Bryce\n",
		"\"No man has the right to be an amateur in the matter of physical training.\" - Socrates\n",
		"\"All war is a symptom of man's failure as a thinking animal.\" - John Steinbeck\n",
		"\"I think it's a new feature. Don't tell anyone it was an accident.\" - Larry Wall\n",
		"\"The problem of viruses is temorary and will be solved in two years.\" - John McAfee\n",
		"\"All we have to decide is what to do with the time that is given to us.\" - J.R.R. Tolkien\n",
		"\"Faithless is he that says farewell when the road darkens.\" - J.R.R. Tolkien\n",
		"\"I will not say: do not weep; for not all tear are an evil.\" - J.R.R. Tolkien\n",
		"\"Most men would rather deny a hard truth than face it.\" - George R.R. Martin\n",
		"\"Death is so terribly final, while life is full of possibilities.\" - George R.R. Martin\n",
		"\"In the midst of chaos, there is also opportunity.\" - Sun-Tzu\n",
		"\"The greatest victory is that which requires no battle.\" - Sun-Tzu\n",
		"\"Even the finest sword plunged into salt water will eventually rust.\" - Sun-Tzu\n"
};


// ---- DEVICE DATA STRUCTURES ----

/* const struct file_operations operations
This is the file operation data structure. This is needed for the device to be recognized
by the kernel. Implements open, read, write, and release system calls. */
const struct file_operations operations = {
		.owner = THIS_MODULE,
		.open = open_inspiration,							// Activated on file open
		.read = read_inspiration,							// Activated on file read
		.write = write_inspiration,							// Activated on file write
		.release = release_inspiration,						// Activated on file release
};


// ---- DEVICE FUNCTION DEFINITIONS ----

/* static int __init init_inspiration(void)
A function to create initialize the inspiration module. This function should be used only at 
initialization of the inspiration module. This makes use of the __init macro to only be run at
module initialization. */
static int __init init_inspiration(void) {
	// This will attempt to dynamically allocate a Major number
	MAJOR = register_chrdev(0, DEVICE_NAME, &operations);
	
	// Handle Negative Major Number
	if(MAJOR < 0){
		printk(KERN_ALERT "Inspiration: Module Failed To Register Major Number.\n");
		return MAJOR;
	}
	printk(KERN_INFO "Inpsiration: Module MAJOR allocated with value: %d\n", MAJOR);
	
	// Register the Class for the Device
	InspoClass = class_create(THIS_MODULE, CLASS_NAME);
	// IMPLEMENT ERROR CHECKING
	printk(KERN_INFO "Inspiration: Created Class for Device Successfully.\n");
	
	// Register the Driver for the Device
	InspoDevice = device_create(InspoClass, NULL, MKDEV(MAJOR, 0), NULL, DEVICE_NAME);
	// IMPLEMENT ERROR CHECKING
	printk(KERN_INFO "Inspiration: Created Driver for Device Successfully.\n");
	
	return 0;
}


/* staic void __exit exit_inspiration(void)
A module function to handle cleanup and exit the module. This function makes use of the __exit
macro, and will only be run during module deactivation (rmmod). */
static void __exit exit_inspiration(void) {
	device_destroy(InspoClass, MKDEV(MAJOR, 0));
	class_unregister(InspoClass);
	class_destroy(InspoClass);
	unregister_chrdev(MAJOR, DEVICE_NAME);
	printk(KERN_INFO "Inspiration: Module Deactivated\n");
}


/* static int open_inspiration(struct inode *inodep, struct file *filep)
This function will generate a random integer which will be used to determine which quote from the
predefined array will be returned as the device driver is read. This value will be stored in a
global variable, so it may be accessed from the dev_read function. */
static int open_inspiration(struct inode *inodep, struct file *filep) {
	random_int = 0;
	random_int = get_random_int();
	// Check for negative number
	if(random_int < 0){
		random_int = random_int * -1;
	}
	random_int = random_int % 20;
	// Return info for DMESG
	printk(KERN_INFO "Inspiration: Random Number : %d\n", random_int);
	return 0;
}


/* static ssize_t read_inspiration(struct file *filep, char *buffer, size_t len, loff_t *offset)
This function will handle when the device driver is read from. This should handle data being sent
to the user through the use of the copy_to_user() function. */
static ssize_t read_inspiration(struct file *filep, char *buffer, size_t len, loff_t *offset) {
	int error_code;
	int dest_test;
	
	dest_test = access_ok(&buffer,len);
	error_code = 0;
	// Making sure the user buffer can be written to.
	if(dest_test != 0){
		printk(KERN_ALERT "Inspiration: -EFAULT at Source");
		return -EFAULT;
	}
	
	// Gather the Quote
	random_str = quotes[random_int];
	size_of_random = strlen(random_str);
	
	// This should move the string from kernel to the buffer - Currently Doesn't and I don't know why.
	//error_code = copy_to_user(buffer, random_str, size_of_random); // ?? WHY DOES THIS NOT WORK!?	
	
	// Ultimately this is not the desired method to do this, but it seems to work without issue.
	/*return (size_of_random = 0); <-- I have tried a bunch of ways to get this to work, and 
	ultimately defaulted to what I have below.*/
	return simple_read_from_buffer(buffer, len, offset, random_str, size_of_random);
}

/* static ssize_t write_inspiration(struct file *filep, const char *buffer, size_t len, loff_t *offset)
This function simply exists to prevent the user from writing to this device, as this device is not
permitted to be read from. */
static ssize_t write_inspiration(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
	// Return EPERM error.
	printk(KERN_INFO "Inspiration: Cannot Write to this Device. \n");
	return -EPERM;
}


/* static int release_inspiration(struct inode *inodep, struct file *filep)
This device will handle releasing the device module when it is called by a userspace program. */
static int release_inspiration(struct inode *inodep, struct file *filep) {
	printk(KERN_INFO "Inspiration: The Device Has Been Released.\n");
	return 0;
}


// ---- DEVICE MACRO DECLARATIONS ----
module_init(init_inspiration);
module_exit(exit_inspiration);