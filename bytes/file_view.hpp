#pragma once
#define $included$bytes$file_view 2.3

#include<cstddef>
#include<cstdio>
#include<utility>
#include<optional>
#include<limits>
#include<cstdlib>
#include<algorithm>
#include"ctypes/cstring/basic_cstring_t.hpp"

#include"_fseek64.ipp"
struct file_view
{
public:
    using offset_t = std::ptrdiff_t;
public:
    enum class seek_origin{beginning=SEEK_SET,current=SEEK_CUR,end=SEEK_END};
    using enum seek_origin;
    struct position
    {
        seek_origin origin = seek_origin::beginning;
        offset_t offset = 0;
        constexpr position(std::size_t u)noexcept:offset(static_cast<offset_t>(u)){};
        constexpr position(seek_origin s=seek_origin::beginning,offset_t i=0)noexcept:origin(s),offset(i){};
        constexpr position(offset_t i,seek_origin s)noexcept:origin(s),offset(i){};
        friend inline constexpr position operator+(position p,offset_t offset)noexcept
        {
            return position{p.origin,p.offset+offset};
        }
        friend inline constexpr position operator-(position p,offset_t offset)noexcept
        {
            return position{p.origin,p.offset-offset};
        }
        inline constexpr position operator+=(offset_t offset)noexcept
        {
            this->offset += offset;
            return *this;
        }
        inline constexpr position operator-=(offset_t offset)noexcept
        {
            this->offset -= offset;
            return *this;
        }
    };
    friend inline constexpr position operator+(seek_origin origin,offset_t offset)noexcept
    {
        return position{origin,+offset};
    }
    friend inline constexpr position operator-(seek_origin origin,offset_t offset)noexcept
    {
        return position{origin,-offset};
    }
private:
    std::FILE* stream=nullptr;
    friend struct file;
public:
    inline constexpr operator std::FILE*()const noexcept
    {
        return stream;
    }
public:
    inline constexpr file_view(std::FILE* handle=nullptr)noexcept
        :stream(handle){}
public:
    inline constexpr file_view(file_view const&)noexcept = default;
    inline constexpr file_view& operator=(file_view const&)noexcept = default;
public:
    inline std::size_t read(void* buffer,std::size_t size)noexcept
    {
        return std::fread(buffer,1,size,stream);
    }
    inline std::size_t write(void const* buffer,std::size_t size)noexcept
    {
        return std::fwrite(buffer,1,size,stream);
    }
public:
    inline std::optional<std::size_t> tell()noexcept
    {
        if(auto result=ftell64(stream);result!=-1)
        {
            return result;
        }
        else return std::nullopt;
    }
    inline bool seek(position pos)noexcept
    {
        return fseek64(stream,pos.offset,std::to_underlying(pos.origin)) == 0;
    }
public:
    inline std::optional<std::byte> get()noexcept
    {
        if(auto result=std::fgetc(stream);result!=EOF)
        {
            return static_cast<std::byte>(result);
        }
        else return std::nullopt;
    }
    inline bool put(std::byte b)noexcept
    {
        return std::fputc(static_cast<unsigned char>(b),stream) != EOF;
    }
public:
    inline bool eof()noexcept
    {
        return std::feof(stream);
    }
public:
    inline static std::size_t copy(file_view in,file_view out,std::size_t size=std::numeric_limits<std::size_t>::max(),std::size_t buffer_size=BUFSIZ)noexcept
    {
        void* buffer = std::malloc(buffer_size);
        if(!buffer)return 0;
        std::size_t total_copied = 0;
        while (total_copied < size)
        {
            std::size_t to_copy = std::min(buffer_size,size-total_copied);
            std::size_t read_bytes = in.read(buffer,to_copy);
            std::size_t write_bytes = out.write(buffer,read_bytes);
            total_copied += write_bytes;
            if(write_bytes<to_copy)break;
        }
        std::free(buffer);
        return total_copied;
    }
};