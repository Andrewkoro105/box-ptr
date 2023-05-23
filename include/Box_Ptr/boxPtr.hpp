#pragma once

#include <memory>
namespace bp {
	template<typename T>
	class BoxPtr{
	protected:
		T* ptr;
		
	public:
		explicit BoxPtr(T* ptr) : ptr(ptr) {
		}
		
		BoxPtr(const BoxPtr<T>& boxPtr) : BoxPtr(new T{*boxPtr.ptr}){
		}
		
		BoxPtr(BoxPtr<T>&& boxPtr) noexcept : BoxPtr(boxPtr.ptr){
			if (&boxPtr != this)
				boxPtr.ptr = nullptr;
		}
		
		void reset(T* ptr_){
			delete ptr;
			ptr = ptr_;
		}
		
		T* get() const{
			return ptr;
		}
		
		BoxPtr<T>& operator=(BoxPtr<T>&& boxPtr) noexcept {
			if (&boxPtr == this)
				return  *this;
			delete ptr;
			ptr = boxPtr.ptr;
			boxPtr.ptr = nullptr;
			return *this;
		}
		
		BoxPtr<T>& operator=(const BoxPtr<T>& boxPtr) noexcept {
			if (&boxPtr == this)
				return  *this;
			delete ptr;
			ptr = new T{*boxPtr.ptr};
			return *this;
		}
		
		std::add_lvalue_reference_t<T> operator*() const{
			return *ptr;
		}
		
		T* operator->() const{
			return ptr;
		}
		
		~BoxPtr(){
			delete ptr;
		}
	};
	
	template<typename R, typename T = R, typename ...As>
	BoxPtr<R> makeBoxPtr(As&&... args){
		return BoxPtr<R>{new T{std::forward<As>(args)...}};
	}
	
}
