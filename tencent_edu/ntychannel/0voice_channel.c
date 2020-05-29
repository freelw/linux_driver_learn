#include <linux/fs.h>

#ifndef NTYCHANNEL_MAJOR
#define NTYCHANNEL_MAJOR 96
#endif

#ifndef NTYCHANNEL_NR_DEVS
#define NTYCHANNEL_NR_DEVS 2
#endif

static int channel_major = NTYCHANNEL_MAJOR;

struct cdev cdev;

//private_data
struct ntychannel {
    char *data;
    unsigned long size;
};

// write
ssize_t channel_write(struct file *, const char __user *, size_t, loff_t *) {

}
// read
ssize_t channel_read(struct file *, char __user *, size_t, loff_t *) {

}

// open
int channel_open(struct inode *, struct file *) {

}
// close
int channel_release(struct inode *, struct file *) {

}
// poll
unsigned int channel_poll(struct file *, struct poll_table_struct *) {

}

// file_operations
static const struct file_operations channel_ops = {
    .owner = THIS_MODULE,
    .write = channel_write,
    .read = channel_read,
    .open = channel_open,
    .release = channel_release,
    .poll = channel_poll,
}

//insmod
static int voice_channel_init(void) {

    int result;
    // register
    dev_t devno = MKDEV(channel_major, 0);
    result = register_chrdev_region(devno, NTYCHANNEL_NR_DEVS, "ntychannel");
    if (result < 0) {
        return result;
    }

    cdev_init();

    // cdev_init

    // cdev_add

    // malloc private_data

}

//rmmod
static void voice_channel_exit(void) {

}

module_init(voice_channel_init);
moduel_exit(voice_channel_exit);
MODULE_LICENSE("GPL");