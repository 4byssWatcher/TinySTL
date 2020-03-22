## TinySTL

参考SGI-STL V3.3，部分借助新特性改写



本项目包含：

* 仿SGI-STL的内存池allocator

* 能管理数组的shared_ptr
* polymorphic_allocator(的没加construct实现版本)

* 没加新东西(除了右值相关)的其他组件



剩余计划：

* rbtree / map / set
* string, 可能会往 qstring 方向实现



编译环境: VS2019 

语言环境: C++17

采用VS的单元测试，仅保留其中cpp文件。预定改为更轻便的catch2



已测试：

* vector
* deque
* unique_ptr / shared_ptr