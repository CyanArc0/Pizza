#pragma once
#define $included$ctypes$cstring$basic_cstring_t 2.0

#include"string_traits/char_type.hpp"

template<char_type char_t>
using basic_cstring_t = char_t const*;