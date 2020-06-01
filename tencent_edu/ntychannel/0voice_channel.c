#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#ifndef NTYCHANNEL_MAJOR
#define NTYCHANNEL_MAJOR 96
#endif

#ifndef NTYCHANNEL_NR_DEVS
#define NTYCHANNEL_NR_DEVS 2
#endif

#ifndef NTYCHANNEL_SIZE
#define NTYCHANNEL_SIZE 4096
#endif

static int channel_major = NTYCHANNEL_MAJOR;
struct cdev cdev;

#define ENABLE_POLL 1

//private_data
struct ntychannel {
    char *data;
    unsigned long size;
#if ENABLE_POLL
    wait_queue_head_t inq;
#endif 
};
#if ENABLE_POLL
int have_data = 0;
#endif 

struct ntychannel *channel_devp;

// write
ssize_t channel_write(struct file * filp, const char __user *buffer, size_t size, loff_t *ppos) {
    // copy buffer to channel private data
    int ret = 0;
    unsigned long p = *ppos;
    unsigned int count = size;
    struct ntychannel *channel = filp->private_data;
    if (p >= NTYCHANNEL_SIZE) {
        return -1;
    }
    if (count + p > NTYCHANNEL_SIZE) {
        count = NTYCHANNEL_SIZE - p;
    }
    if (copy_from_user(channel->data + p, buffer, count)) {
        return -EFAULT;
    } else {
        *ppos += count;
        ret = count;
        channel->size += count;
        *(channel->data + p + count) = '\0';
        printk(KERN_INFO "written %d byte(s) from %ld\n", count, p);
    }
    return ret;
}
// read
ssize_t channel_read(struct file * filp, const char __user *buffer, size_t size, loff_t *ppos) {
    int ret = 0;
    unsigned long p = *ppos;
    unsigned int count = size;
    struct ntychannel *channel = filp->private_data;
    if (p >= NTYCHANNEL_SIZE) {
        return -1;
    }
    if (count + p > NTYCHANNEL_SIZE) {
        count = NTYCHANNEL_SIZE - p;
    }
    if (copy_to_user(buffer, (void *)(channel->data+p), count)) {
        return - EFAULT;
    } else {
        ret = strlen(buffer);
        channel->size -= ret;
        printk(KERN_INFO "read %d byte(s) from %ld\n", ret, p);
    }
    return ret;
}

// open
int channel_open(struct inode *inode, struct file *filp) {
    int num = MINOR(inode->i_rdev);
    if (num >= channel_devp) {
        return -ENODEV;
    }
    filp->private_data = &channel_devp[num];
    return 0;
}

// close
int channel_release(struct inode *inode, struct file *filp) {
    

}
// poll
unsigned int channel_poll(struct file *filp, struct poll_table_struct *wait) {
    
}

// file_operations
static const struct file_operations channel_ops = {
    .owner = THIS_MODULE,
    .write = channel_write,
    .read = channel_read,
    .open = channel_open,
    .release = channel_release,
    .poll = channel_poll,
};

//insmod
static int voice_channel_init(void) {

    int result;
    // register
    dev_t devno = MKDEV(channel_major, 0);
    result = register_chrdev_region(devno, NTYCHANNEL_NR_DEVS, "ntychannel");
    if (result < 0) {
        return result;
    }

    // cdev_init
    cdev_init(&cdev, &channel_ops);

    // cdev_add
    cdev_add(&cdev, devno, NTYCHANNEL_NR_DEVS);

    // malloc private_data
    channel_devp = kmalloc(NTYCHANNEL_NR_DEVS * sizeof(ntychannel), GFP_KERNEL);
    if (!channel_devp) {
        result = -ENOMEM;
        goto fail_malloc:
    }
    for (int i = 0; i < NTYCHANNEL_NR_DEVS; ++ i) {
        channel_devp[i].size = 0;
        channel_devp[i].data = kmalloc(NTYCHANNEL_SIZE, GFP_KERNEL);
        memset(channel_devp[i].data, 0, NTYCHANNEL_SIZE);
    }
    printk(KERN_INFO "ntychannel_init");
    return 0;

fail_malloc:
    unregister_chrdev_region(devno, NTYCHANNEL_NR_DEVS);
    return -1;
}

//rmmod
static void voice_channel_exit(void) {
    for (int i = 0; i < NTYCHANNEL_NR_DEVS; ++ i) {
        kfree(channel_devp[i].data);
    }
    kfree(channel_devp);
    cdev_del(&dev);
    dev_t devno = MKDEV(channel_major, 0);
    unregister_chrdev_region(devno, NTYCHANNEL_NR_DEVS);
    printk(KERN_INFO "channel_release");
}

module_init(voice_channel_init);
moduel_exit(voice_channel_exit);
MODULE_LICENSE("GPL");