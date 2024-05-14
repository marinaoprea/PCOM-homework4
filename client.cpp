#include <arpa/inet.h>
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <iostream>

#include <bits/stdc++.h>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

#include "client.hpp"
#include "requests.hpp"
#include "helpers.hpp"

#define SPACE_INDENT 4

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

int main()
{
    char title[FIELD_SIZE], author[FIELD_SIZE], genre[FIELD_SIZE], publisher[FIELD_SIZE], page_count[FIELD_SIZE];

    char host[25];
    strcpy(host, HOST);
    //int sockfd = open_connection(host, PORT, AF_INET, SOCK_STREAM, 0);

    char command[COMMAND_SIZE];
    char *message, *response;
    char *session_cookie = NULL;
    char *token = NULL;

    while (fgets(command, COMMAND_SIZE - 1, stdin)) {
        int sockfd = open_connection(host, PORT, AF_INET, SOCK_STREAM, 0);
        
        if (strncmp(command, EXIT_COMMAND, strlen(EXIT_COMMAND)) == 0) {
            close_connection(sockfd);
            break;
        }

        if (strncmp(command, REGISTER_COMMAND, strlen(REGISTER_COMMAND)) == 0) {
            std::cout << "username=";
            char username[USERNAME_SIZE];
            fgets(username, USERNAME_SIZE - 1, stdin);
            username[strlen(username) - 1] = '\0';

            if (contains_whitespaces(username)) {
                std::cout << "ERROR: Username cannot contain whitespaces!\n";
                continue;
            }
            
            std::cout << "password=";
            char password[PASSWORD_SIZE];
            fgets(password, PASSWORD_SIZE - 1, stdin);
            password[strlen(password) - 1] = '\0';
            if (contains_whitespaces(password)) {
                std::cout << "ERROR: Password cannot contain whitespaces!\n";
                continue;
            }

            json j;
            j["username"] = username;
            j["password"] = password;

            char *json_string = (char *)calloc(strlen(j.dump().c_str()), sizeof(char));
            strcpy(json_string, const_cast <char *> (j.dump().c_str()));

            //std::cout << json_string << '\n';

            message = compute_post_request(host, const_cast <char *> ("/api/v1/tema/auth/register"), const_cast <char *> ("application/json"), &json_string, 1, NULL, 0, NULL);

            //std::cout << message << '\n';

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);



            //std::cout << response << '\n';

            if (strstr(response, "HTTP/1.1 2")) {
                std::cout << "SUCCESS: Utilizator inregistrat cu succes!\n";
            } else {
                if (strstr(response, "username already taken") == 0) {
                    std::cout << "ERROR: Nume de utilizator folosit deja!\n";
                }
            }
            continue;
        }

        if (strncmp(command, LOGIN_COMMAND, strlen(LOGIN_COMMAND)) == 0) {
            std::cout << "username=";
            char username[USERNAME_SIZE];
            fgets(username, USERNAME_SIZE - 1, stdin);
            username[strlen(username) - 1] = '\0';
            std::cout << "password=";
            char password[PASSWORD_SIZE];
            fgets(password, PASSWORD_SIZE - 1, stdin);
            password[strlen(password) - 1] = '\0';

            json j;
            j["username"] = username;
            j["password"] = password;

            char *json_string = (char *)calloc(strlen(j.dump().c_str()) + 1, sizeof(char));
            strcpy(json_string, const_cast <char *> (j.dump().c_str()));

         //   std::cout << json_string << '\n';

            message = compute_post_request(host, const_cast <char *> ("/api/v1/tema/auth/login"), const_cast <char *> ("application/json"), &json_string, 1, NULL, 0, NULL);

           // std::cout << message;

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            //std::cout << response;

            if (strstr(response, "HTTP/1.1 2")) {
                std::cout << "SUCCESS: Utilizatorul a fost logat cu succes!\n";
                char *cookie_start = strstr(response, "connect.sid");
                char *cookie_end = strstr(cookie_start, ";");

                session_cookie = (char *)calloc((int) (cookie_end - cookie_start), sizeof(char));

                strncpy(session_cookie, cookie_start, (int)(cookie_end - cookie_start));

                //std::cout << session_cookie << '\n';
            } else {
                    std::cout << "ERROR: Credentialele nu sunt bune!\n";
            }
            continue;
        }

        if (strncmp(command, ENTER_COMMAND, strlen(ENTER_COMMAND)) == 0) {
            message = compute_get_request(host, const_cast <char *> ("/api/v1/tema/library/access"), NULL, &session_cookie, 1, NULL);

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            //std::cout << response << '\n';

            if (strstr(response, "HTTP/1.1 2")) {
                std::cout << "SUCCESS: Utilizatorul are acces la biblioteca!\n";

                char *start_json = strchr(response, '{');
                char *end_json = strchr(start_json, '}');
                char *json_string = (char *)calloc((int)(end_json - start_json) + 2, sizeof(char));
                strncpy(json_string, start_json, (int)(end_json - start_json) + 1);

                //std::cout << json_string << '\n';

                json j = json::parse(json_string);
                //std::cout << j["token"].dump().c_str() << "\n";

                token = (char *)calloc(strlen(j["token"].dump().c_str()) + 1, sizeof(int));
                strncpy(token, j["token"].dump().c_str() + 1, strlen(j["token"].dump().c_str()) - 2);
           }
           continue;
        }

        if (strncmp(command, GET_ALL_COMMAND, strlen(GET_ALL_COMMAND)) == 0) {
            message = compute_get_request(host, const_cast <char *> ("/api/v1/tema/library/books"), NULL, NULL, 0, token);

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);
            if (strstr(response, "HTTP/1.1 2")) {
                //std::cout << token << '\n';

                char *start_json = strchr(response, '[');
                char *end_json = strchr(start_json, ']');
                char *json_string = (char *)calloc((int)(end_json - start_json) + 2, sizeof(char));
                strncpy(json_string, start_json, (int)(end_json - start_json) + 1);

                //std::cout << json_string << '\n';

                json j = json::parse(json_string);

                std::cout << "SUCCESS: " << j.dump(SPACE_INDENT) << '\n';

                //std::cout << response << '\n';
            } else {
                char *start_json = strchr(response, '{');
                char *end_json = strchr(start_json, '}');
                char *json_string = (char *)calloc((int)(end_json - start_json) + 2, sizeof(char));
                strncpy(json_string, start_json, (int)(end_json - start_json) + 1);

                //std::cout << json_string << '\n';

                json j = json::parse(json_string);

                std::cout << "ERROR: " << j["error"].dump() << '\n';
            }
            continue;
        }

        if (strncmp(command, ADD_COMMAND, strlen(ADD_COMMAND)) == 0) {
            std::cout << "title=";
            fgets(title, FIELD_SIZE - 1, stdin);
            title[strlen(title) - 1] = '\0';

            std::cout << "author=";
            fgets(author, FIELD_SIZE - 1, stdin);
            author[strlen(author) - 1] = '\0';

            std::cout << "genre=";
            fgets(genre, FIELD_SIZE - 1, stdin);
            genre[strlen(genre) - 1] = '\0';

            std::cout << "publisher=";
            fgets(publisher, FIELD_SIZE - 1, stdin);
            publisher[strlen(publisher) - 1] = '\0';

            std::cout << "page_count=";
            fgets(page_count, FIELD_SIZE - 1, stdin);
            page_count[strlen(page_count) - 1] = '\0';

            int count = atoi(page_count);
            if (count == 0) {
                std::cout << "ERROR: Tip de date incorect pentru pagini!\n";
                continue;
            }

            json j;
            j["title"] = title;
            j["author"] = author;
            j["genre"] = genre;
            j["publisher"] = publisher;
            j["page_count"] = count;

            char *json_string = (char *)calloc(strlen(j.dump().c_str()) + 1, sizeof(char));
            strcpy(json_string, (j.dump().c_str()));

            message = compute_post_request(host, const_cast <char *> ("/api/v1/tema/library/books"), const_cast <char *> ("application/json"), &json_string, 1, NULL, 0, token);

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            //std::cout << response << '\n';
            if (strstr(response, "HTTP/1.1 2")) {
                std::cout << "SUCCESS: Carte adaugata cu succes!\n";
            } else {
                char *start_json = strchr(response, '{');
                char *end_json = strchr(start_json, '}');
                char *json_string = (char *)calloc((int)(end_json - start_json) + 2, sizeof(char));
                strncpy(json_string, start_json, (int)(end_json - start_json) + 1);

                //std::cout << json_string << '\n';

                json j = json::parse(json_string);

                std::cout << "ERROR: " << j["error"].dump() << '\n';
            }

            continue;
        }

        if (strncmp(command, GET_COMMAND, strlen(GET_COMMAND)) == 0) {
            std::cout << "id=";
            char id[FIELD_SIZE];
            fgets(id, FIELD_SIZE - 1, stdin);

            if (atoi(id) == 0 && strcmp("0", id)) {
                std::cout << "ERROR: Format id gresit!\n";
                continue;
            }

            id[strlen(id) - 1] = '\0';

            char path[FIELD_SIZE];
            strcpy(path, "/api/v1/tema/library/books/");
            strcat(path, id);

            message = compute_get_request(host, path, NULL, NULL, 0, token);

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            if (strstr(response, "HTTP/1.1 2")) {
                char *start_json = strchr(response, '{');
                char *end_json = strchr(start_json, '}');
                char *json_string = (char *)calloc((int)(end_json - start_json) + 2, sizeof(char));
                strncpy(json_string, start_json, (int)(end_json - start_json) + 1);

                json j = json::parse(json_string);

                std::cout << "SUCCESS: " << j.dump(SPACE_INDENT) << '\n';
            } else {
                char *start_json = strchr(response, '{');
                char *end_json = strchr(start_json, '}');
                char *json_string = (char *)calloc((int)(end_json - start_json) + 2, sizeof(char));
                strncpy(json_string, start_json, (int)(end_json - start_json) + 1);

                //std::cout << json_string << '\n';

                json j = json::parse(json_string);

                std::cout << "ERROR: " << j["error"].dump() << '\n';
            }

            continue;
        }

        if (strncmp(command, DELETE_COMMAND, strlen(DELETE_COMMAND)) == 0) {
            std::cout << "id=";
            char id[FIELD_SIZE];
            fgets(id, FIELD_SIZE - 1, stdin);

            if (atoi(id) == 0 && strcmp("0", id)) {
                std::cout << "ERROR: Format id gresit!\n";
                continue;
            }

            id[strlen(id) - 1] = '\0';

            char path[FIELD_SIZE];
            strcpy(path, "/api/v1/tema/library/books/");
            strcat(path, id);
            //std::cout << path << '\n';

            message = compute_delete_request(host, path, token);

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            if (strstr(response, "HTTP/1.1 2")) {
                std::cout << "SUCCESS: Carte stearsa cu succes!\n";
            } else {
                char *start_json = strchr(response, '{');
                char *end_json = strchr(start_json, '}');
                char *json_string = (char *)calloc((int)(end_json - start_json) + 2, sizeof(char));
                strncpy(json_string, start_json, (int)(end_json - start_json) + 1);

                //std::cout << json_string << '\n';

                json j = json::parse(json_string);

                std::cout << "ERROR: " << j["error"].dump() << '\n';
            }

            continue;
        }

        if (strncmp(command, LOGOUT_COMMAND, strlen(LOGOUT_COMMAND)) == 0) {
            message = compute_get_request(host, const_cast <char *> ("/api/v1/tema/auth/logout"), NULL, &session_cookie, 1, NULL);

            send_to_server(sockfd, message);

            response = receive_from_server(sockfd);

            if (strstr(response, "HTTP/1.1 2")) {
                std::cout << "SUCCESS: Utilizatorul s-a delogat cu succes!\n";
                free(token);
                token = NULL;
            } else {
                char *start_json = strchr(response, '{');
                char *end_json = strchr(start_json, '}');
                char *json_string = (char *)calloc((int)(end_json - start_json) + 2, sizeof(char));
                strncpy(json_string, start_json, (int)(end_json - start_json) + 1);

                //std::cout << json_string << '\n';

                json j = json::parse(json_string);

                std::cout << "ERROR: " << j["error"].dump() << '\n';
            }

            continue;
        }
    }

    return 0;
}