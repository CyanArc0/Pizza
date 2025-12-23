#if defined(_WIN32) and not defined(__MINGW32__)
#define fseek64 _fseeki64
#define ftell64 _ftelli64
#define off64_t __int64
#else//POSIX
#define fseek64 fseeko64
#define ftell64 ftello64
#endif
//off64_t ftell64(FILE *stream);
//int fseek64(FILE* stream, off64_t offset, int whence);