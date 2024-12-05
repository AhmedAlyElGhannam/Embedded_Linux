#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	int fd = open("/sys/class/leds/input3::capslock/brightness", O_RDWR);
	if (argv[1] != NULL)
		// print input argument into fd
		dprintf(fd, "%s\n", argv[1]);
	return 0;
}
