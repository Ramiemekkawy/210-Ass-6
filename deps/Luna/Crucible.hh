#pragma once

#ifndef Crucible_HH_
#define Crucible_HH_

#include <cstdint>
#include <string>
#include <vector>
#include <cassert>
#include <exception>
#include <stdexcept>
#include <sstream>

#include <fstream>
#include <iostream>
#include <bitset>

#include <memory>
#include <utility>
#include <algorithm>

#include "format.h"
#include "Utils.hh"

namespace cru
{
	// hashing
	using u32 = uint32_t;
	using u64 = uint64_t;
	extern const u32 seed_def;

	// murmur 3
	//! https://en.wikipedia.org/wiki/MurmurHash#Algorithm
	u32 murmur( std::string const& str, u32 const seed = seed_def );

	// djb2
	u64 djb2( std::string str );
}


namespace pandora
{
	//DeclFnAlias( std::move, move );
	using std::move;

	template<class>
	class Option;

	template<class T, class D = std::remove_reference_t<T>>
	Option<D> Some( const T& in )
	{
		return Option<D>( in );
	}

	template<class T, class D = std::remove_reference_t<T>>
	Option<D> Some( T&& in )
	{
		return Option<D>( in );
	}

	template<class T>
	T some(const Option<T>& inref)
	{
		if ( !inref.nil() )
			return inref.val();
		else
			return T();
	}

	template<class T>
	class Option {
	public:
		explicit operator bool() const
		{
			return !_nil;
		}

		Option() : _nil( true ) {}
		Option( const T& in ) : _nil( false ), _val( in ) {}
		Option( T&& in ) : _nil( false ), _val( move( in ) ) {}

		T val() const
		{
			return _val;
		}

		T unwrap() const
		{
			if ( nil() )
				throw std::runtime_error( "Accessed nil Option" );
			return _val;
		}

		void val( const T& in )
		{
			_val = in;
			_nil = false;
		}

		void val( T&& in )
		{
			_val = in;
			_nil = false;
		}

		bool nil() const
		{
			return _nil;
		}

		void nilly()
		{
			_nil = true;
		}

		T& as_mut()
		{
			if ( nil() )
				throw std::runtime_error( "Accessed nil Option" );
			return _val;
		}

		T unwrap_or( T else_opt ) const
		{
			if ( nil() )
				return else_opt;
			
			return _val;
		}

		template<class Func>
		auto map( const Func& func ) -> Option<decltype(func( _val ))>
		{
			if( some() )
				return Option<decltype(func( _val ))>( func( _val ) );
			else
				return Option<decltype(func( _val ))>();
		}


		bool some() const
		{
			return !_nil;
		}

	private:
		bool _nil;
		T _val;
	};

#if 1
	/// UNFINISHED
	/// TODO : everything
	template<typename DataTy>
	class Vec {
		using isz = ptrdiff_t;

	public:
		Vec() : Vec( 64 ) {}
		static Vec newWithCap( isz cap )
		{
			return Vac( cap );
		}
		static Vec new_N_with( isz size, DataTy init )
		{
			return Vec( size, init );
		}

		Vec slice( isz from, isz till )
		{
			return Vec( _core + from, _core + till - 1 );
		}

		Vec slice_from( isz from )
		{
			return Vec( _core + from, _core + size - 1 );
		}

		Vec slice_till( isz till )
		{
			return Vec( _core, _core + till - 1 );
		}

		bool push_back( const DataTy& in )
		{
			if( len() < capacity() ) {
				++_size;
				_core[_size] = in;
				return true;
			}
			else {
				try {
					DataTy* x = new DataTy[_capacity * 2];
				}
				catch( ... ) {
					return false;
				}
				_copy( _core, x, 0, _size );
			}

			return true;
		}

		isz len() const
		{
			return _size;
		}

		isz capacity() const
		{
			return _capacity;
		}

	private:
		Vec( isz cap )
			:
			_core( new DataTy[cap] ),
			_capacity( cap ),
			_size( 0 )
		{}
		Vec( isz size, DataTy init )
			:
			_core( new DataTy[newSize( size )] ),
			_capacity( newSize( size ) ),
			_size( size )
		{
			for( auto ptr = _core; (ptr != (_core + size)); ++ptr ) {
				*ptr = init;
			}
		}
		Vec( DataTy* start, DataTy* end )
			:
			_core( start ),
			_size( cast( isz )(end - start + 1) ),
			_capacity( _size )
		{}

		static isz newSize( isz in )
		{
			if( n % 2 == 0 ) {
				return n;
			}
			else {
				if( n >= 256 )
					return n + 1;
				else
					return n * 2;
			}
		}

	private:
		/// FIXME! Use a smart pointer?
		DataTy * _core;
		isz _capacity;
		isz _size;
	};

