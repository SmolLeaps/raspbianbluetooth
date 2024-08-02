#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

void print_key(uint8_t keycode) {
    // This is a simplified example; you may need to handle more keycodes and modifiers
    switch (keycode) {
        case 4: printf("A\n"); break;
        case 5: printf("B\n"); break;
        case 6: printf("C\n"); break;
        case 7: printf("D\n"); break;
        case 9: printf("E\n"); break;
        case 10: printf("G\n"); break;
        case 11: printf("H\n"); break;
        case 12: printf("I\n"); break;
        case 13: printf("J\n"); break;
        case 14: printf("K\n"); break;
        case 15: printf("L\n"); break;
        case 16: printf("M\n"); break;
        case 17: printf("N\n"); break;
        case 18: printf("O\n"); break;
        case 19: printf("P\n"); break;
        case 20: printf("Q\n"); break;
        case 21: printf("R\n"); break;
        case 22: printf("S\n"); break;
        case 23: printf("T\n"); break;
        case 24: printf("U\n"); break;
        case 25: printf("V\n"); break;
        case 26: printf("W\n"); break;
        case 27: printf("X\n"); break;
        case 28: printf("Y\n"); break;
        case 29: printf("Z\n"); break;

        // Add more keycodes as needed
        default: printf("Unknown keycode: %d\n", keycode);
    }
}

void check_device(libusb_device *device) {

    printf("\n ---entered check_device() --- \n");
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(device, &desc);
    if (r < 0) {
        fprintf(stderr, "Failed to get device descriptor: %s\n", libusb_error_name(r));
        return;
    }

    if (desc.bDeviceClass == LIBUSB_CLASS_PER_INTERFACE) {
        libusb_config_descriptor *config;
        r = libusb_get_config_descriptor(device, 0, &config);
        if (r < 0) {
            fprintf(stderr, "Failed to get config descriptor: %s\n", libusb_error_name(r));
            return;
        }

        const libusb_interface *inter;
        const libusb_interface_descriptor *interdesc;
        for (int i = 0; i < config->bNumInterfaces; i++) {
            inter = &config->interface[i];
            for (int j = 0; j < inter->num_altsetting; j++) {
                interdesc = &inter->altsetting[j];
                if (interdesc->bInterfaceClass == LIBUSB_CLASS_HID &&
                    interdesc->bInterfaceSubClass == 1 && // Boot Interface Subclass
                    interdesc->bInterfaceProtocol == 1) { // Keyboard
                    printf("Keyboard found: VID=0x%04x, PID=0x%04x\n", desc.idVendor, desc.idProduct);
                }
                for (int k = 0; k < interdesc->bNumEndpoints; k++) {
                    const libusb_endpoint_descriptor *epdesc = &interdesc->endpoint[k];
                    printf("Endpoint address: 0x%02x\n", epdesc->bEndpointAddress);
                }
            }
        }

        libusb_free_config_descriptor(config);
    }
        printf("\n --- exit check_device() --- \n");
}

int main() {
    printf("init\n");
    libusb_device **devs;
    libusb_context *ctx = NULL;
    int r;
    ssize_t cnt;

    r = libusb_init(&ctx);
    if (r < 0) {
        fprintf(stderr, "Init Error %d\n", r);
        return 1;
    }

    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        fprintf(stderr, "Get Device Error\n");
        libusb_exit(ctx);
        return 1;
    }

    //logic for calling check_device()
       for (ssize_t i = 0; i < cnt; i++) {
        check_device(devs[i]);
    }
    // printf("breakpoint 1 | \n");
    libusb_device *keyboard = NULL;
    // printf("breakpoint 2 | \n");
    struct libusb_device_descriptor desc;
    // code to check for specific keyboards connected
    for (ssize_t i = 0; i < cnt; i++) {
        libusb_get_device_descriptor(devs[i], &desc);
        if (desc.idVendor == 0x0c45 && desc.idProduct == 0x7018) { // TODO: receive dynamic values for the found keyboard from check_device()
            // printf("breakpoint 3 | \n");
            keyboard = devs[i];
            break;
        }
    }

    if (keyboard == NULL) {
        fprintf(stderr, "Keyboard not found\n");
        libusb_free_device_list(devs, 1);
        libusb_exit(ctx);
        return 1;
    }

    // printf("breakpoint 4 | \n");
    libusb_device_handle *handle;
    // printf("breakpoint 5 | \n");
    r = libusb_open(keyboard, &handle);
    // printf("breakpoint 6 | \n");
    if (r != 0) {
        fprintf(stderr, "Cannot open device: %s\n", libusb_error_name(r));
        libusb_free_device_list(devs, 1);
        libusb_exit(ctx);
        return 1;
    }
    // printf("breakpoint 7 | \n");
    libusb_free_device_list(devs, 1);
    // Detach the kernel driver if necessary
    if (libusb_kernel_driver_active(handle, 0) == 1) {
        printf("Kernel driver active. Detaching it.\n");
        r = libusb_detach_kernel_driver(handle, 0);
        if (r != 0) {
            fprintf(stderr, "Cannot detach kernel driver: %s\n", libusb_error_name(r));
            libusb_close(handle);
            libusb_exit(ctx);
            return 1;
        }
    }
    // printf("breakpoint 8 | \n");  
    r = libusb_claim_interface(handle, 0);
    if (r != 0) {
        fprintf(stderr, "Cannot claim interface: %s\n", libusb_error_name(r));
        libusb_close(handle);
        libusb_exit(ctx);
        return 1;
    }

    uint8_t data[8];
    int actual_length;

    while (1) {
        r = libusb_interrupt_transfer(handle, 0x81, data, sizeof(data), &actual_length, 5000);
        // printf("breakpoint 9 | \n", r);
        // printf("variables in breakpoint 9: %s | %u \n", handle, data);
        // char str[20];  // Make sure the array is large enough to hold the number as a string
        // sprintf(str, "%u", handle); // Convert the unsigned int to a string
        // printf("The string representation of the unsigned integer is: %s\n", str);
        if (r == 0 && actual_length == sizeof(data)) {
            // printf("breakpoint 10  | \n");
            for (int i = 2; i < actual_length; i++) {
                // printf("breakpoint 11 | \n");
                if (data[i] != 0) {
                    // printf("breakpoint 12 | \n");
                    print_key(data[i]);
                }
            }
        } else if (r != LIBUSB_ERROR_TIMEOUT) {
            fprintf(stderr, "Error in interrupt transfer: %s\n", libusb_error_name(r));
        }
        else {
            fprintf(stderr, "Error receiving data: %s\n", libusb_error_name(r));
        }
    }

    libusb_release_interface(handle, 0);
    libusb_close(handle);
    libusb_exit(ctx);

    return 0;
}
