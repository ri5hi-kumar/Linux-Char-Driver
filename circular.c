#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>

#include "common.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rishi");
MODULE_DESCRIPTION("Character Device with Circular Queue");

static int major;
struct queue *q = NULL;

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

static long my_unlocked_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	int size;
	switch(cmd) {
	case SET_SIZE_OF_QUEUE:
		if(copy_from_user(&size, (int *)arg, sizeof(int))) {
				pr_err("circular_chrdev: Error copying from user\n");
				return -EFAULT;
		}
		queue_initialize(size);

		pr_info("circular_chrdev: queue size set: %d\n", q->size);
		break;

	default:
		pr_info("circular_chrdev: Wrong command\n");
	}

	return 0;
}

static struct file_operations fops = {
	.open = my_open,
	.read = my_read,
	.release = my_release,
	.unlocked_ioctl = my_unlocked_ioctl
};

static int __init my_init(void)
{
	queue_initialize(0);
	major = register_chrdev(major, "circular_chrdev", &fops);
	if(major < 0) {
		pr_err("circular_chrdev: Unable to get major number\n");
		return 0;
	} pr_info("circular_chrdev: Major number for device: %d\n", major);
	return 0;
}

static void __exit my_exit(void)
{
	queue_destroy();
	unregister_chrdev(major, "circular_chrdev");
	pr_info("Goodbye Cruel World\n");
}

module_init(my_init);
module_exit(my_exit);
