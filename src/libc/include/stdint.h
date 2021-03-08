#ifndef __LIBC_STDINT_H__
#define __LIBC_STDINT_H__ 1



#define PTRDIFF_MIN -9223372036854775808ll
#define PTRDIFF_MAX 9223372036854775807ll
#define SIZE_MAX 18446744073709551615ull

#define INT8_MIN -128
#define INT16_MIN -32768
#define INT32_MIN -2147483648
#define INT64_MIN -9223372036854775808ll
#define INT_FAST8_MIN -128
#define INT_FAST16_MIN -32768
#define INT_FAST32_MIN -2147483648
#define INT_FAST64_MIN -9223372036854775808ll
#define INT_LEAST8_MIN -128
#define INT_LEAST16_MIN -32768
#define INT_LEAST32_MIN -2147483648
#define INT_LEAST64_MIN -9223372036854775808ll
#define INTMAX_MIN -9223372036854775808ll
#define INTPTR_MIN -9223372036854775808ll
#define INT8_MAX 127
#define INT16_MAX 32767
#define INT32_MAX 2147483647
#define INT64_MAX 9223372036854775807ll
#define INT_FAST8_MAX 127
#define INT_FAST16_MAX 32767
#define INT_FAST32_MAX 2147483647
#define INT_FAST64_MAX 9223372036854775807ll
#define INT_LEAST8_MAX 127
#define INT_LEAST16_MAX 32767
#define INT_LEAST32_MAX 2147483647
#define INT_LEAST64_MAX 9223372036854775807ll
#define INTMAX_MAX 9223372036854775807ll
#define INTPTR_MAX 9223372036854775807ll

#define UINT8_MAX 255
#define UINT16_MAX 65535
#define UINT32_MAX 4294967295u
#define UINT64_MAX 18446744073709551615ull
#define UINT_FAST8_MAX 255
#define UINT_FAST16_MAX 65535
#define UINT_FAST32_MAX 4294967295u
#define UINT_FAST64_MAX 18446744073709551615ull
#define UINT_LEAST8_MAX 255
#define UINT_LEAST16_MAX 65535
#define UINT_LEAST32_MAX 4294967295u
#define UINT_LEAST64_MAX 18446744073709551615ull
#define UINTMAX_MAX 18446744073709551615ull
#define UINTPTR_MAX 18446744073709551615ull

#define INT8_C(v) (v)
#define INT16_C(v) (v)
#define INT32_C(v) (v)
#define INT64_C(v) (v) ## ll
#define INTMAX_C(v) (v) ## ll
#define UINT8_C(v) (v)
#define UINT16_C(v) (v)
#define UINT32_C(v) (v) ## u
#define UINT64_C(v) (v) ## ull
#define UINTMAX_C(v) (v) ## ull



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
