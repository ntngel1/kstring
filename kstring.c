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
   instance->how_much = length;
   return instance;
}

char* kstring_cstr(kstring_t* instance)
{
    char* str = malloc(instance->length + 1);
    memcpy(str, instance->data, instance->length);
    str[instance->length] = '\0';
    return str;
}

void kstring_print(kstring_t* instance)
{
    fwrite(instance->data, sizeof(uint8_t), instance->length, stdout);
}

kstring_t* kstring_concat(kstring_t* left, kstring_t* right)
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
        if (instance->data[i] >= 97 && instance->data[i] <= 122)
        {
            printf("%c\n", instance->data[i]);
            instance->data[i] = 32;
        }
    }
}

int main(int argc, const char** argv)
{
    kstring_t* left = kstring_dup("test");
    kstring_t* right = kstring_dup(" 2test");
    kstring_uppercase(left);
    kstring_print(left);
    //kstring_t* concated = kstring_concat(left, right);
    //kstring_print(concated);

    //const char* simple = kstring_cstr(concated);
    //fwrite(simple, sizeof(char), string_length(simple), stdout);


    return 0;
}

