#pragma once
#define $included$loop$for$FOR_SIZE 2.2

#include<type_traits>
#include"FOR.hpp"

#define FOR_SIZE($count) FOR(static_cast<std::decay_t<decltype(($count))>>(0),($count))