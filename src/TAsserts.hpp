#ifndef TPP_ASSERTS_HPP
	#define TPP_ASSERTS_HPP

#include <type_traits>
#include <iterator>

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

#define ASSERT_POINTER(Pointer) \
	static_assert( std::is_pointer<Pointer>, \
		"Expected pointer type")

#endif