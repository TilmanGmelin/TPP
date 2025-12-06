//##########################################
//##########################################
//###									 ###
//###		AUTHOR: Tilman Gmelin		 ###
//###		ALL RIGHTS RESERVED	 		 ###
//###									 ###
//##########################################
//##########################################	

#ifndef TILPP
	#define TILPP

#include <utility>
#include <algorithm>
#include <type_traits>
#include <cstdint>
#include <vector>
#include <iterator> 
#include <list>


template 
<
	typename Container,
	typename T
>
struct IS_FORWARD_CONTAINER_OF
{
	static constexpr bool value =
	    std::is_same<T, typename Container::value_type>::value &&
	    std::is_convertible<
	        typename std::iterator_traits<typename Container::iterator>::iterator_category,
	        std::forward_iterator_tag
	    >::value;
	};
#define ASSERT_CONTAINER_OF(Container, T) static_assert(IS_FORWARD_CONTAINER_OF<Container, T>::value, "Containercontenttype and valuetype must be the same and a forward iterator is neccesarry!!!1!")

template <typename T, typename = void>
struct IS_BYTE_CONTAINER : std::false_type {};

template <typename T>
struct IS_BYTE_CONTAINER<T,
	std::void_t<
		decltype(std::declval<T&>().size()),
		decltype(std::declval<T&>().resize(std::size_t{})),
		decltype(std::declval<T&>().data())
	>	
> : std::true_type {};

#define ASSERT_BYTE_CONTAINER(Container) \
    static_assert(IS_BYTE_CONTAINER<Container>::value, \
                  "Container must have size(), resize(), and data()")

namespace tpp
{
	template 
	<
		typename Container, 
		typename T
	>
	FORCE_INLINE typename Container::iterator binaryInsertUnique(Container& _c, const T& _value)
	{
		ASSERT_CONTAINER_OF(Container, T);
	
		auto it = std::lower_bound(_c.begin(), _c.end(), _value); // find element or position (sorted)
		return ((it == _c.end() || *it != _value) ? _c.insert(it, _value) : it); // if element not found, insert new		
	}

	template 
	<
		typename Container, 
		typename T
	>
	FORCE_INLINE typename Container::iterator insertAt(Container& _c, uint64 _index, const T& _value)
	{
		ASSERT_CONTAINER_OF(Container, T);
		
		return (_c.insert(_c.begin() + _index, _value));
	}

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

	INLINE uint64_t cstrcpyCount(const char* _src, std::vector<char>& _dst)
	{
	    uint64_t out = 0;

	    while (*_src)
	    {
	        _dst.push_back(*_src++);
	        out++;
	    }
	    return out;
	}

	

	template<class Ptr>
	FORCE_INLINE std::uint16_t readU16(const Ptr _p)
	{
	    static_assert(
	        std::is_pointer_v<Ptr>,
	        "readU16 expects pointer type"
	    );

	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	    return static_cast<std::uint16_t>(
	        static_cast<std::uint8_t>(_p[0]) |
	        (static_cast<std::uint16_t>(static_cast<std::uint8_t>(_p[1])) << 8)
	    );
	#else
	    return static_cast<std::uint16_t>(
	        (static_cast<std::uint16_t>(static_cast<std::uint8_t>(_p[0])) << 8) |
	        static_cast<std::uint8_t>(_p[1])
	    );
	#endif
	}

	FORCE_INLINE uint32 simpleHash(const char* _str, const char& _delim)
	{
		uint32 out = 0;
		while (*_str != _delim)
			out += 31 * out + static_cast<unsigned char>(*_str++);
		return (out);
	}

	class StringPermutationFactory
	{
	private:
		static std::vector<char> preset_;  //raw form that is filled
	public:
		StringPermutationFactory() = delete;
		StringPermutationFactory(const char* _format, std::list<std::list<std::string>>& _sections)
		{
			preset_.clear();
			_format += tpp::cstrcpyCount(_format, preset_);
			preset_.push_back(0);
			while (*_format || *(_format + 1))
			{
				// insert section data into preset
				uint16 acc_strlens = _sections.front().size() * sizeof(uint16);
				for (auto s : _sections.front())
				{
					tpp::pushU16(preset_, static_cast<uint16>(acc_strlens)); 
					acc_strlens += s.length() + sizeof(uint16) + 1; //distance to end and dont forget null term
				}
				
				//insert strings in order with distance to end of section postfixed
				acc_strlens -= _sections.front().size() * sizeof(uint16);

				preset_.reserve(preset_.size() + acc_strlens + 64);
				const char* next_section = &preset_.back() + acc_strlens - 1;
				for (auto s : _sections.front())
				{
					tpp::cstrcpyCount(s.c_str(), preset_);
					preset_.push_back(0);
					tpp::pushU16(preset_, static_cast<uint16>(next_section - (&preset_.back() - 1)));
				}
				_sections.pop_front();

				// insert static string from format into preset
				_format += tpp::cstrcpyCount(++_format, preset_);
				preset_.push_back(0);
			}
			preset_.push_back(0);
			preset_.push_back(0);
			preset_.shrink_to_fit();
			auto f = preset_.data();
			int x = 0;
			for (int i = 0; i < 10; i++)
				x = f[i] + x;

		}

