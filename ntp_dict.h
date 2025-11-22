#ifndef NTP_DICT
#define NTP_DICT
#include "ntp_types.h"
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


bool32 dict_init(Dict *dict, uint32 size);
void dict_destroy(Dict *dict);
bool32 dict_add(Dict *dict, char *key, void *value_ptr);
void *dict_get(Dict *dict, char *key);
uint32 get_hashcode(char *key);
#endif

#ifdef NTP_DICT_IMPL

#define HASHMUL 420
#define ARENA_SIZE 10000
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
  dict->entries = (DictEntry*)malloc(size * sizeof(DictEntry));
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

#endif
