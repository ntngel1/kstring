#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
   instance->how_much = length;
   return instance;
}

char* kstring_cstr(const kstring_t* instance)
{
    char* str = malloc(instance->length + 1);
    memcpy(str, instance->data, instance->length);
    str[instance->length] = '\0';
    return str;
}

void kstring_copy(const kstring_t* source, kstring_t* destination)
{
    if (destination->length < source->length)
    {
        kstring_free(destination);
        destination = kstring_create(source->length);
    }
    
    destination->how_much = source->length;
    
    memcpy(destination->data, source->data, source->length);
}

void kstring_print(const kstring_t* instance)
{
    fwrite(instance->data, sizeof(uint8_t), instance->length, stdout);
}

kstring_t* kstring_concat(const kstring_t* left, const kstring_t* right)
{
    kstring_t* instance = kstring_create(left->length + right->length);
    
    memcpy(instance->data, left->data, left->length);
    instance->how_much += left->length;

    memcpy(instance->data + left->length, right->data, right->length);
    instance->how_much += right->length;

    return instance;
}

void kstring_uppercase(kstring_t* instance)
{
    for (size_t i = 0; i < instance->length; ++i)
    {
        if ('a' <= instance->data[i] && instance->data[i] <= 'z')
        {
            instance->data[i] = instance->data[i] - 'a' + 'A';
        }
    }
}

void kstring_lowercase(kstring_t* instance)
{
    for (size_t i = 0; i < instance->length; ++i)
    {
        if ('A' <= instance->data[i] && instance->data[i] <= 'Z')
        {
            instance->data[i] = instance->data[i] - 'A' + 'a';
        }
    }
}

size_t kstring_length(const kstring_t* instance)
{
   return instance->length; 
}

bool kstring_compare(const kstring_t* left, const kstring_t* right)
{
    if (left->length != right->length)
    {
        return false;
    }

    for (size_t i = 0; i < left->length; ++i)
    {
        if (left->data[i] != right->data[i])
        {
            return false;
        }
    }

    return true;
}

size_t kstring_indexOf(const kstring_t* instance, char character)
{
    for (size_t i = 0; i < instance->length; ++i)
    {
        if (instance->data[i] == character)
        {
            return i;
        }
    }

    return -1;
}

size_t kstring_indexOf(const kstring_t* instance, const char* string)
{
    
}


int main(int argc, const char** argv)
{
    kstring_t* string = kstring_dup("Hello, world");
    kstring_t* string2 = kstring_dup("Hello, world1");
    printf("%d", kstring_compare(string, string2));

    return 0;
}

