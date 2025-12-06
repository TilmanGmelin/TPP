//##########################################
//##########################################
//###									 ###
//###		AUTHOR: Tilman Gmelin		 ###
//###		ALL RIGHTS RESERVED	 		 ###
//###									 ###
//##########################################
//##########################################	



//##########################################
//#				INLINE 				 	   #
//##########################################	
/*
Marks a Function elegable for inlining. The compiler may decided
to inline such a function. 
*/
//##########################################
//#				FORCE_INLINE 		 
//##########################################
/*
Marks a function as forced to be inline. The compiler will 100% inline this 
function wherever it will be called. Used for any getter/setter or small functions
to save the calling overhead
*/
//##########################################
//#				HOT 				 
//##########################################
/*
Marks a function as very frequently used. If set wisely, the compiler will
be able to optimimze better
*/
//##########################################
//#				COLD 				 
//##########################################
/*
Same as Hot, only that the function is marked as rarely used. 
*/
//##########################################
//#				NORETURN 			 
//##########################################
/*
Marks a function to not be returning ever. This is the case for example in error
paths that will call exit(). Should be rarely used and is not realy an important one, as 
It lets the compiler remove dead-path assumptions, tighten diagnostics, and optimize unreachable code.
*/
//##########################################
//#				PURE 				 
//##########################################
/*
A PURE function is a function which return value does rely on extrenal read-only data and input parameters.
and has has NO side effekts.
Repeated calls with identicall input can be optimized away this way, as long as globals have not been
modified in between the calls.
*/
//##########################################
//#				CONST 				 
//##########################################
/*
A CONST function is function that does NOT rely on any external data
*/
//##########################################
//#				MALLOC 				 
//##########################################
/*
Marks a function to be allocating new memory, that needs to be handled
*/
//##########################################
//#				NOINLINE 			 
//##########################################
/*
This Function will not be inligned
Use for large shit, maybe for shit that should stay within its section and so on. 
*/
//##########################################
//#				ALIGN(n) 			 
//##########################################
/*
Aligns the datatype to start in specified alignment
Used for SIMDs and BUS-Optimizations 
*/
//##########################################
//#				PACKED 				 
//##########################################
/*
Disables Padding for given struct.
No bytes between members, so beware of missaligned access!
Because mvc is stupid, we have packed start and end for now.
If you want to solve this for me, have some fun with the preprocessor, because a struct 
is a struct is a struct
and we basicially want the gcc way of marking the struct or class as packed xd
*/
//##########################################
//#				PACKED_START 		 
//##########################################
/*
(see PACKED first)
Write this as modifier to the struct/class
*/
//##########################################
//#				PACKED_END 			 
//##########################################
/*
(see Packed first)
write this behind the struct...
*/
//##########################################
//#				SECTION(name) 		 
//##########################################
/*
Places data in Specific LINKER section, effectefly grouping
it in outcomming memory layout. Cache set friendlyness can be achieved here.
That also includes NUMA optimization, hot/cold splitting, startup and pipeline ordering
... and other linker tricks that might concern max
*/
//##########################################
//#				FLATTEN 			 
//##########################################
/*
Forces the compiler to inline all calls inside a function, recursively, as much as possible.
Can increase code size significantly.
Used for small, preformance critical wrappers that call multiple tiny functions. 
Avoid bigger call trees and branching behavior traps.
Due to the exsistence of FORCE_INLINE, it is adviced not to use Flatten.
*/
//##########################################
//#				NO_SANITIZE(str) 	 
//##########################################
/*
Used to disable
	- thread or memory checks
	- undefined behavior checks
	- integer overflow, alignment, or float-divide checks

"address" — disable address‑sanitizer instrumentation.
"thread" — disable thread‑sanitizer instrumentation. 
"memory" — disable memory‑sanitizer (uninitialized memory use check). 
"undefined" — disable undefined‑behavior checks from undefined‑behavior sanitizer (UBSan). 
"coverage" — disable sanitizer‑coverage instrumentation (if coverage instrumentation is enabled).

... and more fine‑grained UBSan‑checks: e.g. "signed-integer-overflow", "unsigned-integer-overflow", "pointer-overflow", "null", "vptr" (for C++ RTTI / vptr checks), 
and others depending on which -fsanitize= flags you use. 

*/
//##########################################
//#				EXPORT 				 
//##########################################
/*
Force export symbol outside the shared library or obj file. 
Used for dll`s, normaly in comabination with IMPORT
*/
//##########################################
//#				IMPORT 				 
//##########################################
/*
Counterpart to EXPORT, this imports a symbol from another shared library.
It tells the compiler not to allocate storage for it, as it is elsewhere. 
*/
//##########################################
//#				WEAK 				 
//##########################################
/*
If no strong definition of this function is defined, use this one. 
Used for optional overrides. Debug shit can be inserted good this way.
*/
//##########################################
//#				MAY_ALIAS 			 
//##########################################
/*
This data may be accessed through a reference from another universe,
so some assumptions can NOT be made, which hurts optimization.
*/

