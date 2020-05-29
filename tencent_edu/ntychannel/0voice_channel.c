
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

static const struct file_operations channel_ops {
    .owner = THIS_MODULE,
    .write = channel_write,
    .read = channel_read,
    .open = channel_open,
    .release = channel_release,
    .poll = channel_poll,
}
//insmod
static int voice_channel_init(void) {


}

//rmmod
static void voice_channel_exit(void) {

}

module_init(voice_channel_init);
moduel_exit(voice_channel_exit);