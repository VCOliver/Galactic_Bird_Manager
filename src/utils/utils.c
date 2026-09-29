#include "utils/utils.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int read_char(void) {
    // Fallback to normal getchar if input is not coming from a real terminal (e.g., piped from a file)
    if (!isatty(STDIN_FILENO)) {
        return getchar();
    }

    struct termios oldt, newt;
    int ch;

    // Securely get current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical (line) mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);

    // Apply new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    // Restore old settings immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}
