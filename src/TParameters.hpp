//##########################################
//##########################################
//###									 ###
//###		AUTHOR: Tilman Gmelin		 ###
//###		ALL RIGHTS RESERVED	 		 ###
//###									 ###
//##########################################
//##########################################	

#ifndef TPP_PARAMETERS_HPP
	#define TPP_PARAMETERS_HPP

#ifndef CACHE_LINE_SIZE
	#define CACHE_LINE_SIZE 64
#endif

#ifndef L1_ASSOCIATIVITY
	#define L1_ASSOCIATIVITY 8
#endif
#define CACHE_LINE_SET_SIZE L1_ASSOCIATIVITY

#ifndef L2_ASSOCIATIVITY
	#define L2_ASSOCIATIVITY 16
#endif
#define CACHE_LINE_SET_SIZE L2_ASSOCIATIVITY

#ifndef L3_ASSOCIATIVITY
	#define L3_ASSOCIATIVITY 32
#endif
#define CACHE_LINE_SET_SIZE L3_ASSOCIATIVITY

#ifndef SIMD_MAX
	#define SIMD_MAX 64
#endif

#ifndef PAGE_SIZE
	#define PAGE_SIZE 4096
#endif


#endif