#pragma once

#include <string>

SH_NAMESPACE_BEGIN

template <typename _Type>
using BasicString = std::basic_string < _Type, std::char_traits<_Type>, StdAlloc<_Type> >;
using String = BasicString<char>;

SH_NAMESPACE_END