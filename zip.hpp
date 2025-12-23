#pragma once
#include<cstdint>
#pragma pack(push, 1)
static constexpr std::uint32_t zip_CDFH_signature=0x02014b50;
struct alignas(1) zip_CDFH
{
    std::uint32_t signature;
    std::uint16_t version_made_by;
    std::uint16_t version_needed;
    std::uint16_t general_purpose_flag;
    std::uint16_t compression_method;
    std::uint16_t last_mod_file_time;
    std::uint16_t last_mod_file_date;
    std::uint32_t crc32;
    std::uint32_t compressed_size;
    std::uint32_t uncompressed_size;
    std::uint16_t file_name_length;
    std::uint16_t extra_field_length;
    std::uint16_t file_comment_length;
    std::uint16_t disk_number_start;
    std::uint16_t internal_file_attributes;
    std::uint32_t external_file_attributes;
    std::uint32_t relative_offset_local_header;
    /*file_name*/
    /*extra_field*/
    /*file_comment*/
};
static_assert(sizeof(zip_CDFH)==46,"zip_CDFH size incorrect");
static constexpr std::uint32_t zip_EOCD_signature=0x06054b50;
struct alignas(1) zip_EOCD 
{
    std::uint32_t signature;
    std::uint16_t disk_number;
    std::uint16_t start_disk_number;
    std::uint16_t total_entries_this_disk;
    std::uint16_t total_entries;
    std::uint32_t central_directory_size;
    std::uint32_t central_directory_offset;
    std::uint16_t comment_length;
    /*comment*/
};
static_assert(sizeof(zip_EOCD)==22,"zip_EOCD size incorrect");
#pragma pack(pop)