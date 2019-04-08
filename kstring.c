#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "str_util.c"

typedef struct kstring {
    uint8_t* data;
    size_t length;
    size_t how_much;
} kstring_t;

kstring_t* kstring_create(size_t length)
{
    kstring_t* instance = malloc(sizeof(kstring_t));
    instance->length = length;
    instance->data = malloc(sizeof(uint8_t) * length);
    return instance;
}

void kstring_free(kstring_t* instance)
{
    free(instance->data);
    free(instance);
}

kstring_t* kstring_dup(const char* string) {
   size_t length = string_length(string);
   kstring_t* instance = kstring_create(length);
   memcpy(instance->data, string, length);
   return instance;
}

void kstring_print(kstring_t* instance)
{
    fwrite(instance->data, sizeof(uint8_t), instance->length, stdout);
}


int main(int argc, const char** argv)
{
    kstring_t* string = kstring_dup("Test String");
    kstring_print(string);
    
    kstring_free(string);
    
    return 0;
}

