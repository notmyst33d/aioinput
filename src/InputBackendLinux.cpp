#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/uinput.h>

#include <iostream>

#include "InputBackendLinux.h"

void uinput_emit(int fd, int type, int code, int value) {
    struct input_event event;

    event.type = type;
    event.code = code;
    event.value = value;
    event.time.tv_sec = 0;
    event.time.tv_usec = 0;

    write(fd, &event, sizeof(event));
}

InputBackendLinux::InputBackendLinux() {
    struct uinput_setup usetup;

    this->fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    ioctl(this->fd, UI_SET_EVBIT, EV_KEY);
    ioctl(this->fd, UI_SET_EVBIT, EV_ABS);
    ioctl(this->fd, UI_SET_EVBIT, EV_SYN);
    ioctl(this->fd, UI_SET_KEYBIT, BTN_DPAD_UP);
    ioctl(this->fd, UI_SET_KEYBIT, BTN_DPAD_DOWN);
    ioctl(this->fd, UI_SET_KEYBIT, BTN_A);
    ioctl(this->fd, UI_SET_ABSBIT, ABS_X);
    ioctl(this->fd, UI_SET_ABSBIT, ABS_Y);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x045e;
    usetup.id.product = 0x0b12;
    usetup.id.version = 0x0507;
    strcpy(usetup.name, "Microsoft Xbox Series S|X Controller");

    ioctl(this->fd, UI_DEV_SETUP, &usetup);
    ioctl(this->fd, UI_DEV_CREATE);
}

InputBackendLinux::~InputBackendLinux() {
    ioctl(this->fd, UI_DEV_DESTROY);
    close(this->fd);
}

void InputBackendLinux::dispatch_event(InputEvent event) {
    switch (this->last_event) {
        case STRUM_UP:
            uinput_emit(this->fd, EV_KEY, BTN_DPAD_UP, 0);
            uinput_emit(this->fd, EV_SYN, SYN_REPORT, 0);
            break;
        case STRUM_DOWN:
            uinput_emit(this->fd, EV_KEY, BTN_DPAD_DOWN, 0);
            uinput_emit(this->fd, EV_SYN, SYN_REPORT, 0);
            break;
        default:
            break;
    }

    switch (event) {
        case STRUM_UP:
            uinput_emit(this->fd, EV_KEY, BTN_DPAD_UP, 1);
            uinput_emit(this->fd, EV_SYN, SYN_REPORT, 0);
            this->last_event = event;
            break;
        case STRUM_DOWN:
            uinput_emit(this->fd, EV_KEY, BTN_DPAD_DOWN, 1);
            uinput_emit(this->fd, EV_SYN, SYN_REPORT, 0);
            this->last_event = event;
            break;
        default:
            break;
    }
}

