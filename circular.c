#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rishi");
MODULE_DESCRIPTION("Character Device with Circular Queue");

static int __init my_init(void)
{
    pr_info("Module loaded\n");
    return 0;
}

static void __exit my_exit(void)
{
    pr_info("Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
