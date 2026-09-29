#include <stdio.h>
#include <stdlib.h>
#include "galactic.h"
#include "galactic/menu.h"
#include "utils/utils.h"

/**
 * @brief The main entry point of the application.
 *
 * @return 0 on successful execution.
 */
int main(void){
    Status stats = INIT;
    char sel = 0;

    while(stats != EXIT_PROGRAM){
        switch (stats)
        {
        case INIT:
            print_menu();
            sel = read_char();
            stats = EXIT_PROGRAM;
            break;
        
        default:
        sel = 0;
            break;
        }
    }

    printf("You selected option %c! Bye bye!\n", sel);

    return 0;
}