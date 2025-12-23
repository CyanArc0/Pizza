#pragma once
#define $included$ctypes$array_t 1.1

#include <cstddef>

template<typename T,std::size_t size>
using array_t = T[size];