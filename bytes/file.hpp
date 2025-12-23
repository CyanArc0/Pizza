#pragma once
#define $included$bytes$file 2.2

#include<filesystem>
#include<cstdio>
#include<utility>
#include"ctypes/cstring/basic_cstring_t.hpp"
#include"file_view.hpp"

#include"_fopenU.ipp"
struct file : file_view
{
public:
    enum class open_mode{readonly=0b0000,writeonly=0b0001,overwrite=0b0010,append=0b0011,inout=0b0100,text=0b1000};
    using enum open_mode;
    friend inline constexpr open_mode operator|(open_mode a,open_mode b)noexcept
    {
        return static_cast<open_mode>(std::to_underlying(a)|std::to_underlying(b));
    }
protected:
    inline static constexpr basic_cstring_t<Uchar_t> c_str(open_mode mode)noexcept
    {
    switch_begin:
        switch(mode)
        {
        case open_mode::readonly:
            return Ucstr("rb");
        case open_mode::writeonly:
            return Ucstr("wbx");
        case open_mode::overwrite:
            return Ucstr("wb");
        case open_mode::append:
            return Ucstr("ab");
        case open_mode::readonly|open_mode::inout:
            return Ucstr("r+b");
        case open_mode::writeonly|open_mode::inout:
            return Ucstr("w+bx");
        case open_mode::overwrite|open_mode::inout:
            return Ucstr("w+b");
        case open_mode::append|open_mode::inout:
            return Ucstr("a+b");
        case open_mode::readonly|open_mode::text:
            return Ucstr("r");
        case open_mode::writeonly|open_mode::text:
            return Ucstr("wx");
        case open_mode::overwrite|open_mode::text:
            return Ucstr("w");
        case open_mode::append|open_mode::text:
            return Ucstr("a");
        case open_mode::readonly|open_mode::inout|open_mode::text:
            return Ucstr("r+");
        case open_mode::writeonly|open_mode::inout|open_mode::text:
            return Ucstr("w+x");
        case open_mode::overwrite|open_mode::inout|open_mode::text:
            return Ucstr("w+");
        case open_mode::append|open_mode::inout|open_mode::text:
            return Ucstr("a+");
        default:
            mode = static_cast<open_mode>(std::to_underlying(mode)&0b1111);
            goto switch_begin;
        }
    }
public:
    inline file(std::filesystem::path const& path,open_mode mode)noexcept
        :file_view(fopenU(path.c_str(),c_str(mode))){}
    inline ~file()noexcept
    {
        if(stream)std::fclose(stream),stream=nullptr;
    }
public:
    inline file(file const&)noexcept = delete;
    inline file(file&& that)noexcept
        :file_view(that.stream)
    {
        that.stream = nullptr;
    }
    inline file& operator=(file const&)noexcept = delete;
    inline file& operator=(file&& that)&noexcept
    {
        std::swap(stream,that.stream);
        return *this;
    }
protected:
    inline file(std::FILE* handle)noexcept
        :file_view(handle){}
public:
    inline static file make(std::FILE*&& handle)noexcept
    {
        return file(handle);
    }
};
