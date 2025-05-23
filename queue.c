#include <linux/kernel.h>
#include <linux/slab.h>

#include "common.h"

void queue_destroy(void)
{
	if(!q) {
		pr_err("queue: destroy: queue is not initialized yet\n");
		return;
	}

	if(!(q->front == -1 && q->rear == -1)) {
		int front = q->front;
		int rear = q->rear;
		while(rear >= front) {
			pr_info("queue: distroying: %s\n", q->items[rear].data);
			kfree(q->items[rear].data);
			rear--;
		}
	}

	q->front = q->rear = -1;
	kfree(q->items);
	q->items = NULL;
	kfree(q);
	q = NULL;
	pr_info("queue: queue distroyed\n");
}


void queue_initialize(int size)
{
	if(size <= 0) {
		pr_err("queue: error, size is negative\n");
		return;
	}

	queue_destroy();

	q = (struct queue *)kmalloc(sizeof(struct queue), GFP_KERNEL);
	if(!q) {
		pr_err("queue: error allocating size for struct\n");
		return;
	}
	q->size = size;
	q->front = q->rear = -1;
	q->items = (struct data *)kmalloc(q->size * sizeof(struct data), GFP_KERNEL);
	if(!q->items) {
		pr_err("queue: error allocating size for array of data\n");
		kfree(q);
		return;
	}
	pr_info("queue: queue initalized with size: %d\n", q->size);
}

void queue_push(struct data d)
{
	if(!q) {
		pr_err("queue: push: queue is not initialized yet\n");
		return;
	}

	if((q->rear + 1) % q->size == q->front) {
		pr_err("queue: queue is full\n");
		return;
	}

	q->rear = (q->rear + 1) % q->size;
	q->items[q->rear] = d;

	if(q->front == -1) {
		q->front = q->rear;
	}

	pr_info("queue: data pushed: %s\n", q->items[q->rear].data);
}

struct data *queue_pop(void)
{
	if(!q) {
		pr_info("queue: pop: queue is not initialized yet\n");
		return NULL;
	}

	if(q->front == -1 && q->rear == -1) {
		pr_err("queue: queue is empty\n");
		return NULL;
	}

	struct data *d = &q->items[q->front];

	pr_info("queue: data popped: %s\n", q->items[q->front].data);

	if(q->front == q->rear) {
		q->front = q->rear = -1;
		return d;
	}

	q->front = (q->front + 1) % q->size;
	return d;
}

void queue_display(void)
{
	if(!q) {
		pr_err("queue: display: queue is not initialized yet\n");
		return;
	}

	if(q->front == -1 && q->rear == -1) {
		pr_err("queue: queue is empty\n");
		return;
	}

	int front = q->front;
	int rear = q->rear;

	pr_info("QUEUE: ");
	while(rear >= front) {
		pr_info("%s ", q->items[rear].data);
		rear--;
	}
	pr_info("\n");
}
