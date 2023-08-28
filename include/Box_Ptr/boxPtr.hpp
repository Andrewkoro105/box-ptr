#pragma once

#include <memory>

namespace bp {
	namespace detail {
		template<typename T>
		static auto checkDynCopyableHelper(T&&) -> decltype(std::declval<T>()->copy(), std::true_type{}) {
			return {};
		};
		
		[[maybe_unused]] static std::false_type checkDynCopyableHelper(...) {
			return {};
		};
		
		template<typename T>
		constexpr auto checkDynCopyable = std::is_same_v<std::true_type, decltype(checkDynCopyableHelper(std::declval<T>()))>;
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
					if constexpr(std::is_abstract_v<T> && detail::checkDynCopyable<T*>)
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
		
		BoxPtr<T>& operator=(BoxPtr<T>&& boxPtr) noexcept {
			if(&boxPtr == this)
				return *this;
			delete ptr;
			ptr = boxPtr.ptr;
			boxPtr.ptr = nullptr;
			return *this;
		}
		
		BoxPtr<T>& operator=(const BoxPtr<T>& boxPtr) noexcept {
			if(&boxPtr == this)
				return *this;
			delete ptr;
			if constexpr(std::is_abstract_v<T>)
				ptr = boxPtr.ptr->copy();
			else
				ptr = new T{*boxPtr.ptr};
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
	BoxPtr<R> makeBoxPtr(As&& ... args) {
		return BoxPtr<R>{new T{std::forward<As>(args)...}};
	}
	
	template<typename R, typename T>
	BoxPtr<R> dynamicCast(BoxPtr<T>&& boxPtr) {
		R* r = dynamic_cast<R*>(boxPtr.get());
		boxPtr.set(nullptr);
		return BoxPtr<R>{r};
	}
}
