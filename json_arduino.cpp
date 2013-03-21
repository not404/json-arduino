/* Minimalist JSON Parser for Arduino
 *
 * Cobbled together by Laurence A. Lee (rubyjedi@gmail.com)
 *   from the following original sources:
 *
 * jsmn         - JSON Lexical Scanner and Tokenizer (http://zserge.com/jsmn.html)
 * jsmn-example - JSON Parser Example for Jsmn (https://github.com/alisdair/jsmn-example)
 * 
 */

#include "json_arduino.h"

typedef enum {
	KEY, VALUE
} parse_state;


/* LALEE: Allow callers to create a Token-List structure to hold the parsed JSON Tokens */

token_list_t* create_token_list(int length){
	token_list_t *token_list = (token_list_t*)malloc(1*sizeof(token_list_t) );
	token_list->tokens = (jsmntok_t*)malloc(length*sizeof(jsmntok_t));
	token_list->length = length;
	token_list->count = 0;

	return token_list;
}

/* LALEE: Allow callers to release the Token-List structure's memory when finished with it. */

void release_token_list(token_list_t *token_list){
	free(token_list->tokens);
	free(token_list);
	token_list = NULL;
}

/* LALEE: Manage the retrieval of Key-Value Pairs */

char* json_get_value(token_list_t *token_list, char *key) {
	jsmntok_t *tokens = token_list->tokens;
	jsmntok_t *key_token;

	/* Token 0 is the JSMN_OBJECT representing entire structure. */
	/* Since we expect Key/Value Pairs, the following (without checking key_token->type==JSMN_KEY) is sufficient. */
	for (int i=1; i < token_list->count; i+=2) {
		key_token = tokens + i;
		if (strncmp(key,key_token->string,key_token->size)==0) {
			return (tokens+(i+1))->string;
		}
	}
	return NULL;
}

/* LALEE: json_token_tostr() was derived from git://github.com/alisdair/jsmn-example.git */

char * json_token_tostr(char *js, jsmntok_t *t) {
	js[t->end] = '\0';
	return js + t->start;
}


/* LALEE: Returns number of Key/Value pairs discovered. Nested structures not supported. */

int json_to_token_list(char *json_string, token_list_t *token_list){
	int found_pairs = 0;

	jsmntok_t *tokens = token_list->tokens;
	jsmn_parser parser;
	parse_state state = KEY;

	jsmn_init(&parser);

	/* LALEE: Run the Jsmn lexical scanner against the JSON String. We're memory-constrained, so no elaborate error-checking here. */
	int parse_status = jsmn_parse(&parser, json_string, tokens, token_list->length);
	if (parse_status != JSMN_SUCCESS) {
		return parse_status; // TODO: Add a mechanism to log a message . . .
	}

	/* LALEE: Very minimalist Parser to refine the Scanned Tokens into JSMN_KEYs and JSMN_VALUEs. */
	/* This will allow us to retrieve "Key/Value Pairs" via json_get_value() */
	/* Token 0 is a JSMN_OBJECT representing the entire JSON structure. */
	/* Since we expect Key/Value Pairs in incoming JSON, the following two-state parser (starting at Token 1, not 0) is sufficient. */
	for (int i = 1; i < token_list->length ; i++) {
		jsmntok_t *t = &tokens[i];

		switch (state) {
			case KEY:
				/* LALEE: Upgrade this Token into a JSMN_KEY */
				t->type = JSMN_KEY;
				t->size = t->end - t->start;
				t->string = json_token_tostr(json_string, t);

				state = VALUE;
				break;

			case VALUE:
				/* LALEE: Upgrade this Token into a JSMN_VALUE */
				t->type = JSMN_VALUE;
				t->size = t->end - t->start;
				t->string  = json_token_tostr(json_string, t);

				found_pairs++;

				state = KEY;
				break;
		}
	}
	return token_list->count = found_pairs;
}
