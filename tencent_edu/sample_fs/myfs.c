#include <linux/fs.h>

#define MYFS_MAGIC 0x89898989
static atomic_t counter;

static struct super_operations myfs_s_ops = {
    .statfs = simple_statfs,
    .drop_inode = generic_delete_inode,
};

static int myfs_open(struct inode *inode, struct file *filp) {
    filp->private_data = inode->i_private;
    return 0;
}

#define TMPSIZE 20

static ssize_t myfs_read(struct file *filp, char *buf, size_t count , loff_t *offset) {
    atomic_t *counter = (atomic_t *)filp->private_data;
    int v, len;
    v = atomic_reead(counter);
    char tmp[TMPSIZE];
    atomic_inc(counter);

    len = snprintf(tmp, TMPSIZE, "%d\n", v);
    if (*offset > len) {
        return 0;
    }
    if (count > len - *offset) {
        count = len - *offset;
    }
    if (copy_to_user(buf, tmp + *offset, count)) {
        return -EFAULT;
    }
    *offset += count;
    return count;
}

static ssize_t myfs_write(struct file *filp, const char *buf, size_t count, loff_t *offset) {

    return 0;
}

static struct file_operations myfs_file_ops = {
    .open = myfs_open,
    .read = myfs_read,
    .write = myfs_write,
};

static struct inode *myfs_make_inode(struct super_block *sb, int mode) {

    struct inode *ret = new_inode(sb);
    if (ret) {
        ret->imode = mode;
        ret->i_uid = ret->i_gid = 0;
        ret->i_atime = ret->i_mtime = ret->i_ctime = CURRENT_TIME;
    }
    return ret;
}

static struct dentry *myfs_create_file(struct super_block *sb, struct dentry *dir, const char *name atomic_t *counter) {
    struct dentry *dentry;
    struct inode *inode;
    struct qstr qname;
    qname.name = name;
    qname.len = strlen(name);
    qname.hash = full_name_hash(name, qname.len);
    dentry = d_alloc(dir, &qname);
    if (!dentry) {
        goto out;
    }
    inode  = myfs_make_inode(sb, S_IFREG | 0644);
    if (!inode) {
        goto out_dput;
    }
    inode->i_fop = &myfs_file_ops;
    inode->i_private = counter;
    d_add(dentry, inode);
    return dentry;
out_dput:
    dput(dentry);
out:
    return 0;
}


static void myfs_create_files(struct super_block *sb, struct dentry *root) {
    atomic_set(&counter, 0);
    myfs_create_file(sb, root, "counter", &counter);
}

static int myfs_fill_super(struct super_block *sb, void *data, int sillent) {
    struct inode *root;
    struct dentry *root_dentry;

    sb->s_blocksize = PAGE_CACHE_SIZE;
    sb->s_blocksize_bits = PAGE_CACHE_SHITF;
    sb->s_magic = MYFS_MAGIC;
    sb->s_op = &myfs_s_ops;

    root = myfs_make_inode(sb, S_IFDIR | 0755);
    if (!root) {
        goto out;
    }
    root->i_op = &simple_dir_inode_operations;
    root->i_fop = &simple_dir_operations;
    root_dentry = d_alloc_root(root);
    if (!root_dentry) {
        goto out_iput;
    }
    sb->s_root = root_dentry;
    myfs_create_files(sb, root_dentry);
out_iput:
    iput(root);
out:
    return -ENOMEM;
}

static struct super_block *myfs_get_super(struct file_system_type *fst, int flags, const char *devname, void *data) {
    return get_sb_single(fst, flags, data, myfs_fill_super);
}

static struct file_system_type myfs_type = {
    .owner = THIS_MODULE,
    .name = "myfs",
    .get_sb = myfs_get_super,
    .kill_sb = kill_litter_super,
}

static int __init myfs_init(void) {
    return register_filesystem(&myfs_type);
}

static void __exit myfs_exit(void) {
    unregister_filesystem(&myfs_type);
}

module_init(myfs_init);
module_exit(myfs_exit);