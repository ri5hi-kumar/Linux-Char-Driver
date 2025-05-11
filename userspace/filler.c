#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define DRIVER_NAME "/dev/rishi"
#define PUSH_DATA _IOW ('a', 'b', struct data *)

struct data {
    int length;
    char *data;
};

int main(void) {
    int fd = open(DRIVER_NAME, O_RDWR);
    struct data *d = malloc(sizeof(struct data));
    d->length = 4;
    d->data = malloc(4);
    memcpy(d->data, "xyz", 4);
    int ret0 = ioctl(fd, PUSH_DATA, d);
    close(fd);
    free(d->data);
    free(d);
    return 0;
}
