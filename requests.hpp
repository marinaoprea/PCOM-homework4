#ifndef _REQUESTS_
#define _REQUESTS_

/**
 * computes and returns GET request
*/
char *compute_get_request(char *host, char *url, char *query_params,
							char **cookies, int cookies_count, char *token);

/**
 * computes and returns POST request
*/
char *compute_post_request(char *host, char *url, char* content_type, 
						   char **body_data, int body_data_fields_count, 
						   char** cookies, int cookies_count, char *token);

/**
 * computes and returns DELETE request
*/
char *compute_delete_request(char *host, char *url, char *token);

#endif
