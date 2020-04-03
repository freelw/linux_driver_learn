#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

dev_t dev;
#define COUNT 1
#define BASEMINOR 0
#define NAME "wangli_dev"

int demo_open(struct inode *inode, struct file *filp)
{
	printk("---%s---%s---%d---\n", __FILE__, __func__, __LINE__);
	return 0;
}

int demo_release(struct inode *inde, struct file *filp)
{
	printk("---%s---%s---%d---\n", __FILE__, __func__, __LINE__);
	return 0;
}

struct cdev *cdevp = NULL;
struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_release,
};
int __init demo_init(void)
{
	int ret = alloc_chrdev_region(&dev, BASEMINOR, COUNT, NAME);
	if (ret < 0) {
		printk(KERN_ERR"alloc_chrdev_region failed...\n");
		goto err1;
	}
	printk(KERN_INFO "major = %d\n", MAJOR(dev));
	cdevp = cdev_alloc();
	if (NULL == cdevp) {
		printk(KERN_ERR"cdev_alloc failed...\n");
		ret = -ENOMEM;
		goto err2;
	}
	cdev_init(cdevp, &fops);
	ret = cdev_add(cdevp, dev, COUNT);
	if (ret < 0) {
		printk(KERN_ERR"cdev_add failed...\n");
		goto err2;
	}
	printk("---%s---%s---%d---\n", __FILE__, __func__, __LINE__);
	return 0;
err2:
	unregister_chrdev_region(dev, COUNT);
err1:
	return ret;
}

void __exit demo_exit(void)
{
	cdev_del(cdevp);
	unregister_chrdev_region(dev, COUNT);
	printk("---%s---%s---%d---\n", __FILE__, __func__, __LINE__);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
