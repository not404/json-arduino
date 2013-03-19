#include <stdlib.h>
#include "utility/jsmn.h"
#include "utility/map_lib.h"

#ifndef __JSON_ARDUINO_H_
#define __JSON_ARDUINO_H_

int json_to_hash(char* json_string, struct map_t* json_hash);

#endif
