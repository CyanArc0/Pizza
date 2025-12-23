#pragma once
#define $included$string_traits$char_type 1.2

#include<type_traits>

template<typename char_t>
concept char_type
     = std::is_same_v<char_t,char>
    or std::is_same_v<char_t,wchar_t>
    or std::is_same_v<char_t,char8_t>
    or std::is_same_v<char_t,char16_t>
    or std::is_same_v<char_t,char32_t>;