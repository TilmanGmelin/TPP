//##########################################
//##########################################
//###									 ###
//###		AUTHOR: Tilman Gmelin		 ###
//###		ALL RIGHTS RESERVED	 		 ###
//###									 ###
//##########################################
//##########################################	

#ifndef TPP_BASIC_TYPES_HPP
	#define TPP_BASIC_TYPES_HPP

#include <array>
#include "TIntrinsics.hpp"
#include "TParameters.hpp" 

typedef std::array<uint8_t, CACHE_LINE_SIZE> 	CacheLine;
typedef std::array<CacheLine, L1_ASSOCIATIVITY> CacheLineSet;

typedef uint64_t 	uint64;
typedef uint32_t 	uint32;
typedef uint16_t 	uint16;
typedef uint8_t  	uint8;

typedef int64_t 	int64;
typedef int32_t 	int32;
typedef int16_t 	int16;
typedef int8_t  	int8;

typedef uint8 		byte;

typedef uint32 		uint;

typedef uintptr_t 	uintptr;

#endif