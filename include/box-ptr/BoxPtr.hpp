#pragma once

#include <memory>

namespace bp {
	namespace detail {
		template<typename T>
		static auto check_dyn_copyable_helper(T&&) -> decltype(std::declval<T>()->copy(), std::true_type{}) {
			return {};
		};
		
		[[maybe_unused]] static std::false_type check_dyn_copyable_helper(...) {
			return {};
		};
		
		template<typename T>
		constexpr auto check_dyn_copyable = std::is_same_v<std::true_type, decltype(check_dyn_copyable_helper(std::declval<T>()))>;
	}
	
	
	template<typename T>
	class BoxPtr {
	protected:
		T* ptr;
	
	public:
		explicit BoxPtr(T* ptr = nullptr) : ptr(ptr) {
		}
		
		BoxPtr(const BoxPtr<T>& other) :
			BoxPtr([&]() -> T* {
				if(other.ptr) {
					if constexpr(std::is_abstract_v<T> && detail::check_dyn_copyable<T*>)
						return other.ptr->copy();
					else
						return new T{*other.ptr};
				} else {
					return nullptr;
				}
			}()) {
		}
		
		BoxPtr(BoxPtr<T>&& other) noexcept : BoxPtr(other.ptr) {
			if(&other != this)
				other.ptr = nullptr;
		}
		
		template<typename R, typename = std::enable_if_t<std::is_base_of_v<T, R>, void> >
		BoxPtr(BoxPtr<R>&& other) : BoxPtr(other.get()) {
			other.set(nullptr);
		}
		
		void reset(T* ptr_) {
			delete ptr;
			ptr = ptr_;
		}
		
		void set(T* ptr_) {
			ptr = ptr_;
		}
		
		T* get() const {
			return ptr;
		}
		
		BoxPtr<T>& operator=(BoxPtr<T>&& box_ptr) noexcept {
			if(&box_ptr == this)
				return *this;
			delete ptr;
			ptr = box_ptr.ptr;
			box_ptr.ptr = nullptr;
			return *this;
		}
		
		BoxPtr<T>& operator=(const BoxPtr<T>& box_ptr) noexcept {
			if(&box_ptr == this)
				return *this;
			delete ptr;
			if constexpr(std::is_abstract_v<T>)
				ptr = box_ptr.ptr->copy();
			else
				ptr = new T{*box_ptr.ptr};
			return *this;
		}
		
		std::add_lvalue_reference_t<T> operator*() const {
			return *ptr;
		}
		
		T* operator->() const {
			return ptr;
		}
		
		~BoxPtr() {
			delete ptr;
		}
	};
	
	template<typename R, typename T = R, typename ...As>
	BoxPtr<R> make_box_ptr(As&& ... args) {
		return BoxPtr<R>{new T{std::forward<As>(args)...}};
	}
	
	template<typename R, typename T>
	BoxPtr<R> dynamic_box_cast(BoxPtr<T>&& box_ptr) {
		R* r = dynamic_cast<R*>(box_ptr.get());
		box_ptr.set(nullptr);
		return BoxPtr<R>{r};
	}
}
