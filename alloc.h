/* stateless, pending modification */
#pragma once
#ifndef _TINYSTL_ALLOC_H_
#define _TINYSTL_ALLOC_H_

#ifndef THROW_BAD_ALLOC
#   include <stdio.h>
#   include <stdlib.h>
#   define THROW_BAD_ALLOC fprintf(stderr,"out of memory\n");exit(1)
#endif /* THROW_BAD_ALLOC */

#include "string.h"

namespace TinySTL
{
    enum { ALIGN = 8 };
    enum { MAX_BYTES = 128 };
    enum { NFREELISTS = (MAX_BYTES / ALIGN) };

    class malloc_alloc_template
    {
    private:
        /* pending modification for custom malloc_alloc_oom_handler */
        static void* oom_malloc(size_t)
        {
            THROW_BAD_ALLOC;
        }
        /* pending modification for custom malloc_alloc_oom_handler */
        static void* oom_realloc(void*, size_t)
        {
            THROW_BAD_ALLOC;
        }
    public:
        static void* allocate(size_t bytes)
        {
            void* ret = malloc(bytes);
            if (0 == ret)ret = oom_malloc(bytes);
            return ret;
        }
        static void deallocate(void* ptr, size_t /* bytes */)
        {
            free(ptr);
        }
        static void* reallocate(void* ptr, size_t old_sz, size_t new_sz)
        {
            void* ret = realloc(ptr, new_sz);
            if (0 == ret)ret = oom_realloc(ptr, new_sz);
            return ret;
        }

    };

    class default_alloc_template
    {
    private:
        static char* start_free;
        static char* end_free;
        static size_t heap_size;
        union obj
        {
            union obj* free_list_link;
            char client_data[1];
        };
        static obj* free_list[NFREELISTS];
    private:
        /* round up to a multiple of ALIGN(ALIGN==2^n) */
        static size_t round_up(size_t bytes)
        {
            return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
        }
        static size_t freelist_index(size_t bytes)
        {
            return (((bytes)+ALIGN - 1) / ALIGN - 1);
        }
        static char* chunk_alloc(size_t size, int& nobjs);
        static void* refill(size_t n);

    public:
        static void* allocate(size_t bytes)
        {
            void* ret = 0;

            if (bytes > (size_t)MAX_BYTES)
                ret = malloc_alloc_template::allocate(bytes);
            else
            {
                obj** my_free_list;
                my_free_list = free_list + freelist_index(bytes);
                obj* result = *my_free_list;
                if (0 == result)ret = refill(round_up(bytes));
                else
                {
                    *my_free_list = result->free_list_link;
                    ret = result;
                }
            }
            return ret;
        }
        static void deallocate(void* ptr, size_t bytes)
        {
            obj* q = (obj*)(ptr);

            if (bytes > (size_t)MAX_BYTES)
                malloc_alloc_template::deallocate(ptr, bytes);
            else
            {
                obj** my_free_list;
                my_free_list = free_list + freelist_index(bytes);
                q->free_list_link = *my_free_list;
                *my_free_list = q;
            }
        }
        static void* reallocate(void* ptr, size_t old_sz, size_t new_sz);
    };
}

#endif /* _TINYSTL_ALLOC_H_ */