#include<cstddef>
#include<cstdint>
#include<vector>
#include<expected>
#include<string>
#include<limits>
#include<cerrno>
#include<system_error>
#include"bytes/file_view.hpp"
#include"bytes/file.hpp"
#include"loop/for/FOR_SIZE.hpp"
#include"zip.hpp"

std::expected<void,std::string> merge(file_view ipic,file_view izip,file_view outf)
{
    union//error handling
    {
        bool _b;
        std::size_t _s;
    };
    //直接复制图片文件
    {
        file::copy(ipic,outf); if(!ipic.eof())[[unlikely]]return std::unexpected("@merge:1;"+std::generic_category().message(errno));
    }
    //std::size_t picsize = *outf.tell();
    auto _otell = outf.tell(); if(!_otell)[[unlikely]]return std::unexpected("@merge:2;"+std::generic_category().message(errno));
    std::size_t picsize = *_otell;
    
    //复制并修复ZIP偏移
    {
        //先写入ZIP数据
        file::copy(izip,outf); if(!izip.eof())[[unlikely]]return std::unexpected("@merge:3;"+std::generic_category().message(errno));

        //搜索并读取EOCD
        zip_EOCD eocd;

        //假设没有注释(eocd.comment_length==0)
    _b= izip.seek(file::end - sizeof(zip_EOCD)); if(!_b)[[unlikely]]return std::unexpected("@merge:4;"+std::generic_category().message(errno));
        
        //std::size_t eocd_p = *izip.tell();
        auto _ztell = izip.tell(); if(!_ztell)[[unlikely]]return std::unexpected("@merge:5;"+std::generic_category().message(errno));
        std::size_t eocd_p = *_ztell;
        
    _s= izip.read(&eocd,sizeof(zip_EOCD)); if(_s<sizeof(zip_EOCD))[[unlikely]]return std::unexpected("@merge:6;"+std::generic_category().message(errno));
        
        if(eocd.signature!=zip_EOCD_signature)[[unlikely]]
        {
            //可能有注释，进行搜索
        _b= izip.seek(file::end - sizeof(zip_EOCD) - 1); if(!_b)[[unlikely]]return std::unexpected("@merge:7;"+std::generic_category().message(errno));

            FOR_SIZE(std::numeric_limits<std::uint16_t>::max())//0xFFFF最大注释长度
            {
            _s= izip.read(&eocd.signature,sizeof(uint32_t)); if(_s<sizeof(uint32_t))[[unlikely]]return std::unexpected("@merge:8;"+std::generic_category().message(errno));
                if(eocd.signature==zip_EOCD_signature)[[unlikely]]
                {
                _b= izip.seek(file::current - sizeof(uint32_t)); if(!_b)[[unlikely]]return std::unexpected("@merge:9;"+std::generic_category().message(errno));

                    //eocd_p = *izip.tell();
                    _ztell = izip.tell(); if(!_ztell)[[unlikely]]return std::unexpected("@merge:10;"+std::generic_category().message(errno));
                    eocd_p = *_ztell;

                _s= izip.read(&eocd,sizeof(zip_EOCD)); if(_s<sizeof(zip_EOCD))[[unlikely]]return std::unexpected("@merge:11;"+std::generic_category().message(errno));
                    break;
                }
            _b= izip.seek(file::current - sizeof(uint32_t) - 1); if(!_b)[[unlikely]]return std::unexpected("@merge:12;"+std::generic_category().message(errno));
            }
            if(eocd.signature!=zip_EOCD_signature)[[unlikely]]return std::unexpected("EOCD signature incorrect.");
        }
        
        //修改并写入CDFH
    _b= izip.seek(eocd.central_directory_offset); if(!_b)[[unlikely]]return std::unexpected("@merge:13;"+std::generic_category().message(errno));
    _b= outf.seek(picsize + eocd.central_directory_offset); if(!_b)[[unlikely]]return std::unexpected("@merge:14;"+std::generic_category().message(errno));
        std::vector<std::byte> buffer;
        FOR_SIZE(eocd.total_entries)
        {
            zip_CDFH cdfh;
        _s= izip.read(&cdfh,sizeof(zip_CDFH)); if(_s<sizeof(zip_CDFH))[[unlikely]]return std::unexpected("@merge:15;"+std::generic_category().message(errno));
            if(cdfh.signature!=zip_CDFH_signature)[[unlikely]]return std::unexpected("CDFH signature incorrect at index:["+std::to_string(_iterator_)+"]");
            std::size_t extra_size = cdfh.file_name_length + cdfh.extra_field_length + cdfh.file_comment_length;
            buffer.resize(extra_size);
        _s= izip.read(buffer.data(),extra_size); if(_s<extra_size)[[unlikely]]return std::unexpected("@merge:16;"+std::generic_category().message(errno));
            cdfh.relative_offset_local_header+=picsize;//修复偏移地址
        _s= outf.write(&cdfh,sizeof(zip_CDFH)); if(_s<sizeof(zip_CDFH))[[unlikely]]return std::unexpected("@merge:17;"+std::generic_category().message(errno));
        _s= outf.write(buffer.data(),extra_size); if(_s<extra_size)[[unlikely]]return std::unexpected("@merge:18;"+std::generic_category().message(errno));
        }
 
        //修改并写入EOCD
    _b= outf.seek(picsize+eocd_p); if(!_b)[[unlikely]]return std::unexpected("@merge:19;"+std::generic_category().message(errno));
        eocd.central_directory_offset+=picsize;//修复偏移地址
    _s= outf.write(&eocd,sizeof(zip_EOCD)); if(_s<sizeof(zip_EOCD))[[unlikely]]return std::unexpected("@merge:20;"+std::generic_category().message(errno));
    }
    return {};
}