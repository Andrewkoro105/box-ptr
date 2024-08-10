#pragma once

#include <memory>
#include <type_traits>
#include <iostream>
#include <hash_fun.h>

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
	public:
		BoxPtr(T_* ptr = nullptr) : ptr_(ptr) { // NOLINT(*-explicit-constructor)
		}
		
		BoxPtr(BoxPtr<T_, true> const& other) :
			BoxPtr([&]() -> T_* {
				if(other.ptr_) {
					if constexpr(std::is_abstract_v<T_> && detail::check_dyn_copyable<T_*>)
						return other.ptr_->copy();
					else
						return new T_{*other.ptr_};
				} else {
					return nullptr;
				}
			}()) {
		}
		
		template<bool Copyable>
		BoxPtr(BoxPtr<T_, Copyable>&& other) noexcept : BoxPtr(other.ptr_) {
			if(&other != this)
				other.ptr_ = nullptr;
		}
		
		template<typename R, bool Copyable, typename = std::enable_if_t<std::is_base_of_v<T_, R> > >
		BoxPtr(BoxPtr<R, Copyable>&& other) : BoxPtr(other.get()) {
			other.set(nullptr);
		}
		
		auto operator=(BoxPtr&& box_ptr) noexcept -> BoxPtr&{
			if(&box_ptr == this)
				return *this;
			delete ptr_;
			ptr_ = box_ptr.ptr_;
			box_ptr.ptr_ = nullptr;
			return *this;
		}
		
		auto operator=(BoxPtr const& box_ptr) noexcept -> BoxPtr&{
			if(&box_ptr == this)
				return *this;
			delete ptr_;
			if constexpr(std::is_abstract_v<T_>)
				ptr_ = box_ptr.ptr_->copy();
			else
				ptr_ = new T_{*box_ptr.ptr_};
			return *this;
		}
		
		template<typename R, bool Copyable, typename = std::enable_if_t<std::is_base_of_v<T_, R> > >
		auto operator==(BoxPtr<R, Copyable> const& box_ptr) const -> bool{
			return ptr_ == box_ptr.ptr_;
		}
		
		auto reset(T_* ptr) -> void {
			delete this->ptr_;
			this->ptr_ = ptr;
		}
		
		auto set(T_* ptr) -> void {
			this->ptr_ = ptr;
		}
		
		auto get() const -> T_*{
			return this->ptr_;
		}
		
		auto operator*() const -> std::add_lvalue_reference_t<T_>{
			return *this->ptr_;
		}
		
		auto operator->() const -> T_*{
			return this->ptr_;
		}
		
		~BoxPtr() {
			delete this->ptr_;
		}
	
	protected:
		T_* ptr_;
	};
	
	template<typename T_>
	class BoxPtr<T_, false> {
	public:
		explicit BoxPtr(T_* ptr = nullptr) : ptr_(ptr) {
		}
		
		BoxPtr(BoxPtr<T_, false> const& other) = delete;
		
		template<bool Copyable>
		BoxPtr(BoxPtr<T_, Copyable>&& other) noexcept : BoxPtr(other.ptr_) {
			if(&other != this)
				other.ptr_ = nullptr;
		}
		
		template<typename R, bool Copyable, typename = std::enable_if_t<std::is_base_of_v<T_, R> > >
		BoxPtr(BoxPtr<R, Copyable>&& other) : BoxPtr(other.get()) {
			other.set(nullptr);
		}
		
		BoxPtr& operator=(BoxPtr&& box_ptr) noexcept {
			if(&box_ptr == this)
				return *this;
			delete ptr_;
			ptr_ = box_ptr.ptr_;
			box_ptr.ptr_ = nullptr;
			return *this;
		}
		
		BoxPtr& operator=(BoxPtr const& box_ptr) = delete;
		
		template<typename R, bool Copyable, typename = std::enable_if_t<std::is_base_of_v<T_, R> > >
		auto operator==(BoxPtr<R, Copyable> const& box_ptr) const -> bool{
			return ptr_ == box_ptr.ptr_;
		}
		
		auto reset(T_* ptr) -> void {
			delete this->ptr_;
			this->ptr_ = ptr;
		}
		
		auto set(T_* ptr) -> void {
			this->ptr_ = ptr;
		}
		
		auto get() const -> T_*{
			return this->ptr_;
		}
		
		auto operator*() const -> std::add_lvalue_reference_t<T_>{
			return *this->ptr_;
		}
		
		auto operator->() const -> T_*{
			return this->ptr_;
		}
		
		~BoxPtr() {
			delete this->ptr_;
		}
	
	protected:
		T_* ptr_;
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

template<typename T_>
struct std::hash<bp::BoxPtr<T_> >{
	auto operator()(bp::BoxPtr<T_> const& box_ptr) const -> size_t{
		return std::hash<T_>{}(*box_ptr);
	}
};
