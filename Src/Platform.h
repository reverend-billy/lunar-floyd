/** Platform Utilities
  * Example:
  *    <code lang="c++">
  *    PLATFORM_PACK_START(1)
  *    typedef struct
  *    {
  *       uint8_t u8Value;
  *       // Padding would automatically be added here if packing were not specified
  *       uint32_t u32Value;
  *    } PLATFORM_ATTRIBUTE_PACKED Structure_t;
  *    PLATFORM_PACK_END()
  *    </code>
  *                                                                                 
*/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************
// Includes
*******************************************************************************/
// Module Includes
// Platform Includes
// Other Includes


/*******************************************************************************
// Public Constants
*******************************************************************************/

// Structure packing macros -- note these are compiler specific

// Definitions specific to either Keil or IAR
#if !defined(__GNUC__)

// Define min comparison without the benefit of the GCC typeof extension
#define PLATFORM_MIN(a, b) ((a) < (b) ? (a) : (b))
// Define max comparison without the benefit of the GCC typeof extension
#define PLATFORM_MAX(a, b) ((a) > (b) ? (a) : (b))

// Turn value into a string with quotes
#define PLATFORM_MAKE_STRING(X) #X

// Specify start of packed structure
// This is primarily used for bit-packing structures used for messaging
#define PLATFORM_PACK_START(X) _Pragma(PLATFORM_MAKE_STRING(pack(X)))

// Specify end of packed structure
// This is primarily used for bit-packing structures used for messaging
#define PLATFORM_PACK_END()    _Pragma("pack()")

// Empty for non-GCC platforms
#define PLATFORM_ATTRIBUTE_PACKED

// --- Keil compiler alignment ---
#if defined(__CC_ARM )
// Specify alignment for a variable
#define PLATFORM_ALIGN(X) __align(X)
#endif

// --- IAR compiler alignment ---
#if defined(__ICCARM__)
// Specify alignment for a structure
// This is primarily used for bit-packing structures used for messaging
#define PLATFORM_ALIGN(X) _Pragma(PLATFORM_MAKE_STRING(data_alignment=X))
#endif

// Specify alignment for a variable
#define PLATFORM_ATTRIBUTE_ALIGN(X)



// -- GCC compiler ---
#else // __GNUC__

// Min that only evaluates a & b once
// This  makes use of GCC typeof extension
#define PLATFORM_MIN(a, b) __extension__({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a < _b ? _a : _b;})

// Max that only evaluates a & b once
// This  makes use of GCC typeof extension
#define PLATFORM_MAX(a, b) __extension__({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b;})

/** Attribute to define packed structure This is primarily used
  * for bit-packing structures used for messaging              
*/
#define PLATFORM_ATTRIBUTE_PACKED __attribute__ ((packed))

// Specify start of packed structure
// Use this macro before the struct definition.
// X denotes the maximum alignment of struct members. X is not supported with GCC. 
// GCC always use 1 byte maximum alignment.
#define PLATFORM_PACK_START(x)

// Specify end of packed structure
// Use this macro after the struct definition.
// With GCC, add PLATFORM_ATTRIBUTE_PACKED after the closing } of the struct definition.
#define PLATFORM_PACK_END()

// Specify alignment for a variable
#define PLATFORM_ATTRIBUTE_ALIGN(X) __attribute__ ((aligned(X)))

/** Specify alignment for a variable Use this macro before the
  * variable definition. X denotes the storage alignment value in
  * bytes. To be GCC compatible use PLATFORM_ATTRIBUTE_ALIGN(X)
  * before the ; on normal variables. Use
  * PLATFORM_ATTRIBUTE_ALIGN(X) before the opening { on struct
  * variables.                                                   
*/
#define PLATFORM_ALIGN(X)

#endif

#ifdef __cplusplus
extern "C"
}
#endif


