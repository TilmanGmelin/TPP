#ifndef TPP_MATH_HPP
	#define TPP_MATH_HPP
#include "TIntrinsics.hpp"
#include "TBasicTypes.hpp"
FORCE_INLINE uint32 simpleHash(const char* _str, const char& _delim)
{
	uint32 out = 0;
	while (*_str != _delim)
		out += 31 * out + static_cast<unsigned char>(*_str++);
	return (out);
}

#define HASH(_str, _delim) simpleHash(_str, _delim)

#endif
