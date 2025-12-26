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


static constexpr std::uint16_t zip64_extended_info_header_id=0x0001;
struct zip64_extended_info_header
{
    std::uint16_t header_id;      // 固定为 0x0001
    std::uint16_t data_size;      // 后续数据大小
    // 以下字段按需出现（当原字段为0xFFFFFFFF时）
    //std::uint64_t uncompressed_size;
    //std::uint64_t compressed_size;
    //std::uint64_t local_header_offset;  // ★ 需要修正的关键字段
    //std::uint32_t disk_start_number;
};
static_assert(sizeof(zip64_extended_info_header) == 4, "zip64_extended_info_header size incorrect");

static constexpr std::uint32_t zip64_ECDR_signature=0x06064b50;
struct alignas(1) ZIP64_End_Of_Central_Directory_Record
{
    std::uint32_t signature;
    std::uint64_t size_of_zip64_EOCD;
    std::uint16_t version_made_by;
    std::uint16_t version_needed;
    std::uint32_t disk_number;
    std::uint32_t start_disk_number;
    std::uint64_t total_entries_this_disk;
    std::uint64_t total_entries;
    std::uint64_t central_directory_size;
    std::uint64_t central_directory_offset;//需要修正的关键字段
    /*zip64 extensible data sector*/
};
static_assert(sizeof(ZIP64_End_Of_Central_Directory_Record)==56,"ZIP64_End_Of_Central_Directory_Record size incorrect");

static constexpr std::uint32_t zip64_ECDL_signature=0x07064b50;
struct alignas(1) ZIP64_End_Of_Central_Directory_Locator
{
    std::uint32_t signature;
    std::uint32_t disk_with_zip64_EOCD;
    std::uint64_t offset_zip64_EOCD;//需要修正的关键字段
    std::uint32_t total_number_of_disks;
};
static_assert(sizeof(ZIP64_End_Of_Central_Directory_Locator)==20,"ZIP64_End_Of_Central_Directory_Locator size incorrect");

#pragma pack(pop)