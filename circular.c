#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rishi");
MODULE_DESCRIPTION("Character Device with Circular Queue");

static ssize_t my_read(struct file *f, char __user *u, size_t, loff_t *o)
{
	pr_info("circular_chrdev: Read operation performed\n");
	return 0;
}

static int my_open(struct inode *inode, struct file *f)
{
	pr_info("circular_chrdev: Open operation performed\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *f)
{
	pr_info("circular_chrdev: Release operation performed\n");
	return 0;
}

static struct file_operations fops = {
	.open = my_open,
	.read = my_read,
	.release = my_release
};

static int major;

static int __init my_init(void)
{
	major = register_chrdev(major, "circular_chrdev", &fops);
	if(major < 0) {
		pr_err("circular_chrdev: Unable to get major number\n");
		return 0;
	}
	pr_info("circular_chrdev: Major number for device: %d\n", major);
	return 0;
}

static void __exit my_exit(void)
{
	unregister_chrdev(major, "circular_chrdev");
	pr_info("Goodbye Cruel World\n");
}

module_init(my_init);
module_exit(my_exit);
