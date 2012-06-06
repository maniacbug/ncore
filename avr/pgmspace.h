#ifndef __PGMSPACE_H__
#define __PGMSPACE_H__

#include <inttypes.h>
#define PROGMEM
#define strlen_P strlen
#define memcpy_P memcpy
#define PSTR(x) (x)
typedef const char prog_char;
typedef uint8_t prog_uint8_t;

#ifdef __cplusplus
extern "C" {
#endif

inline uint8_t pgm_read_byte(const uint8_t* p) { return *p; }
inline uint32_t pgm_read_dword(const uint32_t* p) { return *p; }
extern void printf_P(const char* format,...);

#ifdef __cplusplus
}
#endif
#endif // __PGMSPACE_H__
