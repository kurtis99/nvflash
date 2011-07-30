#include "nvflash.h"

struct winsize ws;

int debug;

struct UsbIdent NvidiaIdent = {
	.idProduct = NVIDIA_IDPRODUCT,
	.idVendor = NVIDIA_IDVENDOR,
};

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

static int check_permitions()
{
	int r = 0;

	/* Access to low level device functions usually requires root access, so we check if
	 * programm was started as root or not */
	uid_t uid;
	uid = getuid();
	if (uid != 0) 
		r = -1;		/* if not root */

	return r;
}

int main(int argc, char* argv[])
{
	int r;
	libusb_device *dev;

	if (getenv("TEST_USB_DEBUG") != NULL)
		debug = 1;

	r = check_permitions();
	if (r != 0) {
		fprintf(stderr, "Not enought permitions.\n");
		return -1;
	}

	get_console_size(&ws);

	r = usb_initialize();
	if (r != 0) {
		fprintf(stderr, "Cant initialize USB\n");
		return -1;
	}

	/* nVidia device - idVendor:idProduct 0955:7820 */
	find_usb_device(NvidiaIdent, dev);

	return 0;
}
