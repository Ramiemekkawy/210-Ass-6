#include "Crucible.hh"


namespace cru
{
	using u8 = uint8_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	using isz = ptrdiff_t;
	using usz = size_t;
	const u32 seed_def = 518491;

	//! |-|ttps://en.wikipedia.org/wiki/MurmurHash#Algorithm
	u32 murmur( std::string const& str, u32 const seed )
	{
		auto const _rol32 =
			[]( u32 original, u32 bits ) {
			return (original << bits) | (original >> (32 - bits));
		};

		u32 const
			c1 = 0xcc9e2d51,
			c2 = 0x1b873593;
		u32 const
			r1 = 15,
			r2 = 13;
		u32 const
			m = 5,
			n = 0xe6546b64;

		auto hash = seed;

		auto const len = str.length() / 4;

		const u32* view = (const u32*)(str.c_str());

		for( usz p = 0; p < len; ++p ) {
			auto k = *(view + p);
			k *= c1;
			k = _rol32( k, r1 );
			k *= c2;

			hash ^= k;
			hash = _rol32( hash, r2 ) * m + n;
		}

		const u8* tail = (const u8*)(str.c_str() + len * 4);

		u32 k1 = 0;

		switch( len & 3 ) {
		case 3:
			k1 ^= *(tail + 2) << 16;

		case 2:
			k1 ^= *(tail + 1) << 8;

		case 1:
			k1 ^= *(tail);

			k1 *= c1;
			k1 = _rol32( k1, r1 );
			k1 *= c2;
			hash ^= k1;
		default:
			break;
		}

		hash ^= str.length();
		hash ^= (hash >> 16);
		hash *= 0x85ebca6b;
		hash ^= (hash >> 13);
		hash *= 0xc2b2ae35;
		hash ^= (hash >> 16);

		return hash;
	}


	u64 djb2( std::string str )
	{
		u32 hash = 5381;
		int c, i = 0;

		while( c = str[i++] )
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

		return hash;
	}

	u32 fnv1a( const std::string& str )
	{
		using ubyte = unsigned char;

		const u32
			offset = 2166136261,
			prime = 16777619;

		auto hash = offset;

		for( const ubyte& b : str ) {
			hash ^= b;
			hash *= prime;
		}

		return hash;
	}
}

namespace pandora
{

}

#if 0
template<class OutTy, class InTy>
OutTy myinto( InTy&& in )
{
	static_assert(0, "myinto is not implemented for your bloody type.");
}

template<class OutTy, class InTy>
OutTy myinto( const InTy& in )
{
	static_assert(0, "myinto is not implemented for your bloody type.");
}

template<>
std::string myinto( const int& in )
{
	return "3";
}

void foo()
{
	int l = 42;
	auto xs = myinto<std::string>( l );
}

#endif

// function piping
namespace C2____
{
#if 0
	template<class FnT, class Fst>
	struct Curry {
#if 0
		template<class... Args>
		auto operator()( Args&&... args )
		{
			return fn( arg1, args... );
		}
		FnT&& fn; // FIXME!
		Fst arg1;
#endif
	};

	template<class T, class F>
	struct Curry<T&&, F*> {
		template<class... Args>
		auto operator()( Args&&... args )
		{
			return (*this->fn)(arg1, args...);
		}
		Curry( F* in, T&& inarg ) : fn( in ), arg1( inarg ) {}

		Curry& operator-- ( int ) { return *this; }

		F* fn; // FIXME!
		T arg1;
	};

	template<class T, class F>
	struct Curry<T&, F*> {
		template<class... Args>
		auto operator()( Args&&... args )
		{
			return (*this->fn)(arg1, args...);
		}
		Curry( F* in, T& inarg ) : fn( in ), arg1( inarg ) {}
		Curry& operator-- ( int ) { return *this; }

		F* fn; // FIXME!
		T& arg1;
	};

	template<class T, class F>
	struct Curry<T&&, const F&> {
		template<class... Args>
		auto operator()( Args&&... args )
		{
			return fn( arg1, args... );
		}
		Curry( const F& in, T&& inarg ) : fn( in ), arg1( inarg ) {}
		Curry& operator-- ( int ) { return *this; }

		F fn; // FIXME!
		T arg1;
	};

	template<class T, class F>
	struct Curry<T&, const F&> {
		template<class... Args>
		auto operator()( Args&&... args )
		{
			return fn( arg1, args... );
		}
		Curry( const F& in, T& inarg ) : fn( in ), arg1( inarg ) {}
		Curry& operator-- ( int ) { return *this; }

		F fn; // FIXME!
		T& arg1;
	};

	// attempt at function piping in c++
	template<class In, class Fn>
	inline auto operator | ( const In& arg, const Fn& func )
	{
		return func( arg );
	}

	template<class In, class Fn>
	inline auto operator | ( In& arg, const Fn& func )
	{
		return func( arg );
	}

	template<class Fn>
	struct Adapt {
		template<class... Args>
		auto operator()( Args&&... args ) const
		{
			return (*this->funcp)(args...);
		}

		template<class... Args>
		auto operator()( const Args&... args ) const
		{
			assert( funcp != nullptr );
			return (*this->funcp)(args...);
		}

		Adapt( Fn* funcp_ ) : funcp( funcp_ ) {}
	private:
		Fn* const funcp;
	};

	template<class FnT>
	inline auto Fn( FnT* fnp ) -> Adapt<FnT>
	{
		return Adapt<FnT>( fnp );
	}

	// not working for some reason
	template<class In, class Fn>
	inline auto operator | ( const In& arg, Fn* funcp )
	{
		auto funcd = Adapt<Fn>( funcp );
		return funcd( arg );
	}

	template<class T, class F>
	inline auto curry( F&& f, T&& t )
	{
		return Curry<decltype(t), decltype(f)>( f, t );
	}

	template<class In, class Fn>
	inline auto operator >> ( const In& arg, const Fn& fun )
	{
		return curry( fun, arg );
	}


	/*
	bool original_ex( bool b, char c )
	{
	return true;
	}

	template<class... Args, class F>
	F example(Args&&... arg)
	{
	/*
	struct {
	template<class T>
	auto operator()( T&& a )
	{
	return original_ex( a, arg... );
	}
	} rv;
	/**
	return [ ]( auto a ) {  return original_ex( a, arg... ); };

	//return rv;
	}
	/**/

	//#define Fn(boo) Adapt<decltype(boo)>( &boo )

	inline void main()
	{
		//bool yes = false;
		auto yes = "blah" | []( const char* s ) { if( s ) return true; return false; } | []( const bool s ) { return 'n'; };
		char* ss = (char*)"asdasd";
		yes = ss | []( char* s ) { if( s ) return true; return false; };

		::cru::u64 n = "blooh" | Fn( ::cru::djb2 );
		//		auto ad = Adapt<decltype(boo)>( &boo );
		bool x = (true >> []( bool s, char j ) { return true; }) -- ('n'); // impossible to do with current precedence rules

																		   //auto xx = (false >> example( 's' ))();

		fmt::print( "{}\n", x );
	}

#endif

	inline void main() {}
}