#ifndef TPP_INTRINSICS_HPP
	#define TPP_INTRINSICS_HPP

#define UNDEFINED
	#if defined(_MSC_VER) 
		#define INLINE 				__inline
		#define FORCE_INLINE 		__forceinline 
		#define NOINLINE 			__declspec(noinline) 
		#define HOT 				UNDEFINED 
		#define COLD 				UNDEFINED 
		#define NORETURN 			__declspec(noreturn) 
		#define PURE 				UNDEFINED 
		#define CONST 				UNDEFINED 
		#define MALLOC 				UNDEFINED 
		#define PACKED_START 		__pragma(pack(push, 1)) 
		#define PACKED_END 			__pragma(pack(pop)) 
		#define FLATTEN 			UNDEFINED 
		#define EXPORT 				__declspec(dllexport) 
		#define IMPORT 				__declspec(dllimport) 
		#define WEAK 				UNDEFINED 
		#define MAY_ALIAS 			UNDEFINED 
		#define ALIGN(n) 			__declspec(align(n)) 
		#define SECTION(name) 		__declspec(allocate(name)) 
		#define NO_SANITIZE(str) 	UNDEFINED 
	#elif defined(__GNUC__) || defined(__clang__) 
		#define INLINE 				inline 
		#define FORCE_INLINE 		inline __attribute__((always_inline)) 
		#define HOT 				__attribute__((hot)) 
		#define COLD 				__attribute__((cold)) 
		#define NORETURN 			__attribute__((noreturn)) 
		#define PURE 				__attribute__((pure)) 
		#define CONST 				__attribute__((const)) 
		#define MALLOC 				__attribute__((malloc)) 
		#define NOINLINE 			__attribute__((noinline)) 
		#define PACKED_START 		__attribute__((packed)) 
		#define PACKED_END 			UNDEFINED 
		#define FLATTEN 			__attribute__((flatten)) 
		#define EXPORT 				__attribute__((visibility("default"))) 
		#define IMPORT 				UNDEFINED 
		#define WEAK 				__attribute__((weak)) 
		#define MAY_ALIAS 			__attribute__((may_alias)) 
		#define ALIGN(n) 			__attribute__((aligned(n))) 
		#define SECTION(name) 		__attribute__((section(name))) 
		#define NO_SANITIZE(str) 	__attribute__((no_sanitize(str))) 
	#else //compiler unkown. 
		#define INLINE 				inline 
		#define FORCE_INLINE 		inline 
		#define HOT 				UNDEFINED 
		#define COLD 				UNDEFINED 
		#define NORETURN 			UNDEFINED 
		#define PURE 				UNDEFINED 
		#define CONST 				UNDEFINED 
		#define MALLOC 				UNDEFINED 
		#define NOINLINE 			UNDEFINED 
		#define ALIGN(n) 			UNDEFINED 
		#define PACKED 				UNDEFINED 
		#define PACKED_START 		UNDEFINED 
		#define PACKED_END 			UNDEFINED 
		#define SECTION(name) 		UNDEFINED 
		#define FLATTEN 			UNDEFINED 
		#define NO_SANITIZE(str) 	UNDEFINED 
		#define EXPORT 				UNDEFINED 
		#define IMPORT 				UNDEFINED 
		#define WEAK 				UNDEFINED 
		#define MAY_ALIAS 			UNDEFINED 
	#endif
#endif