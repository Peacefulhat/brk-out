#ifndef ARENA_H
#define ARENA_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>
#include<assert.h>

#include"pmath.h"

#define A_ASSERT assert
#define ARENA_REGION_CAPACITY 10 // no of nodes in arena

typedef enum {
  CHAR=0,
  INT,
  FLOAT,
  DOUBLE,
  UINT,
  VEC2
}Types;


 struct Region{
   size_t count;
   size_t capacity;
   Types type;
   struct Region *next;
   void*mem_region;
};

typedef struct Region Region;

typedef struct{
  Region*begin,*end;
  size_t region_count;
  size_t region_capacity;
}Arena;
 

Arena* alloc_arena();
size_t sizeof_types(Types type);
Region* alloc_region(size_t capacity,Types t);
void append_data(Arena *arr, Types t, void *data, size_t count);
void append_region(Arena*arena, Region*region);
static Region *find_compatible_region(Arena *arr, Types t, size_t count);
static Region *create_and_append_region(Arena *arr, Types t, size_t count);
void region_free(Region* region);
void arena_free(Arena* arena);
void arena_print(Arena* arena);
const char *type_to_string(Types t);
void region_print(Region* region);


#endif// ARENA_H

#ifdef ARENA_IMP

// alloated area
Arena* alloc_arena(){
  Arena*alloc_space=malloc(sizeof(Arena));
  A_ASSERT(alloc_space!=NULL);
  alloc_space->begin=NULL;
  alloc_space->end=NULL;
  alloc_space->region_count=0;
  alloc_space->region_capacity=ARENA_REGION_CAPACITY;
  return alloc_space;
}

size_t sizeof_types(Types type){
  switch(type){
  case CHAR:
    {
    return sizeof(char);
    }
    break;
  case INT:
    {
    return sizeof(int);
    }
    break;
    
  case FLOAT:
    {
    return sizeof(float);
    }
    break;
    
  case DOUBLE:
    {
    return sizeof(double);
    }
    break;
    
  case UINT:
    {
    return sizeof(unsigned int);
    }
    break;
  case VEC2:
    {
      return sizeof(vec2);
    }
    break;
  }
  return -1;// indiacting not a valid type
}
// treating capacity as total count of verties.count and capacity are equal kind of

Region* alloc_region(size_t capacity, Types t) {
  Region* region_block = malloc(sizeof(Region));
  A_ASSERT(region_block != NULL);
  region_block->mem_region = malloc(sizeof_types(t) * capacity);
  A_ASSERT(region_block->mem_region != NULL);
  
  region_block->capacity = capacity;
  region_block->type = t;
  region_block->count = 0;
  region_block->next = NULL;
  return region_block;
}


static Region *find_compatible_region(Arena *arr, Types t, size_t count) {
  Region *temp = arr->begin;
  while (temp != NULL) {
    if ((temp->capacity - temp->count) >= count && temp->type == t) {
      return temp;
    }
        temp = temp->next;
    }
  return NULL;
}

static Region *create_and_append_region(Arena *arr, Types t, size_t count) {
  size_t cap = count;
  Region *new_region = alloc_region(cap, t);
  A_ASSERT(new_region != NULL);

  if (arr->begin == NULL) {
    arr->begin = arr->end = new_region;
  } else {
    arr->end->next = new_region;
    arr->end = new_region;
  }

  return new_region;
}

void append_data(Arena *arr, Types t, void *data, size_t count) {
  A_ASSERT(arr != NULL);
  A_ASSERT(data != NULL);
  
  size_t type_size = sizeof_types(t);

  Region *region = find_compatible_region(arr, t, count);
  if (region == NULL) {
    region = create_and_append_region(arr, t, count);
  }

  void *dest = (char *)region->mem_region + region->count * type_size;
  memcpy(dest, data, count * type_size);
  region->count += count;
}

void region_free(Region* region){
  A_ASSERT(region!=NULL);
  free(region->mem_region);
  free(region);
}



void arena_free(Arena* arena) {
  A_ASSERT(arena != NULL);
  
  if(arena->begin==NULL){
    free(arena);
    return ;
  }
  
  Region* temp = arena->begin;
  A_ASSERT(temp!=NULL);

  while (temp != NULL) {
    Region* next = temp->next;
    region_free(temp);
    temp = next;
  }

  free(arena);
}

const char *type_to_string(Types t) {
  switch (t) {
  case CHAR: return "CHAR";
  case INT: return "INT";
  case FLOAT: return "FLOAT";
  case DOUBLE: return "DOUBLE";
  case UINT: return "UINT";
  case VEC2: return "VEC2";
  default: return "UNKNOWN";
  }
}

void region_print(Region* region) {
  A_ASSERT(region != NULL);
  printf("Region - Type: %s, Count: %zu, Capacity: %zu\n",
         type_to_string(region->type), region->count, region->capacity);

  size_t i;
  switch (region->type) {
    
  case CHAR: {
    char *data = (char *)region->mem_region;
    for (i = 0; i < region->count; i++) {
      printf("  [%zu] = '%c'\n", i, data[i]);
    }
    break;
  }
    
  case INT: {
    int *data = (int *)region->mem_region;
    for (i = 0; i < region->count; i++) {
      printf("  [%zu] = %d\n", i, data[i]);
    }
    break;
  }
 
  case FLOAT: {
    float *data = (float *)region->mem_region;
    for (i = 0; i < region->count; i++) {
      printf("  [%zu] = %f\n", i, data[i]);
    }
    break;
  }
    
  case DOUBLE: {
    double *data = (double *)region->mem_region;
    for (i = 0; i < region->count; i++) {
      printf("  [%zu] = %lf\n", i, data[i]);
    }
    break;
  }
    
  case UINT: {
    unsigned int *data = (unsigned int *)region->mem_region;
    for (i = 0; i < region->count; i++) {
      printf("  [%zu] = %u\n", i, data[i]);
    }
    break;
  }
	  
  case VEC2: {
    vec2 *data = (vec2 *)region->mem_region;
    for (i = 0; i < region->count; i++) {
      printf("  [%zu] = {%f,%f}\n", i, data[i].x,data[i].y);
    }
    break;
  }
	  
  default:
    printf("  Unknown data type.\n");
  }
  
}

void arena_print(Arena* arena) {
  A_ASSERT(arena != NULL);
  Region* temp = arena->begin;
  int index = 0;
  while (temp != NULL) {
    printf("Region #%d:\n", index++);
    region_print(temp);
    temp = temp->next;
  }
}

void append_region(Arena*arena, Region*region){
  A_ASSERT(arena!=NULL);
  A_ASSERT(region!=NULL);
  if(arena->begin==NULL){
    arena->begin=region;
    arena->end=region;
    arena->region_count++;
    return;
  }
  arena->end->next=region;
  arena->end=region;
}

#endif // ARENA_IMP


