//
// Created by Thomas Borbeli on 30.04.2022.
//

#ifndef WISH_SHELL_WISH_H
#define WISH_SHELL_WISH_H

typedef struct command {
    char * instruction;
    char *args[];
} command;

#endif //WISH_SHELL_WISH_H
