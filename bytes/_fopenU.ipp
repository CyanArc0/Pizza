#ifdef _WIN32
#define fopenU _wfopen
#define Uchar_t wchar_t
#define Ucstr(str) L##str
#else//POSIX
#define fopenU fopen
#define Uchar_t char
#define Ucstr(str) str
#endif
//FILE* fopenU(const Uchar_t* filename, const Uchar_t* mode);