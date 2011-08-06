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
	struct libusb_config_descriptor *config;
	if (getenv("TEST_USB_DEBUG") != NULL)
		debug = 1;

	r = check_permitions();
	if (r != 0) {
		fprintf(stderr, "Not enought permitions.\n");
		return -1;
	}

	get_console_size(&ws);

	r = libusb_init(NULL);
	if (r != 0) {
		libusb_perror(r);
		return r;
	}

	/* nVidia device - idVendor:idProduct 0955:7820 */
	find_usb_device(NvidiaIdent, dev);

	r = libusb_get_active_config_descriptor(dev, &config);
	if (r != 0) {
		fprintf(stderr, "Cant get USB config.\n");
		return r;
	}

	int i;
	if (config->bNumInterfaces > 1) {
		fprintf(stderr, "Huita.\n");
		return -1;
	}
	if (config->interface->num_altsetting == 0) {
		fprintf(stderr, "check_usb_interface(): No interface settings\n");
		return -1;
	}
	if (config->interface[0].num_altsetting > 1) {
		fprintf(stderr, "check_usb_interface(): No interface settings\n");
		return -1;
	}

	uint8_t in_endpoint;
	uint8_t out_endpoint;
	/* Find endpoint to send data */
	for (i=config->interface[0].altsetting[0].bNumEndpoints; i; i--) {
		uint8_t addr = config->interface[0].altsetting[0].endpoint[i].bEndpointAddress;
		if (addr & LIBUSB_ENDPOINT_IN)
			in_endpoint = addr;
		else
			out_endpoint = addr;
	}

	libusb_device_handle *handle;
	r = libusb_open(dev, &handle);
	if (r != 0) {
		fprintf(stderr, "Cant USB for read\n");
		return -1;
	}

	r = libusb_claim_interface(handle, 0) /* Is there a single interface??? */;
	if (r != 0) {
		fprintf(stderr, "Cant claim USB interface.\n");
		return r;
	}

	unsigned char buf[4096];
	int transfered;
	memset(buf, '\0', sizeof(buf));
	r = libusb_bulk_transfer(handle, in_endpoint, buf, sizeof(buf), &transfered, 0);
	if (r != 0) {
		fprintf(stderr, "Cant read from USB\n");
		return -1;
	}

	libusb_close(handle);
	libusb_exit(NULL);

	return 0;
}
