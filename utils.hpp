#ifndef UTILS_H
#define UTILS_H

#include <errno.h>

#define DIE(assertion, call_description)                       \
    do                                                         \
    {                                                          \
        if (assertion)                                         \
        {                                                      \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
            perror(call_description);                          \
            exit(errno);                                       \
        }                                                      \
    } while (0)

#define HOST "34.246.184.49"
#define PORT 8080
#define COMMAND_SIZE 64
#define USERNAME_SIZE 64
#define PASSWORD_SIZE 64
#define COOKIE_SIZE 1024
#define FIELD_SIZE 64

#define EXIT_COMMAND "exit"
#define REGISTER_COMMAND "register"
#define LOGIN_COMMAND "login"
#define ENTER_COMMAND "enter_library"
#define GET_ALL_COMMAND "get_books"
#define GET_COMMAND "get_book"
#define ADD_COMMAND "add_book"
#define DELETE_COMMAND "delete_book"
#define LOGOUT_COMMAND "logout"

#define SPACE_INDENT 4

#endif