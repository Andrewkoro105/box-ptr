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
		
		BoxPtr(BoxPtr<T_, true> const& other) :
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
		
		auto operator=(BoxPtr&& box_ptr) noexcept -> BoxPtr&{
			std::cout << "move\n";
			if(&box_ptr == this)
				return *this;
			delete ptr;
			ptr = box_ptr.ptr;
			box_ptr.ptr = nullptr;
			return *this;
		}
		
		auto operator=(BoxPtr const& box_ptr) noexcept -> BoxPtr&{
			if(&box_ptr == this)
				return *this;
			delete ptr;
			if constexpr(std::is_abstract_v<T_>)
				ptr = box_ptr.ptr->copy();
			else
				ptr = new T_{*box_ptr.ptr};
			return *this;
		}
		
		template<typename R, bool Copyable, typename = std::enable_if_t<std::is_base_of_v<T_, R> > >
		auto operator==(BoxPtr<R, Copyable> const& box_ptr) const -> bool{
			return ptr == box_ptr.ptr;
		}
		
		auto reset(T_* ptr) -> void {
			delete this->ptr;
			this->ptr = ptr;
		}
		
		auto set(T_* ptr) -> void {
			this->ptr = ptr;
		}
		
		auto get() const -> T_*{
			return this->ptr;
		}
		
		auto operator*() const -> std::add_lvalue_reference_t<T_>{
			return *this->ptr;
		}
		
		auto operator->() const -> T_*{
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
		
		BoxPtr(BoxPtr<T_, false> const& other) = delete;
		
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
		
		BoxPtr& operator=(BoxPtr const& box_ptr) = delete;
		
		template<typename R, bool Copyable, typename = std::enable_if_t<std::is_base_of_v<T_, R> > >
		auto operator==(BoxPtr<R, Copyable> const& box_ptr) const -> bool{
			return ptr == box_ptr.ptr;
		}
		
		auto reset(T_* ptr) -> void {
			delete this->ptr;
			this->ptr = ptr;
		}
		
		auto set(T_* ptr) -> void {
			this->ptr = ptr;
		}
		
		auto get() const -> T_*{
			return this->ptr;
		}
		
		auto operator*() const -> std::add_lvalue_reference_t<T_>{
			return *this->ptr;
		}
		
		auto operator->() const -> T_*{
			return this->ptr;
		}
		
		~BoxPtr() {
			delete this->ptr;
		}
	};
	
	template<typename R, typename T = R, typename ...As>
	auto make_box_ptr(As&& ... args) -> BoxPtr<R>{
		return BoxPtr<R>{new T{std::forward<As>(args)...}};
	}
	
	template<typename R, typename T>
	auto dynamic_box_cast(BoxPtr<T>&& box_ptr) -> BoxPtr<R> {
		R* r = dynamic_cast<R*>(box_ptr.get());
		box_ptr.set(nullptr);
		return BoxPtr<R>{r};
	}
}
