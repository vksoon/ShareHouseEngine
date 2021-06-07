#pragma once

#include <string>

SH_NAMESPACE_BEGIN

template <typename _Type>
using BasicString = std::basic_string < _Type, std::char_traits<_Type>, StdAlloc<_Type> >;
using String = BasicString<char>;

template <typename _Type>
using BasicStringStream = std::basic_stringstream < _Type, std::char_traits<_Type>, StdAlloc<_Type> >;
using StringStream = BasicStringStream<char>;

SH_NAMESPACE_END