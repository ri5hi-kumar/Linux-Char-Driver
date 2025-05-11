#ifndef COMMON_H
#define COMMON_H

#define SET_SIZE_OF_QUEUE _IOW ('a', 'a', int *)
#define PUSH_DATA _IOW ('a', 'b', struct data *)
#define POP_DATA _IOR ('a', 'c', struct data *)

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

extern struct queue *q;

void queue_destroy(void);
void queue_initialize(int);
void queue_push(struct data);
struct data *queue_pop(void);
void queue_display(void);

#endif
