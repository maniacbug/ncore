#define PROGMEM
#define strlen_P strlen
#define PSTR(x) (x)
typedef const char prog_char;

#ifdef __cplusplus
extern "C" {
#endif

extern void printf_P(const char* format,...);

#ifdef __cplusplus
}
#endif
