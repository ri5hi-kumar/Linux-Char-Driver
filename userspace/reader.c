#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DRIVER_NAME "/dev/rishi"
#define POP_DATA _IOR ('a', 'c', struct data *)

struct data {
    int length;
    char *data;
};

int main(void) {
    int fd = open(DRIVER_NAME, O_RDWR);
    struct data *d = malloc(sizeof(struct data));
    d->length = 4;
    d->data = malloc(4);
    int ret = ioctl(fd, POP_DATA, d);
    printf("%s\n", d->data);
    close(fd);
    free(d->data);
    free(d);
    return ret;
}
