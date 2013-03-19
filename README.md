Minimalist JSON Parser for Arduino
==================================

Cobbled together by Laurence A. Lee (rubyjedi@gmail.com) from the following original sources:

* jsmn         - JSON Parser (http://zserge.com/jsmn.html)
* jsmn-example - State-Machine JSON Parser Example for Jsmn (https://github.com/alisdair/jsmn-example)
* map_lib      - Associative Array Library (http://www.mailsend-online.com/blog/a-simple-associative-array-library-in-c.html)

**This library is intended to be a lightweight alternative to [AJSON](https://github.com/interactive-matter/aJson.git), specifically for memory-constrained products like the [DigiSpark](http://digistump.com/product.php?id=1) that run on the ATTINY85 processor.**

## Limitations ##
**NESTED STRUCTURES NOT SUPPORTED (YET)**

Currently, this library only handles Key-Value pairs at the Top Level of the JSON String. 

This is not a shortcoming of the Jsmn JSON Parsing library used under the hood - it's a design-decision of how I trivially stitched together Jsmn and map_lib to keep the memory footprint as small as possible.

If you feel the need to fork and enhance, I would suggest putting such enhancements within an `#ifdef` block (say, `#ifdef JSON_ALLOW_NESTED`) to allow end-users to have finer control over what enhancements they may or may not need.

## Usage Example ##
``` java
#include <json_arduino.h>

// NOTE1: As the Caller, you are responsible for the memory management (calling map_create() and map_destroy())
// of the Hashmap used to maintain the Key/Value pairs.
// NOTE2: The Key/Value strings in the HashMap reference strings as tokenized within the original JSON String.

char json_string[256];
int toggle;
struct map_t *json_hash = NULL;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  if (toggle>0){
    strcpy(json_string, "{command:hello, to:world}");
  } else {
    // Demonstrates quotation-marks around tokens are handled by Jsmn OK.
    strcpy(json_string, "{\"command\":\"goodbye\", \"say_to\":\"World\"}"); 
  }
  
  json_hash = map_create(); // Create the Hashmap

  json_to_hash(json_string, json_hash); // Convert JSON String to a Hashmap of Key/Value Pairs
  char* command = map_get(json_hash, "command");
  if (strcmp(command,"hello")==0){
    // do something for the Hello Command ...
    Serial.print("Hello, ");
    Serial.println(map_get(json_hash,"to"));
  } else if (strcmp(command,"goodbye")==0) {
    // do something for the Goodbye Command ...
    Serial.print("Goodbye, Cruel ");
    Serial.println(map_get(json_hash,"say_to"));
  }
  map_destroy(json_hash); // Release the Hashmap, else memory-leak at your own peril.
  toggle = (++toggle % 2); // Weenie way to do "Odd or Even".
  
  delay(500); // Give the loop some time to breathe.
}

```
