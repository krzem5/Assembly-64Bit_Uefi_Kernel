#ifndef __LIBC_STDINT_H__
#define __LIBC_STDINT_H__ 1



#define PTRDIFF_MIN -9223372036854775808ll
#define PTRDIFF_MAX 9223372036854775807ll
#define SIZE_MAX 18446744073709551615ull



typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;
typedef signed char int_fast8_t;
typedef signed short int int_fast16_t;
typedef signed int int_fast32_t;
typedef signed long long int int_fast64_t;
typedef signed char int_least8_t;
typedef signed short int int_least16_t;
typedef signed int int_least32_t;
typedef signed long long int int_least64_t;
typedef int64_t intmax_t;
typedef int64_t intptr_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned short int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef unsigned long long int uint_fast64_t;
typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long long int uint_least64_t;
typedef uint64_t uintmax_t;
typedef uint64_t uintptr_t;



#endif
