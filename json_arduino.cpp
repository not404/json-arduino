/* Minimalist JSON Parser for Arduino
 *
 * Cobbled together by Laurence A. Lee (rubyjedi@gmail.com)
 *   from the following original sources:
 *
 * jsmn         - JSON Parser (http://zserge.com/jsmn.html)
 * jsmn-example - State-Machine JSON Parser Example for Jsmn (https://github.com/alisdair/jsmn-example)
 * map_lib      - Associative Array Library (http://www.mailsend-online.com/blog/a-simple-associative-array-library-in-c.html)
 * 
 */

#include "json_arduino.h"

#ifndef MAX_JSON_TOKENS
#define MAX_JSON_TOKENS 30
#endif

typedef enum {
	START, KEY, VALUE, SKIP, STOP
} parse_state;

/* LALEE: json_token_tostr() was derived from git://github.com/alisdair/jsmn-example.git */

char * json_token_tostr(char *js, jsmntok_t *t) {
	js[t->end] = '\0';
	return js + t->start;
}

/* LALEE: Returns number of Key/Value pairs discovered. Nested structures not supported yet. */

int json_to_hash(char *json_string, struct map_t *json_hash) {
	unsigned int n = MAX_JSON_TOKENS;
	char *json_kvpair_key, *json_kvpair_val; // Need to cache the Key pointer while the Value gets processed.

	int found_pairs = 0;
	jsmn_parser parser;
	parse_state state = START;

	jsmntok_t *tokens = (jsmntok_t*) malloc(sizeof(jsmntok_t) * n);
	jsmn_init(&parser);

	/* LALEE: Run the Jsmn parser against the JSON String. No elaborate error-checking here. */
	int parse_status = jsmn_parse(&parser, json_string, tokens, n);
	if (parse_status != JSMN_SUCCESS) {
		return (parse_status > 0) ? -parse_status : parse_status; // TODO: Add a mechanism to log a message . . .
	}

	/* LALEE: Stateful JSON Parser for JSMN, derived from git://github.com/alisdair/jsmn-example.git, BEGINS */
	size_t i, j;
	size_t object_tokens = 0;

	for (i = 0, j = 1; j > 0; i++, j--) {
		jsmntok_t *t = &tokens[i];

		// Should never reach uninitialized tokens
		// log_assert(t->start != -1 && t->end != -1);

		if (t->type == JSMN_ARRAY || t->type == JSMN_OBJECT) j += t->size;

		switch (state) {
			case START:
				// if (t->type != JSMN_OBJECT)
				// log_die("Invalid response: root element must be an object.");

				state = KEY;
				object_tokens = t->size;

				if (object_tokens == 0) state = STOP;

				//if (object_tokens % 2 != 0)
				// log_die("Invalid response: object must have even number of children.");

				break;

			case KEY:
				object_tokens--;

				// if (t->type != JSMN_STRING)
				// log_die("Invalid response: object keys must be strings.");

				state = VALUE;

				/* LALEE: Save the Key so we can add it to the KV-Pair */
				json_kvpair_key = json_token_tostr(json_string, t);

				break;

			case SKIP:
				// if (t->type != JSMN_STRING && t->type != JSMN_PRIMITIVE)
				// log_die("Invalid response: object values must be strings or primitives.");

				object_tokens--;
				state = KEY;

				if (object_tokens == 0) state = STOP;

				break;

			case VALUE:
				// if (t->type != JSMN_STRING && t->type != JSMN_PRIMITIVE)
				// log_die("Invalid response: object values must be strings or primitives.");


				/* LALEE:  Add this KV-Pair to the HashMap */
				json_kvpair_val = json_token_tostr(json_string, t);
				map_set(json_hash, json_kvpair_key, json_kvpair_val);
				found_pairs++;

				object_tokens--;
				state = KEY;

				if (object_tokens == 0) state = STOP;

				break;

			case STOP:
				// Just consume the tokens
				break;

			default:
				// log_die("Invalid state %u", state);
				state = STOP;
				break;
		}
	}

	/* LALEE: Release dynamic memory allocations.
	 * The JSON parser is done, so we don't need Jsmn's Token Space anymore.
	 * The KV-Pair strings we return in found_pairs are pointing to strings as tokenized within the JSON String Buffer.
	 */
	free(tokens);

	/* LALEE: And now return the HashMap of Key-Value pairs to our caller. All done! */
	return found_pairs;
}
