#include <linux/init.h>
#include <linux/module.h>

static int __init my_test_init(void) {
  printk("my first kenerl module init\n");
  return 0;
}

static void __exit my_test_exit(void) {
  printk("goodbye\n");
}

module_init(my_test_init);
module_exit(my_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ben Shushu");
MODULE_DESCRIPTION("my test kernel module");
MODULE_ALIAS("mytest");
