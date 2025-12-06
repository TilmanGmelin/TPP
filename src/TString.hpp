#ifndef TPP_STRINGS_HPP
	#define TPP_STRINGS_HPP

#include <vector>
#include "TIntrinsics.hpp"
#include "TBasicTypes.hpp"
	

INLINE uint64 cstrcpyCount(const char* _src, char* _dst)
{
	uint64 out = 0;
	while (*_src)
	{
		*(_dst++) = *(_src++);
		out++;
	}
	return (out);
}

INLINE uint64 cstrcpyCount(const char* _src, std::vector<char>& _dst)
{
	uint64 out = 0;

	while (*_src)
	{
		_dst.push_back(*_src++);
		out++;
	}
	return out;
}

#endif