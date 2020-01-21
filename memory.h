#pragma once
#ifndef _TINYSTL_MEMORY_H_
#define _TINYSTL_MEMORY_H_

namespace TinySTL
{
    template<typename T>
    class default_deleter
    {
        void operator ()(T* ptr){if(ptr)delete ptr;}
    };
    /* for array */
    template<typename T>
    class default_deleter<T []>
    {
        void operator ()(T* ptr){if(ptr)delete[] ptr;}
    }

    template<typename T,typename D=default_deleter<T> >
    class unique_ptr
    {
    public:
		typedef  T    element_type;
		typedef  D    deleter_type;
		typedef  T*   pointer;
    private:
        pointer       data;
        deleter_type  deleter;

    public:
        explicit unique_ptr(T* _data=nullptr):data(_data){}
        unique_ptr(T* _data,D _del):data(_data),deleter(_del){}
        unique_ptr(unique_ptr && _up)
        {
            data=std::move(_up.data);
        }
        unique_ptr& operator = (unique_ptr && _up)
        {
            if(&_up!=this)
            {
                clean();
                this->data=std::move(_up.data);
            }
            return *this;
        }
    private:
        void clean()
        {
            deleter(data);
            data=nullptr;
        }
    };
}

#endif /* _TINYSTL_MEMORY_H_ */