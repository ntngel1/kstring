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
    instance->how_much = 0;
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

void kstring_copy(const kstring_t* source, kstring_t** destination)
{
    kstring_t* dest = *destination;

    if (dest->length < source->length || dest->length > source->length)
    {
        dest->data = realloc(dest->data, source->length * sizeof(uint8_t));
    }
    
    dest->how_much = source->length;
    memcpy(dest->data, source->data, source->length);
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

kstring_t* kstring_slice(const kstring_t* source, size_t begin, size_t end)
{
    if (begin > source->length || end > source->length)
    {
        return NULL;
    }

    if (begin == end)
    {
        return NULL;
    }

    if (begin > end)
    {
        size_t temp = begin;
        begin = end;
        end = temp;
    }

    kstring_t* destination = kstring_create(end - begin);
    destination->how_much = destination->length;
    memcpy(destination->data, source->data, destination->length);
    
    return destination;
}

void kstring_append(kstring_t** instance, const char* string)
{
    size_t string_len = string_length(string);
    if (string_len == 0)
    {
        return;
    }

    kstring_t* dereferenced = *instance;

    dereferenced->data = realloc(dereferenced->data, string_len + dereferenced->length);
    memcpy(dereferenced->data + dereferenced->length, string, string_len);
    dereferenced->length = string_len + dereferenced->length;
    dereferenced->how_much = dereferenced->length;
}

uint8_t kstring_at(const kstring_t* instance, size_t position)
{
    if (position >= instance->length)
    {
        fprintf(stderr, "Error accessing element at index(%ld) in kstring(address = %p, length = %ld)\n", position, instance, instance->length);
        return 0;
    }

    return instance->data[position];
}

size_t* __kstring_horspool_build_offset_table__(const char* string, size_t length)
{
    size_t* offset = malloc(sizeof(size_t) * 256);

    for (size_t i = 0; i < 256; ++i)
    {
        offset[i] = length;
    }

    for (size_t i = 0; i < length - 1; ++i)
    {
        offset[string[i]] = length - i - 1;
    }

    return offset;
}

size_t kstring_find(const kstring_t* instance, const char* needle)
{
    size_t needle_length = string_length(needle);
    size_t string_length = instance->length;

    if (needle_length > string_length || needle == "")
    {
        return -1;
    }

    size_t* offsets = __kstring_horspool_build_offset_table__(needle, needle_length);
    const char* string = instance->data;

    size_t skip = 0;
    while (string_length - skip >= needle_length)
    {
        for (size_t i = needle_length - 1; i >= 0; --i)
        {
            if (string[skip + i] != needle[i])
            {
                skip += offsets[string[skip + i]];
                break;
            }

            if (i == 0)
            {
                free(offsets);
                return skip + i;
            }
        }
    }

    free(offsets);
    return -1;
}

void kstring_trim(kstring_t* instance)
{
    size_t counter = 0;
    size_t whitespaceCount = 0;
    
    for (size_t i = 0; i < instance->length; ++i)
    {
        if (instance->data[i] != ' ')
        {
            instance->data[counter++] = instance->data[i];
        } else {
            whitespaceCount++;
        }
    }

    if (whitespaceCount == 0)
        return;

    instance->length = counter;
    instance->how_much = counter;
    instance->data = realloc(instance->data, counter);
}

size_t kstring_lastIndexOf(kstring_t* instance, char character)
{
    for (size_t i = instance->length - 1; i >= 0; --i)
    {
        if (instance->data[i] == character)
        {
            return i;
        }
    }

    return -1;
}

void kstring_reverse(kstring_t* instance)
{
    size_t a = 0;
    size_t b = 0;
    for (a = 0, b = instance->length - 1; a < instance->length && b >= 0; ++a, --b)
    {
        printf("Normal = %c, reversed = %c\n", instance->data[a], instance->data[b]);
        char left_char = instance->data[a];
        char right_char = instance->data[b];
        instance->data[a] = right_char;
        instance->data[b] = left_char;
    }
}

void kstring_test(kstring_t* instance)
{
    printf("TEST RUNNED\n");

    if (instance->length > instance->how_much || instance->length < instance->how_much)
    {
        printf("Warning! Defined length = %ld, but defined how_much = %ld\n", instance->length, instance->how_much);
    }

    const char* string = kstring_cstr(instance);
    const size_t str_len = string_length(string);

    if (str_len > instance->how_much || str_len < instance->how_much)
    {
        printf("Warning! Real string length = %ld, but defined how_much = %ld\n", str_len, instance->how_much);
    }

    if (str_len > instance->length || str_len < instance->length)
    {
        printf("Warning! Real string length = %ld, but defined length = %ld\n", str_len, instance->length);
    }
}

void kstring_insert(kstring_t** instance, size_t position, const char* string) {
    size_t string_len = string_length(string);
    kstring_t* dereferenced = *instance;
    if (position > dereferenced->length - 1) 
    {
        printf("Warning! Trying to insert string(%s) at position(%ld) but instance->length(%ld)\n", string, position, dereferenced->length);
    }

    if (position == dereferenced->length - 1)
    {
        kstring_append(instance, string);
    }

    // TODO Complete
}






int main(int argc, const char** argv)
{
    kstring_t* hello = kstring_dup("Hello");
    kstring_append(&hello, "W O R L DO    ");
    kstring_t* concated = kstring_concat(hello, hello);
    kstring_test(concated);
    kstring_test(hello);

    hello = kstring_slice(hello, 0, 5);
    kstring_test(hello);

    return 0;
}

