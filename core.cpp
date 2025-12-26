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
//#include"DEBUG/LOG.hpp"

std::expected<void,std::string> merge(file_view ipic,file_view izip,file_view outf)
{
    union//error handling
    {
        bool _b;
        std::size_t _s;
    };
    //if (std::setvbuf(ipic, nullptr, _IOFBF, 0x400) != 0)[[unlikely]]return std::unexpected("@merge:29;"+std::generic_category().message(errno));
    //if (std::setvbuf(izip, nullptr, _IOFBF, 0x100000) != 0)[[unlikely]]return std::unexpected("@merge:30;"+std::generic_category().message(errno));
    //if (std::setvbuf(outf, nullptr, _IOFBF, 0x100000) != 0)[[unlikely]]return std::unexpected("@merge:31;"+std::generic_category().message(errno));
    //直接复制图片文件
    {
        file::copy(ipic,outf); if(!ipic.eof())[[unlikely]]return std::unexpected("@merge:1;"+std::generic_category().message(errno));
    }
    //std::size_t picsize = *outf.tell();
    auto _otell = outf.tell(); if(!_otell)[[unlikely]]return std::unexpected("@merge:2;"+std::generic_category().message(errno));
    std::size_t picsize = *_otell;
    //LOG "Picture size:",picsize;
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
        std::size_t central_directory_offset = eocd.central_directory_offset;
        if(eocd.central_directory_offset==std::numeric_limits<std::uint32_t>::max())//ZIP64格式
        {
        _b= izip.seek(eocd_p - sizeof(ZIP64_End_Of_Central_Directory_Locator)); if(!_b)[[unlikely]]return std::unexpected("@merge:21;"+std::generic_category().message(errno));
            ZIP64_End_Of_Central_Directory_Locator locator;
        _s= izip.read(&locator,sizeof(ZIP64_End_Of_Central_Directory_Locator)); if(_s<sizeof(ZIP64_End_Of_Central_Directory_Locator))[[unlikely]]return std::unexpected("@merge:22;"+std::generic_category().message(errno));
            if(locator.signature!=zip64_ECDL_signature)[[unlikely]]return std::unexpected("Unsupported ZIP format: ZIP64 locator signature incorrect.");
            
        _b= izip.seek(locator.offset_zip64_EOCD); if(!_b)[[unlikely]]return std::unexpected("@merge:23;"+std::generic_category().message(errno));
            ZIP64_End_Of_Central_Directory_Record zip64_eocd;
        _s= izip.read(&zip64_eocd,sizeof(ZIP64_End_Of_Central_Directory_Record)); if(_s<sizeof(ZIP64_End_Of_Central_Directory_Record))[[unlikely]]return std::unexpected("@merge:24;"+std::generic_category().message(errno));
            if(zip64_eocd.signature!=zip64_ECDR_signature)[[unlikely]]return std::unexpected("Unsupported ZIP format: ZIP64 EOCD signature incorrect.");
            central_directory_offset = zip64_eocd.central_directory_offset;
            
            //写回修改后的ZIP64结构
        _b= outf.seek(picsize + locator.offset_zip64_EOCD); if(!_b)[[unlikely]]return std::unexpected("@merge:25;"+std::generic_category().message(errno));
            zip64_eocd.central_directory_offset += picsize;
        _s= outf.write(&zip64_eocd,sizeof(ZIP64_End_Of_Central_Directory_Record)); if(_s<sizeof(ZIP64_End_Of_Central_Directory_Record))[[unlikely]]return std::unexpected("@merge:26;"+std::generic_category().message(errno));
            
        _b= outf.seek(picsize + eocd_p - sizeof(ZIP64_End_Of_Central_Directory_Locator)); if(!_b)[[unlikely]]return std::unexpected("@merge:27;"+std::generic_category().message(errno));
            locator.offset_zip64_EOCD += picsize;
        _s= outf.write(&locator,sizeof(ZIP64_End_Of_Central_Directory_Locator)); if(_s<sizeof(ZIP64_End_Of_Central_Directory_Locator))[[unlikely]]return std::unexpected("@merge:28;"+std::generic_category().message(errno));
        }
        else
        {
            //修改并写入EOCD
        _b= outf.seek(picsize+eocd_p); if(!_b)[[unlikely]]return std::unexpected("@merge:19;"+std::generic_category().message(errno));
            eocd.central_directory_offset+=picsize;//修复偏移地址
        _s= outf.write(&eocd,sizeof(zip_EOCD)); if(_s<sizeof(zip_EOCD))[[unlikely]]return std::unexpected("@merge:20;"+std::generic_category().message(errno));
        }
        //修改并写入CDFH
    _b= izip.seek(central_directory_offset); if(!_b)[[unlikely]]return std::unexpected("@merge:13;"+std::generic_category().message(errno));
    _b= outf.seek(picsize + central_directory_offset); if(!_b)[[unlikely]]return std::unexpected("@merge:14;"+std::generic_category().message(errno));
        std::vector<std::byte> buffer;
        FOR_SIZE(eocd.total_entries)
        {
            zip_CDFH cdfh;
        _s= izip.read(&cdfh,sizeof(zip_CDFH)); if(_s<sizeof(zip_CDFH))[[unlikely]]return std::unexpected("@merge:15;"+std::generic_category().message(errno));
            if(cdfh.signature!=zip_CDFH_signature)[[unlikely]]return std::unexpected("CDFH signature incorrect at index:["+std::to_string(_iterator_)+"]");
            std::size_t extra_size = cdfh.file_name_length + cdfh.extra_field_length + cdfh.file_comment_length;
            buffer.resize(extra_size);
        _s= izip.read(buffer.data(),extra_size); if(_s<extra_size)[[unlikely]]return std::unexpected("@merge:16;"+std::generic_category().message(errno));
            if(cdfh.relative_offset_local_header==std::numeric_limits<std::uint32_t>::max())//ZIP64格式
            {
                std::byte* p = buffer.data()+cdfh.file_name_length;
                while(p < buffer.data()+cdfh.file_name_length+cdfh.extra_field_length)
                {
                    zip64_extended_info_header* ext_header = reinterpret_cast<zip64_extended_info_header*>(p);
                    if(ext_header->header_id == zip64_extended_info_header_id)
                    {
                        //找到ZIP64扩展字段
                        size_t offset = sizeof(zip64_extended_info_header)
                            + (cdfh.uncompressed_size == std::numeric_limits<std::uint32_t>::max() ? sizeof(std::uint64_t) : 0)
                            + (cdfh.compressed_size == std::numeric_limits<std::uint32_t>::max() ? sizeof(std::uint64_t) : 0);
                        //修正local header offset字段
                        std::uint64_t* local_header_offset_ptr = reinterpret_cast<std::uint64_t*>(p + offset);
                        *local_header_offset_ptr += picsize;
                        break;
                    }
                    p += sizeof(zip64_extended_info_header) + ext_header->data_size;
                }
            }
            else cdfh.relative_offset_local_header+=picsize;//修复偏移地址
        _s= outf.write(&cdfh,sizeof(zip_CDFH)); if(_s<sizeof(zip_CDFH))[[unlikely]]return std::unexpected("@merge:17;"+std::generic_category().message(errno));
        _s= outf.write(buffer.data(),extra_size); if(_s<extra_size)[[unlikely]]return std::unexpected("@merge:18;"+std::generic_category().message(errno));
        }
    }
    return {};
}