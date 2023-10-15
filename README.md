# Description
BoxPtr is a smart pointer (similar to std::unique_ptr) that owns the object and if you delete the pointer, the object under it will be deleted, unlike std::unique_ptr, BoxPtr can be copied (the object by the pointer will also be copied)
# API Documentation
## BoxPtr
| member function                               | description                                                                                                                                 |
|-----------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------|
| void reset(T* ptr_ = nullptr)                 | changes the object to which the pointer points, deleting the previous one                                                                   |
| T* get()                                      | returns a raw pointer to its object                                                                                                         |
| BoxPtr<T>& operator=(BoxPtr<T>&& box_ptr)      | moves the passed object into itself making it non-working and deleting its old one                                                          |
| BoxPtr<T>& operator=(const BoxPtr<T>& box_ptr) | copies the passed object into itself, deleting its old one (if the type is abstract and has a `copy()` function, it will be called to copy) |
| std::add_lvalue_reference_t<T> operator*()    | returns an object within itself                                                                                                             |
| T* operator->()                               | returns an object within itself                                                                                                             |

## function
| functions                           | description                                                                                                     |
|-------------------------------------|-----------------------------------------------------------------------------------------------------------------|
| BoxPtr\<R> make_box_ptr(As&&... args) | Creates a new object of type `T` that is equal to or inherits from type `R` and creates a BoxPtr pointing to it |