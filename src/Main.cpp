#include <iostream>

#include <asio2/serial_port/serial_port.hpp>
#include <asio2/version.hpp>

#include "InputBackendPlatform.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: aioinput [serial device]\n";
        return 1;
    }

    asio2::serial_port port;
    InputBackendPlatform input;
    bool flushed = false;

    port.bind_start([&]() {
        if (asio2::last_error_val() != 0) {
            std::cout << asio2::last_error_msg() << "\n";
            port.stop();
            return;
        }
        std::cout << "Connected\n";
    });

    port.bind_recv([&](std::string_view data) {
        if (!flushed) {
            flushed = true;
            return;
        }
        for (const int event : data)
            input.dispatch_event((InputEvent)event);
    });

    port.start(argv[1], 9600);

    std::cout << "Type \"q\" and press Enter to exit\n";
    while (std::getchar() != 'q') {}

    port.stop();

    return 0;
}
