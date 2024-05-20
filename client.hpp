#ifndef CLIENT_HPP
#define CLIENT_HPP

void parse_error(char *response);

int get_credentials(char *username, char *password, char *error_msg);

int fill_book_fields(char *title, char *author, char *genre, char *publisher, char *page_count, int &count, char *err_msg);

void get_book_id(char *id);

void register_command(int sockfd);

void login_command(int sockfd, char **session_cookie);

void enter_library_command(int sockfd, char *session_cookie, char **token);

void get_all_command(int sockfd, char *token);

void add_book_command(int sockfd, char *token);

void get_book_command(int sockfd, char *token);

void delete_book_command(int sockfd, char *token);

void logout_command(int sockfd, char *session_cookie, char **token);
#endif