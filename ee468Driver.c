#include <linux/init.h>
//#include <linux/config.h>  // see http://askubuntu.com/a/295142
//#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/kernel.h> 	/* printk() */
#include <linux/slab.h> 	/* kmalloc() */
#include <linux/fs.h> 		/* everything... */
#include <linux/errno.h> 	/* error codes */
#include <linux/types.h> 	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> 	/* O_ACCMODE */
//#include <asm/system.h> 	/* cli(), *_flags */  see http://superuser.com/a/783289
#include <asm/uaccess.h> 	/* copy_from/to_user */

#define MODNAME "ee468Driver"

MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of memory.c functions */
int memory_open(struct inode *inode, struct file *filp);
int memory_release(struct inode *inode, struct file *filp);
ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t memory_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);
void memory_exit(void);
int memory_init(void);

/* Structure that declares the usual file */
/* access functions */
struct file_operations memory_fops = {
    read: memory_read,
    write: memory_write,
    open: memory_open,
    release: memory_release
};

/* Declaration of the init and exit functions */
module_init(memory_init);
module_exit(memory_exit);
/* Global variables of the driver */
/* Major number */
const int memory_major = 60;
/* Buffer to store data */
const int STACK_SIZE = 10;
char *memstack;
int g_readPos = 0;  // index for next read fop



/**
 * Memory Init Module
 */
int memory_init(void) {
    int result;
    /* Registering device */
    /* to make a corresponding device node use console command $mknod /dev/<MODNAME> c 60 0 */
    result = register_chrdev(memory_major, MODNAME, &memory_fops);
    if (result < 0) {
        printk("<1>memory: cannot obtain major number %d\n", memory_major);
        return result;
    }

    /* Allocating memory for the buffer */
    memstack = kmalloc(1, GFP_KERNEL);
    if (!memstack) {
        result = -ENOMEM;
        goto fail;
    }
    memset(memstack, 0, STACK_SIZE);
    printk("<1>Inserting %s module\n", MODNAME);
    return 0;

fail:
    memory_exit();
    return result;
}



/**
 * Memory Exit Module
 */
void memory_exit(void) {
    /* Freeing the major number */
    unregister_chrdev(memory_major, MODNAME);

    /* Freeing buffer memory */
    if (memstack) {
        kfree(memstack);
    }
    printk("<1>Removing %s module\n", MODNAME);
}



/**
 * Memory Open
 */
int memory_open(struct inode *inode, struct file *filp) {
    /* Success */
    return 0;
}



/**
 * Memory Release
 */
int memory_release(struct inode *inode, struct file *filp) {
    /* Success */
    return 0;
}



/**
 * Memory Read
 *
 * filp = file struct ptr. (to write TO)
 * buf = userspace buffer to write FROM
 * count = # of bytes to transfer
 * f_pos = position of where to start writing in this file
 */
ssize_t memory_read(struct file *filp, char *buf,
                    size_t count, loff_t *f_pos) {
    printk("ee468Device: memory_read: entered\n");

    // resize transfer size if needed
    count = (count > STACK_SIZE) ? STACK_SIZE: count;

    int transfered = 0;
    while (count && g_readPos >= 0/*(memstack[g_readPos] != 0)*/)
    {
        // see https://www.kernel.org/doc/htmldocs/kernel-hacking/routines-copy.html
        printk("memory_read: memstack[%d] = %c\n", g_readPos, memstack[g_readPos]);
        put_user(memstack[g_readPos], buf++);

        transfered++;

        count--;
        g_readPos--;
    }
    g_readPos = (g_readPos < 0) ? 0: g_readPos+1;  // to allow proper insertion on next write

    printk("memory_read: memstack = %s, g_readPos = %d\n", memstack, g_readPos);
    return transfered;
}



/**
 * Memory Write
 */
ssize_t memory_write(struct file *filp,  char *buf,
                     size_t      count,  loff_t *f_pos) {
    printk("ee468Device: memory_write: entered\n");
    printk("memory_write: buf = %s, count = %d\n", buf, count);

    // initial check that buf contains any valid char
    char valid[] = "abcdefghijklmnopqrstuvwxyz";
    char *match = strpbrk(buf, valid);  // see http://www.cplusplus.com/reference/cstring/strpbrk/

    // write all valid chars from buf to memstack until full
    while (match != NULL && count != 0){
        if (g_readPos < STACK_SIZE){
            printk("Match is %c\n",*match);
            printk("g_readPos is %d\n",g_readPos);

            memstack[g_readPos] = *match;
            match = strpbrk(match+1, valid);

            g_readPos++;
            count--;
        }
        else{
            printk("BUFFER OVERFLOW!\n");
            g_readPos = STACK_SIZE - 1;  // so can still read from very top of overflown stack
            return 1;
        }
    }

    g_readPos--;
    printk("memory_write: memstack = %s, g_readPos = %d\n", memstack, g_readPos);
    return g_readPos;
}
























