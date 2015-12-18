#pragma once
#ifndef Utils_HH_
#define Utils_HH_

#include <string>
#include <sstream>
#include "format.h"
#include <utility>
#include <functional>
#include <algorithm>

#if 1
#include "exp.inl"

//#define type class
//#define typeof(x) decltype(x)
//#define discard(x) []{ x(); }()
//#define voidify(x) (void)(x)

//#define let auto const

//#define letmut auto
/**/
/**
struct S_i {

};

using S = const S_i;

typedef const struct {

} S3;

struct SS;
using S22 = SS;
struct SS {};
/**/
Struct(Const1) {

};
/**
inline void boo()
{
	SS ss;
	ss = SS();
	Const1 cs;
	//cs = Const1();
	mut<Const1> cs2;
	cs2 = cs;
	static mut<S3> s;
	S3 v;
	//v = S3();
	s = S3();
}
/**/
namespace mars
{

	inline
		std::string repeat( const std::string& c, int n )
	{
		std::string ss;

		for( int i = 0; i < n; ++i )
			ss += c;

		return ss;
	}

	inline
		std::string repeat( char c, int n )
	{
		std::string ss;

		for( int i = 0; i < n; ++i )
			ss.push_back( c );

		return ss;
	}

	inline 
		void log( const std::string& s )
	{
		fmt::print( "\nLog [{}]: {}",
					" ",
					s );
	}

	template<class... Args>
	inline bool log( Args&&... args )
	{
		fmt::print("Log[{}]: ",
					args...)
		return true;
	}
}

#endif

#if 0
namespace
{
	struct Ok_reserve {};
	struct Err_reserve {};
}

template<class> struct Expect;

template<class T>
Expect<T> Ok( T&& );

template<class T>
Expect<T> Err( int );

template<class T>
struct Expect
{
public:
#if 0
	static Expect<T> Err( int r )
	{
		return Expect<T>{ Err_reserve(), r };
	}

	static Expect<T> Ok( const T& inref )
	{
		return Expect<T>{ Ok_reserve(), inref };
	}
#endif

	operator bool() { return !(err == 0); }

	T unwrap() const
	{
		return x;
	}

	int get_err() const
	{
		if( is_ok() )
			return err;
		else
			throw std::exception( "Boo! No errors here." );
	}

	T value_or( const T& alt )
	{
		if( is_ok() ) {
			return x;
		}
		else {
			return alt;
		}
	}

	template<class Func, class... Args>
	auto and_then( Func fun, Args... args )
	{
		if( is_ok() )
			return fun( x, args... );
		else
			throw std::exception( "Blow up! Functor called on Err : Expect." );
	}

	template<class Func, class... Args>
	auto and_then( Func* fun, Args... args )
	{
		if( is_ok() )
			return (*fun)( x, args... );
		else
			throw std::exception( "Blow up! Functor called on Err : Expect." );
	}


	template<class Func, class... Args>
	auto if_not( Func fun, Args... args )
	{
		if( !is_ok() )
			return fun(err, args...);
		else
			throw std::exception( "Blow up! Functor called on Err : Expect." );
	}

	template<class Func, class... Args>
	auto if_not( Func* fun, Args... args )
	{
		if( !is_ok() )
			return (*fun)( err, args... );
		else
			throw std::exception( "Blow up! Functor called on Err : Expect." );
	}

private:
	int err;
	T x;

	bool is_ok() const { return err != 0; }
	Expect( Ok_reserve __, const T& inref ) : x{ inref }, err{ 0 } {}
	Expect( Err_reserve __, int r ) : err{ r } {}
	friend Expect Ok<T>( T&& );
	friend Expect Err<T>( int );
};
namespace
{
	template<class T>
	struct friend_help  {

	};
}
template<class T>
Expect<T> Ok( T&& val )
{
	return Expect<T>( Ok_reserve(), val );
}

template<class T>
Expect<T> Err( int val )
{
	return Expect<T>{ Err_reserve(), val };
}


template<class T>
struct Option {
	bool has() const {
		return is;
	}
	
	T some() const
	{
		if (!has()) {
			/*handle error*/
		}
		return t;
	}

	bool some( const T& in ) {
		t = in;
		is = true;
		return true;
	}

	bool none()
	{
		is = false;
		return true;
	}

	static Option Some( const T& t )
	{
		return Option( t, true );
	}
	static Option Some( T&& t )
	{
		return Option( t, true );
	}
	static Option None()
	{
		return Option( T(), false );
	}

	explicit operator bool() const
	{
		return has();
	}
private:
	T t;
	bool is;
	Option( const T& in, bool f = false ) : t{ in }, is{ f } {}
};

template<class T>
Option<T> Some( T&& t )
{
	return Option<T>::Some( t );
}

template<class T>
Option<T> Some( const T& t )
{
	return Option<T>::Some( t );
}

template<class T>
Option<T> None()
{
	return Option<T>::None();
}

template<class T>
bool some( const Option<T>& t )
{
	return t.has();
}

template<class T>
bool some( Option<T>&& t )
{
	return t.has();
}

template<class T>
bool none( Option<T>&& t )
{
	return !t.has();
}

template<class T>
bool none( const Option<T>& t )
{
	return !t.has();
}

//#define hasSome(o, v) if(auto v = o)
#endif

#if 0
/**/
template<class> class Maybe;

template<class T>
Maybe<T> Yes( const T& in )
{
	return Maybe<T>( in, true );
}
/**/

template<class T>
Maybe<T> Yes( T&& in )
{
	return Maybe<T>( in, true );
}
/**/
/*
template<class T>
Maybe<T> No()
{
	return Maybe<T>( T(), false );
}
/**/

template<class T>
class Maybe {
public:
	struct maybe {
		T t;
		bool b;
		explicit operator bool() const { return b; }
		explicit maybe( const T& t, bool b ) : t{ t }, b{ b } {}
	};

	auto yes() const
	{
		return maybe( (b ? t : T()), b );
	}

	auto no() const
	{
		return !b;
	}

	static Maybe No()
	{
		return Maybe( T(), false );
	}
	static Maybe Yes(const T& in)
	{
		return Maybe( in, true );
	}

private:
	Maybe( const T& t, bool b ) : t{ t }, b{ b } {}
	T t;
	bool b;
	friend Maybe Yes( const T& );
	friend Maybe Yes( T&& );
	//friend Maybe No();
};

// instead of 'void'; at least assignable
struct Unit {
//	operator void() {}
//	Unit() = default;
};

inline Unit call_with_unit( Unit = Unit() ) {
	return Unit();
}

#endif

//#undef EXPERIMENTAL_MARS_

#endif