Minimalist JSON Parser for Arduino
==================================

Cobbled together by Laurence A. Lee (rubyjedi@gmail.com) from the following original sources:

* jsmn         - JSON Lexical Scanner and Tokenizer (http://zserge.com/jsmn.html)
* jsmn-example - JSON Parser Example for Jsmn (https://github.com/alisdair/jsmn-example)

**This library is intended to be a lightweight alternative to [AJSON](https://github.com/interactive-matter/aJson.git), specifically for memory-constrained products like the [DigiSpark](http://digistump.com/product.php?id=1) that run on the ATTINY85 processor.**

**Then again, I'd highly recommend using a larger Arduino-like platform than a DigiSpark or ATTINY85. (My personal favorite is the Teensy 3.x in [pjrc's Teensy Family](https://www.pjrc.com/teensy/) ). Declaring static strings as I have done in the below Usage Example will quickly eat your RAM.**


## Limitations ##
**NESTED STRUCTURES NOT SUPPORTED (YET)**

Currently, this library only handles Key-Value pairs at the Top Level of the JSON String. 

This is not a shortcoming of the Jsmn library used under the hood - it's a design-decision to keep the memory footprint as small as possible.

If you feel the need to fork and enhance, I would suggest putting such enhancements within an `#ifdef` block (say, `#ifdef JSON_ALLOW_NESTED`) to allow end-users to have finer control over what enhancements they may or may not need.

## Usage Example ##
``` java
// NOTE1: As the Caller, you are responsible for the memory management (calling create_token_list() and release_token_list())
// of the Hashmap used to maintain the Key/Value pairs.
// NOTE2: The Key/Value strings in the HashMap reference strings as tokenized within the original JSON String.
// NOTE3: Arduino and AVR chips in general aren't nice about malloc/free, so this demo was refactored declare and reuse the Token List.
// Previous versions of this example would create and release the token list, which "works", but isn't ideal.

#include <json_arduino.h>
char json_string[256];
char* command;
int toggle;

token_list_t *token_list = NULL;

void setup() {
  Serial.begin(115200);
  delay(1000);
  token_list = create_token_list(25); // Create the Token List with enough space for 12 key/value pairs - 12 * 2 [key/value items] + 1 token for whole JSON Object.
}

void loop() {
  if (toggle==0){
    strcpy(json_string, "{command:hello, to:\"What a Wonderful World\"}");
  } else {
    // Demonstrates quotation-marks around tokens are handled by Jsmn OK.
    strcpy(json_string, "{\"command\":\"goodbye\", \"say_to\":\"Incredibly Cruel World\"}"); 
  }

  json_to_token_list(json_string, token_list); // Convert JSON String to a Hashmap of Key/Value Pairs

  command = json_get_value(token_list, (char *)"command");
  if (strcmp(command,"hello")==0){
    // do something for the Hello Command ...
    Serial.print("Hello, ");
    Serial.println( json_get_value(token_list, (char *)"to") );
  } else if (strcmp(command,"goodbye")==0) {
    // do something for the Goodbye Command ...
    Serial.print("Goodbye, You ");
    Serial.println( json_get_value(token_list, (char *)"say_to") );
  } else {
    Serial.println("** BONK **");
  } 

  (++toggle %= 2); // Weenie way to do "Odd or Even".
  delay(250); // Give the loop some time to breathe.

}
```
