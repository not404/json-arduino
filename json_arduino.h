#include <stdlib.h>
#include <string.h>
#include "utility/jsmn.h"

#ifndef __JSON_ARDUINO_H_
#define __JSON_ARDUINO_H_

/* LALEE: Declare a TOKENS container with a little extra to manage the LENGTH of the Tokens */
typedef struct {
	int length; // LALEE: This is the maximum number of tokens available to Jsmn.
	int count;  // LALEE: This is the number of tokens ACTUALLY found by Jsmn.
	jsmntok_t *tokens;
} token_list_t;

token_list_t* create_token_list(int length);
void release_token_list(token_list_t *token_list);
char* json_get_value(token_list_t *token_list, char *key);
int json_to_token_list(char *json_string, token_list_t *token_list);

#endif
