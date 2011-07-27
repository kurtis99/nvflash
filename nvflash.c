#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define USB_READ_BUFFER 8192

struct winsize ws;

int debug = 0;

static int
get_console_size(struct winsize* _ws)
{
	int ret;
	
	ret = ioctl(STDIN_FILENO, TIOCGWINSZ, _ws);
	if (ret == -1) {
		perror("TIOCGWINSZ");
		return 1;
	}

	if (debug)
		printf("Current terminal settings:\nColumns: %u\nRows: %u\n", ws.ws_col, ws.ws_row);

	return 0;
}

static void
print_hex(const unsigned char* _buf, const size_t _len)
{
	assert(_len < USB_READ_BUFFER);
	assert(_buf != NULL);

	size_t width;
	/* It takes 10 bytes to print address and each value occupies 3 bytes on screen */
	width = (ws.ws_col - 10) / 3;
	assert(width > 0);
	assert(width < 200);

	printf("HEX dump of input buffer:\n");

	size_t num_printed = 0; /* number of elements already printed on screen */
	while(num_printed < _len) {
		printf("%02X ", *(_buf+num_printed));
		num_printed++;
		if (!(num_printed % width))
			printf(": %08X\n", num_printed);
	}
	size_t i;
	for (i = 0; i < ((width - (num_printed % width)) * 3); i++)
		putchar(' ');
	printf(": %08X\n", num_printed);
}

int main(int argc, char* argv[])
{
	uid_t uid;
	uid = getuid();
	if (uid != 0) { 
		/* Not root */
		fprintf(stderr, "Need to be root\n");
		exit(EXIT_FAILURE);
	}

	if (getenv("TEST_USB_DEBUG") != NULL)
		debug = 1;

	get_console_size(&ws);

	/* USB bus number and USB device number on bus */
	int bus, device;
	if (argc < 2 || argc > 4) {
		/* set default value */
		bus = 1;
		device = 10;
	}
	else {
		bus = atoi(argv[1]);
		device = atoi(argv[2]);
		/* TODO: check for value ranges */
	}

	/* TODO: check if path exist? */
	char fpath[256];
	memset(fpath, '\0', sizeof(fpath));
	snprintf(fpath, sizeof(fpath), "/dev/bus/usb/%03u/%03u", bus, device);

	FILE* fd;
	fd = fopen(fpath, "w+");
	if (fd == NULL) {
		perror(fpath);
		exit(EXIT_FAILURE);
	}
	
	size_t nr;
	unsigned char buf[USB_READ_BUFFER];
	memset(buf, '\0', sizeof(buf));
	nr = fread(buf, 1, sizeof(buf), fd);
	if (nr == 0)
		perror("fread");

	print_hex(buf, nr);

	int ret;
	ret = fclose(fd);
	if (ret != 0)
		perror("flcose");

	return 0;
}