		void generate(const uint8* _permutation, const char* _dynamics, char* _out)
		{
			char* preset = preset_.data();
		
			//write static section
			uint32 buf = tpp::cstrcpyCount(preset, _out);
			_out += buf;								    //advance to end of string in output								
			preset += buf + 1; 								//advance behind string in input

			while (*preset || *(preset + 1))
			{
				//write dynamic section
				buf = tpp::cstrcpyCount(_dynamics, _out);
				_out += buf;
				_dynamics += buf + 1;

				//write option seciton
				//permutation is an index into the string offset array
				uint16 buf2 = tpp::readU16(preset + *_permutation * sizeof(uint16));

				//copy string at offset. buf now contains the strlen of the string.
				buf = tpp::cstrcpyCount(preset+ buf2, _out);

				//advance to end of string in output						
				_out += buf;															
				preset +=  buf2 + buf + 1 + 											//advance behind string in input
					tpp::readU16(preset + buf2 + buf + 1); 								//...and further advance by the amount written there
				_permutation++;											//advance to next permutation
		
				//write static section
				buf = tpp::cstrcpyCount(preset, _out); 		//write n
				_out += buf;								//advance to end of string in output								
				preset += buf + 1; 							//advance behind string in input
			}
			*_out = 0;
		}
	};

	//just to check if a string is element of a spezific group and returns an id. max 16 strings
	template<class T>
	FORCE_INLINE uint32_t bSearch(const T* _data, uint32_t _count, const T& _key) 
	{
	    uint32 left = 0;
	    uint32 result = UINT32_MAX;
		
	    _count--;
	    while (left <= _count) {
	        uint32 mid = left + (_count - left) / 2;
	        T v = _data[mid];
	        result = (v == _count) ? mid : result;
	        left  = (_count < v) ? left : mid + 1;
	        _count = (_count < v) ? mid - 1 : _count;
	    }
	    return result;
	}

	template
	<
		size_t CACHE_LINES = 1
	>
	class MiniStringMap
	{
	private:
		uint32 count_;
		std::array<uint32, CACHE_LINES * CACHE_LINE_SIZE / sizeof(uint32) - 1> hashes_;
	public:
		MiniStringMap(){};

		void load(std::list<std::string> _strs)
		{
			count_ = _strs.size();
		#ifdef TPP_THROW_0
			if (count_ > CACHE_LINES * sizeof(uint32) - 1)
				throw std::overflow_error("MiniStringMap capacity exceeded");
		#endif
			std::list<uint32> hashes;
			int i = 0;
			for (auto s : _strs)
				hashes.push_front(tpp::simpleHash(s.c_str()));
			hashes.sort();
			hashes.unique();
		#ifdef TPP_THROW_1
			if (hashes.size() != _strs.size())
				throw std::length_error("MiniStringMap contains duplicate hashes or too many elements");
		#endif
			i = 0;
			for (uint32 x : hashes)
				hashes_[i++] = x;
		}

		MiniStringMap(std::list<std::string> _strs) : count_(_strs.size())
		{
		#ifdef TPP_THROW_0
			if (count_ > CACHE_LINES * sizeof(uint32) - 1)
				throw std::overflow_error("MiniStringMap capacity exceeded");
		#endif
			std::list<uint32> hashes;
			int i = 0;
			for (auto s : _strs)
				hashes.push_front(tpp::simpleHash(s.c_str()));
			hashes.sort();
			hashes.unique();
		#ifdef TPP_THROW_1
			if (hashes.size() != _strs.size())
				throw std::length_error("MiniStringMap contains duplicate hashes or too many elements");
		#endif
			i = 0;
			for (uint32 x : hashes)
				hashes_[i++] = x;
		}

		FORCE_INLINE uint32 getId(const char* _str, const char& _delim) const
		{
			uint32 hash = tpp::simpleHash(_str, _delim);
			uint32 left = 0;
			uint32 right = count_ - 1;
			uint32 result = UINT32_MAX;
		
			while (left <= right)
			{
				uint32 mid = left + (right - left) / 2;
				uint32 h = hashes_[mid];
		
				result = (hash == h) ? mid : result;
				left = (hash < h) ? left : mid + 1;
				right = (hash < h) ? mid - 1 : right;
			}
		
			return result;
		}
	};
}

#endif