#pragma once 

#include "ShPlatformDefines.h"
#include "ShCommonDefines.h"
#include "ShStandardHeaders.h"
#include "ShTypes.h"

SH_NAMESPACE_BEGIN

/* pointer type (64/32 platforms are different) */
#if defined(WIN32)
	typedef unsigned __int32 uptr_t;
#else
	typedef unsigned __int64 uptr_t;
#endif

class MemoryAllocatorBase
{
public:
	static void* Allocate(size_t size)
	{
		void* ptr = ::malloc(size + sizeof(size_t));
		if (!ptr)
			return nullptr;

		*((size_t*)ptr) = size;
		return ((uint8_t*)ptr + sizeof(size_t));
	}

	static void* AllocateAligned(size_t size, uint8_t alignment)
	{
		size_t ns = size + alignment;
		uptr_t rawAddr = (uptr_t)Allocate(ns);

		if (!rawAddr)
			return nullptr;

		uptr_t misAlign = rawAddr & (alignment - 1);
		uint8_t adjust = alignment - (uint8_t)misAlign;
		uptr_t alignedAddr = rawAddr + adjust;
		uint8_t* a = (uint8_t*)(alignedAddr - sizeof(uint8_t));
		*a = adjust;

		return (void*)alignedAddr;
	}

	static void* AllocateAligned16(size_t bytes)
	{
		return AllocateAligned(bytes, 16);
	}

	static void Deallocate(void* ptr)
	{
		void* ptrReal = ((uint8_t*)ptr - sizeof(size_t));
		::free(ptrReal);
	}

	static void DeallocateAligned(void* ptr)
	{
		uptr_t alignedAddr = (uptr_t)ptr;
		uint8_t adjust = *((uint8_t*)(alignedAddr - sizeof(uint8_t)));
		uptr_t rawAddr = alignedAddr - adjust;
		Deallocate((void*)rawAddr);
	}

	static void DeallocateAligned16(void* ptr)
	{
		DeallocateAligned(ptr);
	}
};

inline void* SH_Allocate(size_t size)
{
	return MemoryAllocatorBase::Allocate(size);
}

template<class _Type>
_Type* SH_AllocateTyped()
{
	return (_Type*)MemoryAllocatorBase::Allocate(sizeof(_Type));
}

inline void SH_Free(void* ptr)
{
	MemoryAllocatorBase::Deallocate(ptr);
}

template<class _Type, class... _Args>
_Type* SH_New(_Args&&... args)
{
	return ::new (SH_AllocateTyped<_Type>()) _Type(std::forward<_Args>(args)...);
}

template<class _Type>
void SH_Delete(_Type* ptr)
{
	(ptr)->~_Type();
	SH_Free(ptr);
}

template<class _Type>
struct Deleter
{
	constexpr Deleter() noexcept = default;

	template <class _Type2, std::enable_if_t<std::is_convertible<_Type2*, _Type*>::value, int32> = 0>
	constexpr Deleter(const Deleter<_Type2>& other) noexcept { }

	void operator()(_Type* ptr) const
	{
		SH_Delete<_Type>(ptr);
	}
};

template<class _Type>
_Type* SH_NewA(size_t count)
{
	_Type* ptr = (_Type*)Allocate(sizeof(_Type) * count);
	for (size_t i = 0; i < count; ++i)
		::new (&ptr[i]) _Type;

	return ptr;
}

template<class _Type>
void SH_DeleteA(_Type* ptr, size_t count)
{
	for (size_t i = 0; i < count; ++i)
		ptr[i].~_Type();

	SH_Free(ptr);
}

template< class _Type >
class StdAlloc
{
public:
	using value_type = _Type;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	constexpr StdAlloc() = default;
	constexpr StdAlloc(StdAlloc&&) = default;
	constexpr StdAlloc(const StdAlloc&) = default;
	template<class _Type1> constexpr StdAlloc(const StdAlloc<_Type1>&) { };
	template<class _Type1> constexpr bool operator==(const StdAlloc<_Type1>&) const noexcept { return true; }
	template<class _Type1> constexpr bool operator!=(const StdAlloc<_Type1>&) const noexcept { return false; }

	template<class _Type1> class rebind { public: using other = StdAlloc<_Type1>; };

	_Type* allocate(const size_t num)
	{
		if (num == 0)
			return nullptr; // Error

		if (num > std::numeric_limits<size_t>::max() / sizeof(_Type))
			return nullptr; // Error

		void* const pAllocated = SH_Allocate(num * sizeof(_Type));
		if (!pAllocated)
			return nullptr; // Error

		return static_cast<_Type*>(pAllocated);
	}

	void deallocate(pointer p, size_type)
	{
		SH_Free(p);
	}

	static constexpr size_t max_size() { return std::numeric_limits<size_type>::max() / sizeof(_Type); }
	static constexpr void destroy(pointer p) { p->~_Type(); }

	template<class... Args>
	static void construct(pointer p, Args&&... args) { ::new(p) _Type(std::forward<Args>(args)...); }
};

template <typename _Type, typename... _Args>
SharedPtr<_Type> SH_MakeShared(_Args &&... args)
{
	return std::allocate_shared<_Type>(StdAlloc<_Type>(), std::forward<_Args>(args)...);
}

template<typename _Type, typename Delete = Deleter<_Type>>
SharedPtr<_Type> SH_AssignShared(_Type* data, Delete del = Delete())
{
	return SharedPtr<_Type>(data, std::move(del), StdAlloc<_Type>());
}

SH_NAMESPACE_END