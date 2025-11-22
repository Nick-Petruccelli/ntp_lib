#define NTP_LIB
#include "ntp_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

typedef struct {
  char *key;
  uint32 hashcode;
  void *value_ptr;
} DictEntry;

typedef struct {
  DictEntry *entries;
  uint32 size;
  uint32 num_entries;
} Dict;

#define HASHMUL 420
#define ARENA_SIZE 10000

bool32 dict_init(Dict *dict, uint32 size);
void dict_destroy(Dict *dict);
bool32 dict_add(Dict *dict, char *key, void *value_ptr);
void *dict_get(Dict *dict, char *key);
uint32 get_hashcode(char *key);

int main(int argc, char **argv){
  Dict dict;
  if(dict_init(&dict, 100)){
    printf("ERROR: Failed to init dict\n");
    return 1;
  }

  uint32 one = 1;
  uint32 two = 2;
  uint32 three = 3;
  uint32 four = 4;
  uint32 five = 5;
  uint32 six = 6;
  uint32 seven = 7;
  uint32 eight = 8;
  uint32 nine = 9;
  uint32 ten = 10;
  dict_add(&dict, "one", &one);
  dict_add(&dict, "two", &two);
  dict_add(&dict, "three", &three);
  dict_add(&dict, "four", &four);
  dict_add(&dict, "five", &five);
  dict_add(&dict, "six", &six);
  dict_add(&dict, "seven", &seven);
  dict_add(&dict, "eight", &eight);
  dict_add(&dict, "nine", &nine);
  dict_add(&dict, "ten", &ten);
  printf("Dict got: '%s' -> %d\n", "one\0", *(uint32*)dict_get(&dict, "one"));
  printf("Dict got: '%s' -> %d\n", "tow\0", *(uint32*)dict_get(&dict, "two"));
  printf("Dict got: '%s' -> %d\n", "three\0", *(uint32*)dict_get(&dict, "three"));
  printf("Dict got: '%s' -> %d\n", "four\0", *(uint32*)dict_get(&dict, "four"));
  printf("Dict got: '%s' -> %d\n", "five\0", *(uint32*)dict_get(&dict, "five"));
  printf("Dict got: '%s' -> %d\n", "six\0", *(uint32*)dict_get(&dict, "six"));
  printf("Dict got: '%s' -> %d\n", "seven\0", *(uint32*)dict_get(&dict, "seven"));
  printf("Dict got: '%s' -> %d\n", "eight\0", *(uint32*)dict_get(&dict, "eight"));
  printf("Dict got: '%s' -> %d\n", "nine\0", *(uint32*)dict_get(&dict, "nine"));
  printf("Dict got: '%s' -> %d\n", "ten\0", *(uint32*)dict_get(&dict, "ten"));
  dict_add(&dict, "one", &ten);
  dict_add(&dict, "two", &nine);
  dict_add(&dict, "three", &eight);
  dict_add(&dict, "four", &seven);
  dict_add(&dict, "five", &six);
  dict_add(&dict, "six", &five);
  dict_add(&dict, "seven", &four);
  dict_add(&dict, "eight", &three);
  dict_add(&dict, "nine", &two);
  dict_add(&dict, "ten", &one);
  printf("Dict got: '%s' -> %d\n", "one\0", *(uint32*)dict_get(&dict, "one"));
  printf("Dict got: '%s' -> %d\n", "tow\0", *(uint32*)dict_get(&dict, "two"));
  printf("Dict got: '%s' -> %d\n", "three\0", *(uint32*)dict_get(&dict, "three"));
  printf("Dict got: '%s' -> %d\n", "four\0", *(uint32*)dict_get(&dict, "four"));
  printf("Dict got: '%s' -> %d\n", "five\0", *(uint32*)dict_get(&dict, "five"));
  printf("Dict got: '%s' -> %d\n", "six\0", *(uint32*)dict_get(&dict, "six"));
  printf("Dict got: '%s' -> %d\n", "seven\0", *(uint32*)dict_get(&dict, "seven"));
  printf("Dict got: '%s' -> %d\n", "eight\0", *(uint32*)dict_get(&dict, "eight"));
  printf("Dict got: '%s' -> %d\n", "nine\0", *(uint32*)dict_get(&dict, "nine"));
  printf("Dict got: '%s' -> %d\n", "ten\0", *(uint32*)dict_get(&dict, "ten"));
  dict_destroy(&dict);
}

uint32 get_hashcode(char *key){
  uint32 i = 0;
  uint32 char_sum = 0;
  while(key[i] != '\0'){
    char_sum += (uint32)key[i];
    i++;
  }
  return char_sum * HASHMUL;
}

bool32 dict_init(Dict *dict, uint32 size){
  dict->entries = malloc(size * sizeof(DictEntry));
  if(dict->entries == NULL){
    printf("ERROR: Failed to allocate dict entries\n");
    return 1;
  }
  dict->size = size;
  dict->num_entries = 0;
  for(uint32 i=0; i<size; i++){
    dict->entries[i].key = NULL;
  }
  return 0;
}

void dict_destroy(Dict *dict){
  free(dict->entries);
  dict->size = 0;
  dict->num_entries = 0;
}

bool32 dict_add(Dict *dict, char *key, void *value_ptr){
  if(dict->num_entries >= dict->size){
    printf("ERROR: Dict to small\n");
    return 1;
  }
  uint32 hashcode = get_hashcode(key);
  uint32 bin = hashcode % dict->size;
  for(uint32 i=0; i<dict->num_entries; i++){
    bin += i;
    if(bin >= dict->size)
      bin = 0;
    if(dict->entries[bin].key == NULL)
      break;
    if(strcmp(dict->entries[bin].key, key) == 0)
      break;
  }
  dict->entries[bin].key = key;
  dict->entries[bin].hashcode = hashcode;
  dict->entries[bin].value_ptr = value_ptr;
  dict->num_entries++;
  return 0;
}

void *dict_get(Dict *dict, char *key){
  uint32 hashcode = get_hashcode(key);
  uint32 bin = hashcode % dict->size;
  for(uint32 i=0; i<dict->num_entries; i++){
    bin += i;
    if(bin >= dict->size)
      bin = 0;
    if(dict->entries[bin].key == NULL)
      return NULL;
    if(strcmp(dict->entries[bin].key, key) == 0)
      return dict->entries[bin].value_ptr;
  }
  return NULL;
}
