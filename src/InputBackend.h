#pragma once

typedef enum {
    STRUM_REST = 0,
    STRUM_UP = 1,
    STRUM_DOWN = 2,
} InputEvent;

class InputBackend {
public:
    virtual void dispatch_event(InputEvent event) = 0;

protected:
    InputEvent last_event = STRUM_REST;
};
