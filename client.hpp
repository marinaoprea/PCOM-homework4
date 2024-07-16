#ifndef CLIENT_HPP
#define CLIENT_HPP

/**
 * Parses error from server in json format; function exctracts json object,
 * parses it and prints error to stdout
*/
void parse_error(char *response);

/**
 * function reads input for username and password from stdin;
 * function return 0 if credentials are valid and -1 otherwise;
 * function populates err_msg with corresponding error message
*/
int get_credentials(char *username, char *password, char *error_msg);

/**
 * function reads book fields from stdin;
 * function return 0 if book fields are valid and -1 otherwise;
 * function populates err_msg with corresponding error message
*/
int fill_book_fields(char *title, char *author, char *genre, char *publisher,
                     char *page_count, int &count, char *err_msg);

/**
 * function reads book id from stdin and prints error to stdout in case format
 * is invalid
*/
void get_book_id(char *id);

/**
 * performs registration of new user
*/
void register_command(int sockfd);

/**
 * performs login of user; saves session cookie
*/
void login_command(int sockfd, char **session_cookie);

/**
 * performs library access for user based on session cookie; function saves
 * jwt access token
*/
void enter_library_command(int sockfd, char *session_cookie, char **token);

/**
 * performs getting information about all books in library; function parses 
 * returned json and dumps content at stdout
*/
void get_all_command(int sockfd, char *token);

/**
 * performs addition of new book to library; function constructs json object
 * and then deserializes it in form of string and sends it as data in request
*/
void add_book_command(int sockfd, char *token);

/**
 * performs getting information about specific book; function parses 
 * returned json and dumps content at stdout
*/
void get_book_command(int sockfd, char *token);

/**
 * performs deleting specific book from library
*/
void delete_book_command(int sockfd, char *token);

/**
 * performs logout
*/
void logout_command(int sockfd, char **session_cookie, char **token);
#endif