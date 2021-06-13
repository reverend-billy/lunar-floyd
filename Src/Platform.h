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
// Public Constant Definitions
*******************************************************************************/

// Structure packing macros -- note these are compiler specific

// Definitions specific to either Keil or IAR
#if (defined(__CC_ARM) || defined(__ICCARM__))

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
#if defined(__CC_ARM)
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

// --- Compiler Attributes ---
// Specify that parameters should not be nullfunction parameters. This is checked at compile time and a warning is generated if a null value is detexted as a parameter.
#define PLATFORM_NON_NULL __attribute__ ((nonnull))

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

/** Specify that parameters should not be null function
  * \parameters. This is checked at compile time and a warning is
  * generated if a null value is detected as a parameter. GCC
  * uses "nonull" rather than "nonnull"                          
*/
#define PLATFORM_NON_NULL __attribute__ ((nonull))

#endif

// Many functions have no effects except the return value and their return 
// value depends only on the parameters and/or global variables. Such a 
// function can be subject to common subexpression elimination and loop 
// optimization just as an arithmetic operator would be. These functions should 
// be declared with the attribute pure. This function attribute is a GNU 
// compiler extension.  Although related, this function attribute is not 
// equivalent to the __pure keyword. The function attribute equivalent to 
// __pure is __attribute__((const)). 
#define PLATFORM_PURE __attribute__ ((pure))

// Many functions do not examine any values except their arguments, and have no 
// effects except the return value. Basically this is just slightly more strict 
// class than the pure attribute above, since function is not allowed to read 
// global memory. Note that a function that has pointer arguments and examines 
// the data pointed to must not be declared const. Likewise, a function that 
// calls a non-const function usually must not be const. It does not make sense 
// for a const function to return void 
#define PLATFORM_CONST __attribute__ ((const))

// The warn_unused_result attribute causes a warning to be emitted if a caller
// of the function with this attribute does not use its return value. This is 
// useful for functions where not checking the result is either a security 
// problem or always a bug 
#define PLATFORM_WARN_UNUSED_RESULT __attribute__ ((warn_unused_result))

// This attribute, attached to a function, means that code must be emitted for
// the function even if it appears that the function is not referenced. This is 
// useful, for example, when the function is referenced only in inline 
// assembly. 
#define PLATFORM_USED __attribute__ ((used))

// This attribute, attached to a function, means that the function is meant to
// be possibly unused. GCC does not produce a warning for this function. 
#define PLATFORM_UNUSED __attribute__ ((unused))


#ifdef __cplusplus
extern "C"
}
#endif


