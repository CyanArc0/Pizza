#pragma once
#define $included$string_traits$struct_string 3.1

#include<cstddef>
#include<array>
#include<string_view>
#include<iterator>
#include<compare>
#include"ctypes/array_t.hpp"
#include"ctypes/cstring/cstring_t.hpp"

template<typename char_t,std::size_t s_size>
struct struct_string
{
    using value_type = char_t;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    //using iterator = pointer;
    struct iterator
    {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = char_t;
        using difference_type = std::ptrdiff_t;
        using pointer = char_t*;
        using reference = char_t&;
        pointer ptr;
        constexpr iterator& operator++() noexcept
        {
            ++ptr;
            return *this;
        }
        constexpr iterator operator++(int) noexcept
        {
            iterator tmp = *this;
            ++ptr;
            return tmp;
        }
        constexpr iterator& operator--() noexcept
        {
            --ptr;
            return *this;
        }
        constexpr iterator operator--(int) noexcept
        {
            iterator tmp = *this;
            --ptr;
            return tmp;
        }
        constexpr iterator operator+( difference_type n ) const noexcept
        {
            return iterator{ptr + n};
        }
        constexpr iterator operator-( difference_type n ) const noexcept
        {
            return iterator{ptr - n};
        }
        constexpr difference_type operator-( const iterator& other ) const noexcept
        {
            return ptr - other.ptr;
        }
        constexpr reference operator*() const noexcept
        {
            return *ptr;
        }
        constexpr pointer operator->() const noexcept
        {
            return ptr;
        }
        constexpr bool operator==( const iterator& other ) const noexcept
        {
            return ptr == other.ptr;
        }
        constexpr bool operator!=( const iterator& other ) const noexcept
        {
            return ptr != other.ptr;
        }
        constexpr bool operator<( const iterator& other ) const noexcept
        {
            return ptr < other.ptr;
        }
        constexpr bool operator<=( const iterator& other ) const noexcept
        {
            return ptr <= other.ptr;
        }
        constexpr bool operator>( const iterator& other ) const noexcept
        {
            return ptr > other.ptr;
        }
        constexpr bool operator>=( const iterator& other ) const noexcept
        {
            return ptr >= other.ptr;
        }
    };
    //using const_iterator = const_pointer;
    struct const_iterator
    {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const char_t;
        using difference_type = std::ptrdiff_t;
        using pointer = const char_t*;
        using reference = const char_t&;
        pointer ptr;
        constexpr const_iterator& operator++() noexcept
        {
            ++ptr;
            return *this;
        }
        constexpr const_iterator operator++(int) noexcept
        {
            const_iterator tmp = *this;
            ++ptr;
            return tmp;
        }
        constexpr const_iterator& operator--() noexcept
        {
            --ptr;
            return *this;
        }
        constexpr const_iterator operator--(int) noexcept
        {
            const_iterator tmp = *this;
            --ptr;
            return tmp;
        }
        constexpr const_iterator operator+( difference_type n ) const noexcept
        {
            return const_iterator{ptr + n};
        }
        constexpr const_iterator operator-( difference_type n ) const noexcept
        {
            return const_iterator{ptr - n};
        }
        constexpr difference_type operator-( const const_iterator& other ) const noexcept
        {
            return ptr - other.ptr;
        }
        constexpr reference operator*() const noexcept
        {
            return *ptr;
        }
        constexpr pointer operator->() const noexcept
        {
            return ptr;
        }
        constexpr bool operator==( const const_iterator& other ) const noexcept
        {
            return ptr == other.ptr;
        }
        constexpr bool operator!=( const const_iterator& other ) const noexcept
        {
            return ptr != other.ptr;
        }
        constexpr bool operator<( const const_iterator& other ) const noexcept
        {
            return ptr < other.ptr;
        }
        constexpr bool operator<=( const const_iterator& other ) const noexcept
        {
            return ptr <= other.ptr;
        }
        constexpr bool operator>( const const_iterator& other ) const noexcept
        {
            return ptr > other.ptr;
        }
        constexpr bool operator>=( const const_iterator& other ) const noexcept
        {
            return ptr >= other.ptr;
        }
    };
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    std::array<char_t,s_size+1> array;
    consteval struct_string(array_t<const char_t,s_size+1>const& s)noexcept : array(std::to_array(s)) {}
    constexpr operator std::basic_string_view<char_t>()const noexcept
    {
        return std::basic_string_view<char_t>(array.data(),s_size);
    }
    constexpr cstring_t c_str()const noexcept
    {
        return array.data();
    }
    constexpr iterator begin() noexcept
    {
        return array.data();
    }
    constexpr const_iterator begin() const noexcept
    {
        return array.data();
    }
    constexpr const_iterator cbegin() const noexcept
    {
        return array.data();
    }
    constexpr iterator end() noexcept
    {
        return array.data() + s_size;
    }
    constexpr const_iterator end() const noexcept
    {
        return array.data() + s_size;
    }
    constexpr const_iterator cend() const noexcept
    {
        return array.data() + s_size;
    }
    constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }
    constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }
    constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    constexpr reference operator[]( size_type pos )
    {
        return array[pos];
    }
    constexpr const_reference operator[]( size_type pos ) const
    {
        return array[pos];
    }
    constexpr reference at( size_type pos )
    {
        return array.at(pos);
    }
    constexpr const_reference at( size_type pos ) const
    {
        return array.at(pos);
    }
    constexpr reference front()
    {
        return array[0];
    }
    constexpr const_reference front() const
    {
        return array[0];
    }
    constexpr reference back()
    {
        return array[s_size - 1];
    }
    constexpr const_reference back() const
    {
        return array[s_size - 1];
    }
    constexpr pointer data() noexcept
    {
        return array.data();
    }
    constexpr const_pointer data() const noexcept
    {
        return array.data();
    }
    constexpr size_type size()const noexcept
    {
        return s_size;
    }
    constexpr size_type length()const noexcept
    {
        return s_size;
    }
    constexpr size_type max_size() const noexcept
    {
        return s_size;
    }
    constexpr bool empty() const noexcept
    {
        return s_size == 0;
    }
};
template<typename char_t, std::size_t ls_size, std::size_t rs_size>
constexpr bool operator==( const struct_string<char_t,ls_size>& ls,const struct_string<char_t,rs_size>& rs )
{
    if constexpr( ls_size == rs_size )
    {
        return ls.array == rs.array;
    }
    else return false;
}
template<typename char_t, std::size_t ls_size, std::size_t rs_size>
constexpr std::strong_ordering operator<=>( const struct_string<char_t,ls_size>& ls,const struct_string<char_t,rs_size>& rs )
{
    return static_cast<std::basic_string_view<char_t>>(ls) <=> static_cast<std::basic_string_view<char_t>>(rs);
}
template<typename char_t,std::size_t a_size>
struct_string(const char_t(&)[a_size]) -> struct_string<char_t,a_size-1>;