	/// UNFINISHED
	/// TODO : everything
	class String {
		using isz = size_t;
	public:
		String( const char* in ) : _str( in ) {}
		String( const std::string& in ) : _str( in ) {}

		isz len() const
		{
			return _str.length();
		}

		bool empty() const
		{
			return _str.empty();
		}

		isz find( const std::string& sub ) const
		{
			return _str.find( sub );
		}

		isz find( const char sub ) const
		{
			return _str.find( sub );
		}

		char& at( isz idx )
		{
			return _str.at( idx );
		}

		String substr( isz start, isz span = -1 ) const
		{
			if( span > -1 )
				return{ _str.substr( start, span ) };
			else
				return{ _str.substr( start ) };
		}

		String substr( const std::string& from, const std::string& till ) const
		{
			auto start = _str.find( from );
			auto end = _str.find( till );

			if( start != std::string::npos && end != std::string::npos )
				return{ _str.substr( start, end - start ) };

			return{ "" };
		}

		std::vector<String> split_at( const std::string& split_token ) const
		{
			auto tmp_str = _str;


		}

		void trim() {}

		char* as_ptr()
		{
			return &_str.front();
		}



	private:
		std::string _str;
	};


	struct GetLines {
		GetLines()
			: _empty( false )
		{
			std::getline( std::cin, _str );
		}

		GetLines begin() const
		{
			return GetLines();
		}

		std::string operator*() const
		{
			return _str;
		}

		void operator++()
		{
			std::getline( std::cin, _str );
			if( _str == "END?!!" )
				_empty = true;
		}

		GetLines end() const
		{
			return GetLines( true );
		}

		bool operator!=( const GetLines& rhs ) const
		{
			return this->_empty != rhs._empty;
		}

	private:
		GetLines( bool b ) : _empty( b ) {}
		bool _empty;
		std::string _str;
	};

	template<class T>
	struct IInputRange {
		virtual bool empty() const = 0;
		virtual T front() const = 0;
		virtual void pop() = 0;
		//virtual ~IInputRange();
	};

	template<class Range, class Func>
	void foreach( IInputRange<Range>& range, const Func& func )
	{
		while ( !range.empty() ) {
			auto elt = range.front();
			func( elt );
			range.pop();
		}
	}

	inline auto iota( int from, int till )
	{
		struct R : IInputRange<int> {
			bool empty() const
			{
				return _f > _t;
			}

			int front() const
			{
				return _f;
			}

			void pop()
			{
				if( _f < _t )
				++_f;
			}

			R(int f, int t)
				: _f(f), _t(t - 1)
			{

			}

		private:
			int _f, _t;
		};

		return R( from, till );
	}

	struct RGetLines : IInputRange<std::string> {
	private:
		using Str = std::string;
	public:
		RGetLines()
			: _empty( false ),
			_in_str( std::cin )
		{
			this->pop();
		}

		RGetLines( std::istream& ins )
			: _empty( false ),
			_in_str( move( ins ) )
		{
			this->pop();
		}

		Str front() const
		{
			return _str;
		}

		Str& front()
		{
			return _str;
		}

		Option<Str> pull( char delim = '\n' )
		{
			if( !empty() ) {
				this->pop();
				return{ _str };
			}
			else
				return{};

		}

		void pop() override
		{
			pop( '\n' );
		}

		bool pop( char delim )
		{
			std::getline( _in_str, _str, delim );

			if( _in_str.eof() )
				_empty = true;

			return !_empty;
		}

		bool empty() const override
		{
			return _empty;
		}

		virtual ~RGetLines() {}

	private:
		bool _empty;
		Str _str;
		std::istream& _in_str;
	};




	struct sinto {\

	};

	template<class T>
	struct FromT {
		T t;
	};

	template<class F>
	FromT<F> operator>>( F in, sinto&& s )
	{
		return FromT<F>{ std::move( in ) };
	}

	template<class F, class T>
	T operator>>( FromT<F>&& from, T& into )
	{
		std::stringstream ss;
		ss << from.t;
		ss >> into;
		return into;
	}

	template<class T>
	struct ImplInto {
		T t;
		operator T()
		{
			return std::move( t );
		}
	};

	template<class F, class T>
	T operator>>( FromT<F>&& from, ImplInto<T> into )
	{
		std::stringstream ss;
		ss << from.t;
		T t;
		ss >> t;
		return t;
	}

#define _into(x) >> sinto() >> ImplInto<x>()
#define _into_ >> sinto() >>

	inline void foo()
	{
		int s = 0;

		std::string ss;

		auto x = s _into_ ss;

		auto v = 34 _into( std::string );
	}
#endif
}

#endif // Crucible_HH_

