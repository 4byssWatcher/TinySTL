#ifndef TINYSTL_ALLOC_H
#define TINYSTL_ALLOC_H

enum {ALIGN=8};
enum {MAX_BYTES=128};
enum {NFREELISTS=MAX_BYTES/ALIGN};

namespace TinySTL
{
    class malloc_alloc_template
    {
    private:
        static void *oom_malloc(size_t);
	    static void *oom_realloc(void *, size_t);
    public:
        static void *allocate(size_t bytes)
        {
            void *ret=malloc(bytes);
            if(0==ret)ret=oom_malloc();
            return ret;
        }
		static void deallocate(void *ptr, size_t bytes)
        {   
            free(*ptr);
        }
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz)
        {
            void *ret=realloc(ptr,new_sz);
            if(0==ret)ret=oom_realloc(p,new_sz);
            return ret;
        }

    }

    class default_alloc_template
    {
    private:
        union obj
        {
            union obj *free_list_link;
            char client_data[1];
        };
    private:
        obj *free_list[NFREELISTS];
    private:
        /* round up to a multiple of ALIGN(ALIGN==2^n) */
        static size_t ROUND_UP(size_t bytes)
        {
			return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
		}
		static size_t FREELIST_INDEX(size_t bytes)
        {
			return (((bytes)+ALIGN - 1) / ALIGN - 1);
		}
		static void *refill(size_t n);
		static char *chunk_alloc(size_t size, size_t& nobjs);
    public:
        static void *allocate(size_t bytes)
        {
            void *ret=0;
            obj *my_free_list;
            if(bytes>(size_t)MAX_BYTES)
                ret=malloc_alloc_template::allocate(bytes);
            else
            {
                my_free_list=free_list+FREELIST_INDEX(bytes);
                ret=*my_free_list;
                if(0==ret)ret=refill(ROUND_UP(bytes));
                else *my_free_list=ret->free_list_link;
            }
            return ret;
        }
		static void deallocate(void *ptr, size_t bytes)
        {
            obj *q=ptr;
            if(bytes>(size_t)MAX_BYTES)
                malloc_alloc_template::deallocate(ptr,bytes);
            else
            {
                my_free_list=free_list+FREELIST_INDEX(bytes);
                q->free_list_link=*my_free_list;
                *my_free_list=q;
            }
        }
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
    }
}

#endif /* TINYSTL_ALLOC_H */