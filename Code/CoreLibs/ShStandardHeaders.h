#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

#include <memory>

// STL containers
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <set>
#include <list>
#include <forward_list>
#include <deque>
#include <queue>
#include <bitset>
#include <array>

#include <unordered_map>
#include <unordered_set>

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>
#include <iterator>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

extern "C" 
{
#include <sys/types.h>
#include <sys/stat.h>
}

#if SH_PLATFORM == SH_PLATFORM_MSW
#  undef min
#  undef max
#	if !defined(NOMINMAX) && defined(_MSC_VER)
#		define NOMINMAX 
#	endif
#endif

#include "ShMemoryAllocator.h"

SH_NAMESPACE_BEGIN

struct EnumClassHash { template <typename _Type> constexpr std::size_t operator()(_Type t) const { return static_cast<std::size_t>(t); } };

template <typename _Key>
using HashType = typename std::conditional<std::is_enum<_Key>::value, EnumClassHash, std::hash<_Key>>::type;

template <typename _Type, typename _Alloc = StdAlloc<_Type>>
using Deque = std::deque<_Type, _Alloc>;

template <typename _Type, typename _Alloc = StdAlloc<_Type>>
using Array = std::vector<_Type, _Alloc>;

template <typename _Type, typename _Alloc = StdAlloc<_Type>>
using List = std::list<_Type, _Alloc>;

template <typename _Type, typename _Alloc = StdAlloc<_Type>>
using ForwardList = std::forward_list<_Type, _Alloc>;

template <typename _Type, typename _Alloc = StdAlloc<_Type>>
using Stack = std::stack<_Type, std::deque<_Type, _Alloc>>;

template <typename _Type, typename _Alloc = StdAlloc<_Type>>
using Queue = std::queue<_Type, std::deque<_Type, _Alloc>>;

template <typename _Type, typename _Predicate = std::less<_Type>, typename _Alloc = StdAlloc<_Type>>
using Set = std::set<_Type, _Predicate, _Alloc>;

template <typename _Key, typename _Value, typename _Predicate = std::less<_Key>, typename _Alloc = StdAlloc<std::pair<const _Key, _Value>>>
using Map = std::map<_Key, _Value, _Predicate, _Alloc>;

template <typename _Type, typename _Predicate = std::less<_Type>, typename _Alloc = StdAlloc<_Type>>
using MultiSet = std::multiset<_Type, _Predicate, _Alloc>;

template <typename _Key, typename _Value, typename _Predicate = std::less<_Key>, typename _Alloc = StdAlloc<std::pair<const _Key, _Value>>>
using MultiMap = std::multimap<_Key, _Value, _Predicate, _Alloc>;

template <typename _Type, typename _Hash = HashType<_Type>, typename _Cmp = std::equal_to<_Type>, typename _Alloc = StdAlloc<_Type>>
using UnorderedSet = std::unordered_set<_Type, _Hash, _Cmp, _Alloc>;

template <typename _Key, typename _Value, typename _Hash = HashType<_Key>, typename _Cmp = std::equal_to<_Key>, typename _Alloc = StdAlloc<std::pair<const _Key, _Value>>>
using UnorderedMap = std::unordered_map<_Key, _Value, _Hash, _Cmp, _Alloc>;

template <typename _Key, typename _Value, typename _Hash = HashType<_Key>, typename _Cmp = std::equal_to<_Key>, typename _Alloc = StdAlloc<std::pair<const _Key, _Value>>>
using UnorderedMultimap = std::unordered_multimap<_Key, _Value, _Hash, _Cmp, _Alloc>;

template <typename _Type>
using SharedPtr = std::shared_ptr<_Type>;

template <typename _Type>
using WeakPtr = std::weak_ptr<_Type>;

template <typename _Type, typename _Delete = Deleter<_Type>>
using UniquePtr = std::unique_ptr<_Type, _Delete>;

SH_NAMESPACE_END