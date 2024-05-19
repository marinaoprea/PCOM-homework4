#ifndef CLIENT_HPP
#define CLIENT_HPP

void parse_error(char *response);

int get_credentials(char *username, char *password);

void fill_book_fields(char *title, char *author, char *genre, char *publisher, char *page_count, int &count);

void get_book_id(char *id);
#endif