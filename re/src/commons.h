#ifndef commons_header
#define commons_header
/* Author: Michele Miccinesi 02022020 	*/
/* Commons */
#include <stdint.h>

typedef int* pint;
typedef int8_t* pint8_t;
typedef int16_t* pint16_t;
typedef int32_t* pint32_t;
typedef int64_t* pint64_t;
typedef uint8_t* puint8_t;
typedef uint16_t* puint16_t;
typedef uint32_t* puint32_t;
typedef uint64_t* puint64_t;

extern const uint64_t mask8;
extern const uint64_t mask7;
extern const uint64_t mask6;
extern const uint64_t uint64_1;
extern const char *symbol_repr[257];

#define forall(vec, i) for( size_t i = (vec)->begin; i < (vec)->end; ++i )
#define forrall(vec, i) for( size_t i = (vec)->end; i-->(vec)->begin; )

#define DBG printf("%s :: %d\n", __FILE__, __LINE__);

#define cum_size_literal(literal) cum_size_literal_helper(sizeof (literal)-1, literal)
#define cum_size_literal_helper(...) __VA_ARGS__

#endif