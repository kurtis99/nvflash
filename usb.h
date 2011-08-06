#ifndef __USB_HEADER
#define __USB_HEADER

#include <libusb-1.0/libusb.h>

#include "nvflash.h"

struct UsbIdent {
	uint16_t idProduct;
	uint16_t idVendor;
};

typedef struct UsbIdent UsbIdent;

extern int usb_initialize();
extern int find_usb_device(struct UsbIdent, libusb_device *);
extern void libusb_perror(int);

#endif /* __USB_HEADER */

