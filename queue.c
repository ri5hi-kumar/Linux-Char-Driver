#include <stdio.h>
#include <stdlib.h>

struct data {
	int len;
	char *data;
};

struct queue {
	int size;
	int front;
	int rear;
	struct data *items;
};

struct queue *q = NULL;

void queue_destroy(void)
{
	if(!q) {
		printf("queue: queue is not initialized yet\n");
		return;
	}

	free(q->items);
	q->items = NULL;
	free(q);
	q = NULL;
	printf("queue: queue distroyed\n");
}


void queue_initialize(int size)
{
	q = (struct queue *)malloc(sizeof(struct queue));
	if(!q) {
		printf("queue: error allocating size for struct\n");
		return;
	}
	q->size = size;
	q->front = q->rear = -1;
	q->items = (struct data *)malloc(q->size * sizeof(struct data));
	if(!q->items) {
		printf("queue: error allocating size for array of data\n");
		free(q);
		return;
	}
	printf("queue: queue initalized\n");
}

void queue_push(struct data d)
{
	if(!q) {
		printf("queue: queue is not initialized yet\n");
		return;
	}

	if((q->rear + 1) % q->size == q->front) {
		printf("queue: queue is full\n");
		return;
	}

	q->rear = (q->rear + 1) % q->size;
	q->items[q->rear] = d;

	if(q->front == -1) {
		q->front = q->rear;
	}

	printf("queue: data pushed: %s\n", q->items[q->rear].data);
}

void queue_pop(void)
{
	if(!q) {
		printf("queue: queue is not initialized yet\n");
		return;
	}

	if(q->front == -1 && q->rear == -1) {
		printf("queue: queue is empty\n");
		return;
	}

	printf("queue: data popped: %s\n", q->items[q->front].data);

	if(q->front == q->rear) {
		q->front = q->rear = -1;
		return;
	}

	q->front = (q->front + 1) % q->size;
}

void queue_display(void)
{
	if(!q) {
		printf("queue: queue is not initialized yet\n");
		return;
	}

	if(q->front == -1 && q->rear == -1) {
		printf("queue: queue is empty\n");
		return;
	}

	int front = q->front;
	int rear = q->rear;

	printf("QUEUE: ");
	while(rear >= front) {
		printf("%s ", q->items[rear].data);
		rear--;
	}
	printf("\n");
}

int main(void)
{
	queue_initialize(5);
	struct data d = {4, "xyz"};

	queue_push(d);
	queue_push(d);
	queue_push(d);
	queue_push(d);
	queue_push(d);
	queue_push(d);

	queue_display();

	queue_pop();
	queue_pop();
	queue_pop();
	queue_pop();
	queue_pop();
	queue_pop();

	queue_display();

	return 0;
}
