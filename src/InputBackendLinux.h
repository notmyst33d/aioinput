#pragma once

#include "InputBackend.h"

class InputBackendLinux : public InputBackend {
public:
    InputBackendLinux();
    ~InputBackendLinux();
    void dispatch_event(InputEvent event);

private:
    int fd;
};
