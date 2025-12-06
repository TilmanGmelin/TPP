//##########################################
//##########################################
//###									 ###
//###		AUTHOR: Tilman Gmelin		 ###
//###		ALL RIGHTS RESERVED	 		 ###
//###									 ###
//##########################################
//##########################################	

#ifndef TPP_CONTAINER_HPP
	#define TPP_CAINTER_HPP

#include "TIntrinsics.hpp"
#include "TBasicTypes.hpp"

template<class Container>
FORCE_INLINE void pushU16(Container& _c, const uint16 _x)
{
	ASSERT_BYTE_CONTAINER(Container);

	const std::size_t i = _c.size();
	_c.resize(i + 2);

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	_c.data()[i]     = static_cast<typename Container::value_type>(_x);
	_c.data()[i + 1] = static_cast<typename Container::value_type>(_x >> 8);
#else
	_c.data()[i]     = static_cast<typename Container::value_type>(_x >> 8);
	_c.data()[i + 1] = static_cast<typename Container::value_type>(_x);
#endif
}

	template<class Ptr>
	FORCE_INLINE uint16 readU16(const Ptr _p)
	{
	    static_assert(
	        std::is_pointer_v<Ptr>,
	        "readU16 expects pointer type"
	    );

	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	    return static_cast<uint16>(
	        static_cast<uint8>(_p[0]) |
	        (static_cast<uint16>(static_cast<uint8>(_p[1])) << 8)
	    );
	#else
	    return static_cast<uint16>(
	        (static_cast<uint16>(static_cast<std::uint8_t>(_p[0])) << 8) |
	        static_cast<uint8>(_p[1])
	    );
	#endif
	}


#endif