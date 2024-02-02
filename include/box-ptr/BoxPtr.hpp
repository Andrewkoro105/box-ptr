#pragma once

#include <memory>
#include <type_traits>
#include <iostream>

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
		
		template<typename T>
		static constexpr auto copyable_v = (std::is_abstract_v<T> && detail::check_dyn_copyable<T*>) || std::is_copy_constructible_v<T>;
	}
	
	template<typename T_, bool Copyable_ = detail::copyable_v<T_> >
	class BoxPtr;
	
	template<typename T_>
	class BoxPtr<T_, true> {
	protected:
		T_* ptr;
	
	public:
		BoxPtr(T_* ptr = nullptr) : ptr(ptr) { // NOLINT(*-explicit-constructor)
		}
		
		BoxPtr(const BoxPtr<T_, true>& other) :
			BoxPtr([&]() -> T_* {
				if(other.ptr) {
					if constexpr(std::is_abstract_v<T_> && detail::check_dyn_copyable<T_*>)
						return other.ptr->copy();
					else
						return new T_{*other.ptr};
				} else {
					return nullptr;
				}
			}()) {
		}
		
		template<bool Copyable>
		BoxPtr(BoxPtr<T_, Copyable>&& other) noexcept : BoxPtr(other.ptr) {
			if(&other != this)
				other.ptr = nullptr;
		}
		
		template<typename R, bool Copyable, typename = std::enable_if_t<std::is_base_of_v<T_, R> > >
		BoxPtr(BoxPtr<R, Copyable>&& other) : BoxPtr(other.get()) {
			other.set(nullptr);
		}
		
		BoxPtr& operator=(BoxPtr&& box_ptr) noexcept {
			std::cout << "move\n";
			if(&box_ptr == this)
				return *this;
			delete ptr;
			ptr = box_ptr.ptr;
			box_ptr.ptr = nullptr;
			return *this;
		}
		
		BoxPtr& operator=(const BoxPtr& box_ptr) noexcept {
			if(&box_ptr == this)
				return *this;
			delete ptr;
			if constexpr(std::is_abstract_v<T_>)
				ptr = box_ptr.ptr->copy();
			else
				ptr = new T_{*box_ptr.ptr};
			return *this;
		}
		
		void reset(T_* ptr) {
			delete this->ptr;
			this->ptr = ptr;
		}
		
		void set(T_* ptr) {
			this->ptr = ptr;
		}
		
		T_* get() const {
			return this->ptr;
		}
		
		std::add_lvalue_reference_t<T_> operator*() const {
			return *this->ptr;
		}
		
		T_* operator->() const {
			return this->ptr;
		}
		
		~BoxPtr() {
			delete this->ptr;
		}
	};
	
	template<typename T_>
	class BoxPtr<T_, false> {
	protected:
		T_* ptr;
	
	public:
		explicit BoxPtr(T_* ptr = nullptr) : ptr(ptr) {
		}
		
		BoxPtr(const BoxPtr<T_, false>& other) = delete;
		
		template<bool Copyable>
		BoxPtr(BoxPtr<T_, Copyable>&& other) noexcept : BoxPtr(other.ptr) {
			if(&other != this)
				other.ptr = nullptr;
		}
		
		template<typename R, bool Copyable, typename = std::enable_if_t<std::is_base_of_v<T_, R> > >
		BoxPtr(BoxPtr<R, Copyable>&& other) : BoxPtr(other.get()) {
			other.set(nullptr);
		}
		
		BoxPtr& operator=(BoxPtr&& box_ptr) noexcept {
			if(&box_ptr == this)
				return *this;
			delete ptr;
			ptr = box_ptr.ptr;
			box_ptr.ptr = nullptr;
			return *this;
		}
		
		BoxPtr& operator=(const BoxPtr& box_ptr) = delete;
		
		void reset(T_* ptr) {
			delete this->ptr;
			this->ptr = ptr;
		}
		
		void set(T_* ptr) {
			this->ptr = ptr;
		}
		
		T_* get() const {
			return this->ptr;
		}
		
		std::add_lvalue_reference_t<T_> operator*() const {
			return *this->ptr;
		}
		
		T_* operator->() const {
			return this->ptr;
		}
		
		~BoxPtr() {
			delete this->ptr;
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
