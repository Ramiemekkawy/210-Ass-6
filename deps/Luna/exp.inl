#pragma once

#define EXPERIMENTAL_MARS_
#if defined(EXPERIMENTAL_MARS_)



#define FwdFn(F) [](auto&&... args##F)\
 { return F(std::forward<decltype(args##F)>(args##F)...); }

#define DeclFnAlias(F, F2) \
 template<class... T> auto F2(T&&... args##F2)\
 { return F(std::forward<decltype(args##F2)>(args##F2)...); } \

#define DeclFnA(F, F2) static auto const F2 = FwdFn(F)

#define cast(T) static_cast<T>

#define $import using namespace
#define $use using

#define cast(typ) static_cast<typ> 

#define ConstStruct(s, s2) using s2 = s const;

#define concat_(x, y) x ## y

#define concat0_(x, y) concat_(x, y)

#define Struct_Impl_(name, name2, c) struct concat0_(name2, c); using name = concat0_(name2, c) const; struct concat0_(name2, c)

#define Struct(name) Struct_Impl_(name, name##_ConstStruct_, __COUNTER__)

/**/
template<typename T>
using Unqualified =
typename std::remove_cv<typename std::remove_reference<T>::type>::type;
/**/

template<typename T>
using A = T;

inline void foo( A<int[234]> arr )
{
	auto x = arr;
}

Struct( Sc0 )
{

};

Struct( Sc20 )
{

};

Sc0 sc0;

template<class T>
struct imm_impl {
	using type = const T;
	using otype = T;
};

template<class T>
using imm = typename imm_impl<T>::type;

template<class T>
struct mut_impl {
	using type = std::remove_const_t<T>;
};

template<class T>
struct mut_impl<imm_impl<T>> {
	using type = typename imm_impl<T>::otype;
};

template<class T>
using mut = typename mut_impl<T>::type;

template<class C>
using cell = mut<C>;

template<class T>
using ref = mut<T>&;

//static mut<ref<int>> s;

#endif