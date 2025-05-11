#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/wait.h>

#include "common.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rishi");
MODULE_DESCRIPTION("Character Device with Circular Queue");

static int major;
struct queue *q = NULL;

DECLARE_WAIT_QUEUE_HEAD(queue_wait);

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
	struct data d;
	struct data *p;
	switch(cmd) {
	case SET_SIZE_OF_QUEUE:
		if(copy_from_user(&size, (int *)arg, sizeof(int))) {
				pr_err("circular_chrdev: Error copying size from user\n");
				return -EFAULT;
		}
		queue_initialize(size);
		if(!q) {
			pr_err("circular_chrdev: error initializing queue\n");
			return -EFAULT;
		}

		pr_info("circular_chrdev: queue size set: %d\n", q->size);
		queue_display();
		break;

	case PUSH_DATA:
		if(copy_from_user(&d, (struct data *)arg, sizeof(struct data))) {
				pr_err("circular_chrdev: Error copying struct from user\n");
				return -EFAULT;
		}

		char *kbuf = (char *)kmalloc(d.len * sizeof(char), GFP_KERNEL);
		if(!kbuf) {
			pr_err("circular_chrdev: Error allocating memory for kbuf\n");
			return -EFAULT;
		}

		if(copy_from_user(kbuf, ((struct data *)arg)->data, d.len)) {
				pr_err("circular_chrdev: Error copying struct from user\n");
				return -EFAULT;
		}

		d.data = kbuf;
		queue_push(d);
		wake_up_interruptible(&queue_wait);
		if(q) {
			pr_info("circular_chrdev: data pushed: %s\n", q->items[q->rear].data);
		}

		queue_display();
		break;

	case POP_DATA:
		if(q) {
			if(q->front == -1 && q->rear == -1) {
				wait_event_interruptible(queue_wait, q->rear != -1);
			}
		}

		p = queue_pop();
		if(!p) {
			pr_err("circular_chrdev: Issue in popping data\n");
			return -EFAULT;
		}

		if(copy_from_user(&d, (struct data *)arg, sizeof(struct data))) {
				pr_err("circular_chrdev: Error copying struct from user\n");
				return -EFAULT;
		}

		if(copy_to_user(d.data, p->data, d.len)) {
			pr_err("circular_chrdev: Error copying struct to user\n");
			return -EFAULT;
		}
		queue_display();
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
