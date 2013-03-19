// map_lib
// A simple associative-array library for C
//
// License: MIT / X11
// Copyright (c) 2009 by James K. Lawless
// jimbo@radiks.net http://www.radiks.net/~jimbo
// http://www.mailsend-online.com
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_lib.h"

struct map_t *map_create() {
   struct map_t *m;
   m=(struct map_t *)malloc(sizeof(struct map_t));
   m->name=NULL;
   m->value=NULL;
   m->nxt=NULL;

   return m;
}

void map_destroy(struct map_t *map) {
  struct map_t *nxt;
  while (map!=NULL) {
    if(map->value!=NULL){ free(map->value); map->value=NULL; }
    if(map->name !=NULL){ free(map->name);  map->name=NULL; }
    nxt=map->nxt;
    free(map);
    map=nxt;
  }
}

void map_set(struct map_t *m,char *name,char *value) {
   struct map_t *map;

   if(m->name==NULL) {
      m->name=(char *)malloc(strlen(name)+1);
      strcpy(m->name,name);
      m->value=(char *)malloc(strlen(value)+1);
      strcpy(m->value,value);
      m->nxt=NULL;
      return;
   }
   for(map=m;;map=map->nxt) {
      if(!strcmp(name,map->name)) {
         if(map->value!=NULL) {
            free(map->value);
            map->value=(char *)malloc(strlen(value)+1);
            strcpy(map->value,value);
            return;
         }
      }
      if(map->nxt==NULL) {
         map->nxt=(struct map_t *)malloc(sizeof(struct map_t));
         map=map->nxt;
         map->name=(char *)malloc(strlen(name)+1);
         strcpy(map->name,name);
         map->value=(char *)malloc(strlen(value)+1);
         strcpy(map->value,value);
         map->nxt=NULL;
         return;
      } 
   }
}

char *map_get(struct map_t *m,char *name) {
   struct map_t *map;
   for(map=m;map!=NULL;map=map->nxt) {
      if(!strcmp(name,map->name)) {
         return map->value;
      }
   }
   return "";
}
