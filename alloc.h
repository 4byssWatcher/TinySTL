/* untested */
#pragma once
#ifndef _TINYSTL_ALLOC_H_
#define _TINYSTL_ALLOC_H_

#ifndef THROW_BAD_ALLOC
#   include <stdio.h>
#   include <stdlib.h>
#   define THROW_BAD_ALLOC fprintf(stderr,"out of memory\n");exit(1)
#endif /* THROW_BAD_ALLOC */

//#include "string.h"

enum {ALIGN=8};
enum {MAX_BYTES=128};
enum {NFREELISTS=(MAX_BYTES/ALIGN)};

namespace TinySTL
{
    class malloc_alloc_template
    {
    private:
        /* pending modification for custom malloc_alloc_oom_handler */
        static void *oom_malloc(size_t)
        {
            THROW_BAD_ALLOC;
        }
        /* pending modification for custom malloc_alloc_oom_handler */
	    static void *oom_realloc(void *, size_t)
        {
            THROW_BAD_ALLOC;
        }
    public:
        static void *allocate(size_t bytes)
        {
            void *ret=malloc(bytes);
            if(0==ret)ret=oom_malloc(bytes);
            return ret;
        }
		static void deallocate(void *ptr, size_t bytes)
        {   
            free(ptr);
        }
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz)
        {
            void *ret=realloc(ptr,new_sz);
            if(0==ret)ret=oom_realloc(ptr,new_sz);
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
            union obj *free_list_link;
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
        static char *chunk_alloc(size_t size, int& nobjs);
		static void *refill(size_t n);
		
    public:
        /* pending modification for support of multithreading */
        static void *allocate(size_t bytes)
        {
            void *ret=0;

            if(bytes>(size_t)MAX_BYTES)
                ret=malloc_alloc_template::allocate(bytes);
            else
            {
                obj** my_free_list;
                my_free_list=free_list+freelist_index(bytes);
                obj *result=*my_free_list;
                if(0==result)ret=refill(round_up(bytes));
                else 
                {
                    *my_free_list=result->free_list_link;
                    ret=result;
                }
            }
            return ret;
        }
		static void deallocate(void *ptr, size_t bytes)
        {
            obj *q=(obj *)(ptr);

            if(bytes>(size_t)MAX_BYTES)
                malloc_alloc_template::deallocate(ptr,bytes);
            else
            {
                obj** my_free_list;
                my_free_list=free_list+freelist_index(bytes);
                q->free_list_link=*my_free_list;
                *my_free_list=q;
            }
        }
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
    };

    void*
    default_alloc_template::reallocate(void *ptr, size_t old_sz, size_t new_sz)
    {
        void *ret;
        size_t copy_sz;

        if(old_sz>(size_t)MAX_BYTES && new_sz>(size_t)MAX_BYTES)
            return realloc(ptr,new_sz);
        if(round_up(old_sz)==round_up(new_sz))
            return ptr;
        ret=allocate(new_sz);
        copy_sz=new_sz>old_sz?old_sz:new_sz;
        //memcpy(ret,ptr,copy_sz);
        deallocate(ptr,old_sz);
        return ret;
    }

    char* 
    default_alloc_template::chunk_alloc(size_t size, int& nobjs)
    {
        char *ret;
        size_t total_bytes=size*nobjs;
        size_t left_bytes=end_free-start_free;

        if(left_bytes>=total_bytes)
        {
            ret=start_free;
            start_free+=total_bytes;
            return ret;
        }
        else if(left_bytes>=size)
        {
            nobjs=(int)(left_bytes/size);
            ret=start_free;
            start_free+=size*nobjs;
            return ret;
        }
        else
        {
            size_t bytes_to_get=2*total_bytes+round_up(heap_size>>4);
            /* make use of the left-over piece */
            if(left_bytes>0)
            {
                obj **my_free_list=free_list+freelist_index(left_bytes);
                ((obj *)start_free)->free_list_link=*my_free_list;
                *my_free_list=(obj *)start_free;
            }
            start_free=(char*)malloc(bytes_to_get);
            /* allocation failed,try to get memory from larger part of free_list */
            if(0==start_free)
            {
                obj **my_free_list;
                obj *p;
                for(size_t i=size;i<(size_t)MAX_BYTES;i+=(size_t)ALIGN)
                {
                    my_free_list=free_list+freelist_index(i);
                    p=*my_free_list;
                    if(0!=p)
                    {
                        *my_free_list=p->free_list_link;
                        start_free=(char *)p;//a new chunk
                        end_free=start_free+i;
                        /* try allocating again since there's a new chunk */
                        return chunk_alloc(size,nobjs);
                    }
                }
                /* in case of allocation failed,either throw an exception or remedy the situation*/
                end_free=0;
                start_free=(char *)malloc_alloc_template::allocate(bytes_to_get);
            }
            /* allocation succeed */
            heap_size+=bytes_to_get;
            end_free=start_free+bytes_to_get;
            return chunk_alloc(size,nobjs);
        }
        
    }

    void* 
    default_alloc_template::refill(size_t n)
    {
        int nobjs=20;
        char *chunk=chunk_alloc(n,nobjs);
        obj **my_free_list;
        obj *ret,*current_obj,*next_obj;

        if(1==nobjs)return chunk;
        my_free_list=free_list+freelist_index(n);
        ret=(obj *)chunk;
        *my_free_list=next_obj=(obj *)(chunk+n);
        for(int i=1;;++i)
        {
            current_obj=next_obj;
            next_obj=(obj *)((char *)next_obj+n);
            if(nobjs-i==1)
            {
                current_obj->free_list_link=0;
                break;
            }
            current_obj->free_list_link=next_obj;
        }
        return ret;
    }

    char* default_alloc_template::start_free=0;
    char* default_alloc_template::end_free=0;
    size_t default_alloc_template::heap_size=0;
    default_alloc_template::obj* default_alloc_template::free_list[NFREELISTS]={0};
}

#endif /* _TINYSTL_ALLOC_H_ */