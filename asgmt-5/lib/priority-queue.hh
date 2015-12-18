#pragma once

#include <vector>
#include <algorithm>

#include "Utils.hh"
#include "Crucible.hh"

#define fn auto

namespace stdx
{
	namespace p = pandora;

	template<typename Ty, class Pred = std::less<Ty>>
	class BiHeap {
		using This = BiHeap;
		using Idx = ptrdiff_t;
		using TyOpt = pandora::Option<Ty>;
	public:
		static fn newEmpty() -> This
		{
			return This();
		}

		fn push( const Ty& in )
		{
			// case of having removed elements but now putting
			// more in
			if ( _last + 1 < _q.size() ) {
				++_last;
				_q[_last] = in;

				_upheap( _last );
			}
			// case of adding and adding and adding with little
			// removing; got to extend/push_back sometime :T
			else {
				++_last;
				_q.push_back( in );

				_upheap( _last );
			}
		}

		fn front() const -> Ty
		{
			return _q[1];
		}

		/// The same as 'pop' but returns the last available value before popping.
		/// Returns a pandora::Option object that is 'None' if the heap was empty
		/// when 'pull' was called; or contains the last available element (through
		/// 'front') otherwise.
		fn pull() -> TyOpt
		{
			if ( _last >= 1 ) {
				auto rv = p::Some( this->front() );
				// TODO : Downheap after popping
				this->pop();
				return rv;
			}
			else {
				return TyOpt();
			}
		}

		fn pop() -> bool
		{
			if ( _last >= 1 ) {
				_q[1] = _q[_last];
				--_last;
				_downheap( 1 );
				return true;
			}
			else {
				return false;
			}
		}

		bool empty() const
		{
			return _last <= 0;
		}

		BiHeap( This const& ) = default;
		BiHeap( This&& ) = default;
		BiHeap() : _q( 1 ), _last( 0 ), _cmp() {}
		BiHeap( Pred pred ) : _cmp( std::move( pred ) ), _last( 0 ), _q( 1 ) {}
	private:
		bool _upheap( Idx id )
		{
			if ( id < 2 )
				return false;
			while ( id >= 2 && _cmp( _q[id], _q[id / 2] ) ) {
				std::swap( _q[id], _q[id / 2] );
				id = id / 2;
			}
			return true;
		}

		bool _downheap( Idx id )
		{
			if ( id <= 0 || id >= _last )
				return false;
			while ( true ) {
				Idx
					// left child index
					lc = id * 2,
					// right child index
					rc = id * 2 + 1;
				bool
					// ordered with respect to left child? (true if none exists)
					bl = (lc <= _last ? _cmp( _q[id], _q[lc] ) : true),
					// ordered with respect to right child? (true if none exists)
					br = (rc <= _last ? _cmp( _q[id], _q[rc] ) : true)
					;

				// ordered to both children
				if ( bl && br ) {
					break;
				}
				// ordered to neither child
				else if ( !bl && !br ) {
					bool bc = _cmp( _q[lc], _q[rc] );
					std::swap( _q[id], (bc ? _q[lc] : _q[rc]) );
					id = (bc ? lc : rc);
				}
				// ordered between the two children
				else {
					if ( !bl ) {
						std::swap( _q[id], _q[lc] );
						id = lc;
					}
					else {
						std::swap( _q[id], _q[rc] );
						id = rc;
					}
				}
			}
			return true;
		}
	private:
		// TODO make use of pandora::Vec isntead soon
		std::vector<Ty> _q;
		/// Index of last node populated
		Idx _last;

		/// 
		const Pred _cmp;
	};














	template<
		typename Data,
		typename Key = unsigned int,
		typename Pred = std::less<>
	>
		// Turn into a Fibonacci heap.
	class PriorityQueue {
		using This = PriorityQueue;
		using Idx = ptrdiff_t;
		struct Elt;	using EltOpt = pandora::Option<Elt>;
	public:
		struct Elt {
			Key key;
			Data data;
			Elt() : key(), data() {}
			operator std::pair<Key, Data>() const
			{
				return{ key, data };
			}
			operator std::tuple<Key, Data>() const
			{
				return{ key, data };
			}
			Elt( Key k, Data d ) : key( std::move( k ) ), data( std::move( d ) ) {}
			Elt( std::pair<Key, Data> p ) : Elt( p.first, p.second ) {}
		};

		static fn newEmpty() -> This
		{
			return This();
		}

		fn push( const Elt& in )
		{
			// case of having removed elements but now putting
			// more in
			if ( _last + 1 < _q.size() ) {
				++_last;
				_q[_last] = in;

				_upheap( _last );
			}
			// case of adding and adding and adding with little
			// removing; got to extend/push_back sometime :T
			else {
				++_last;
				_q.push_back( in );

				_upheap( _last );
			}
		}

		fn front() const -> Elt
		{
			return _q[1];
		}

		/// The same as 'pop' but returns the last available value before popping.
		/// Returns a pandora::Option object that is 'None' if the heap was empty
		/// when 'pull' was called; or contains the last available element (through
		/// 'front') otherwise.
		fn pull() -> EltOpt
		{
			if ( _last >= 1 ) {
				auto rv = p::Some( this->front() );
				this->pop();
				return rv;
			}
			else {
				return EltOpt();
			}
		}

		fn pop() -> bool
		{
			if ( _last >= 1 ) {
				_q[1] = _q[_last];
				--_last;
				_downheap( 1 );
				return true;
			}
			else {
				return false;
			}
		}

		bool empty() const
		{
			return _last <= 0;
		}

		bool set_priority( const Data& target, Key newp )
		{
			Idx id;
			for ( auto& e : _q ) {
				if ( e.data == target ) {
					e.key = newp;
					id = &e - &_q[0];
					break;
				}
			}

			if ( !_upheap( id ) )
				_downheap( id );

			return true;
		}

		PriorityQueue( This const& ) = default;
		PriorityQueue( This&& ) = default;
		PriorityQueue() : _q( 1 ), _last( 0 ), _cmp() {}
		PriorityQueue( Pred pred ) : _cmp( std::move( pred ) ), _last( 0 ), _q( 1 ) {}
	private:
		bool _upheap( Idx id )
		{
			if ( id < 2 )
				return false;
			while ( id >= 2 && _cmp( _q[id].key, _q[id / 2].key ) ) {
				std::swap( _q[id], _q[id / 2] );
				id = id / 2;
			}
			return true;
		}

		bool _downheap( Idx id )
		{
			if ( id <= 0 || id >= _last )
				return false;
			while ( true ) {
				Idx
					// left child index
					lc = id * 2,
					// right child index
					rc = id * 2 + 1;
				bool
					// ordered with respect to left child? (true if none exists)
					bl = (lc <= _last ? _cmp( _q[id].key, _q[lc].key ) : true),
					// ordered with respect to right child? (true if none exists)
					br = (rc <= _last ? _cmp( _q[id].key, _q[rc].key ) : true)
					;

				// ordered to both children
				if ( bl && br ) {
					break;
				}
				// ordered to neither child
				else if ( !bl && !br ) {
					bool bc = _cmp( _q[lc].key, _q[rc].key );
					std::swap( _q[id], (bc ? _q[lc] : _q[rc]) );
					id = (bc ? lc : rc);
				}
				// ordered between the two children
				else {
					if ( !bl ) {
						std::swap( _q[id], _q[lc] );
						id = lc;
					}
					else {
						std::swap( _q[id], _q[rc] );
						id = rc;
					}
				}
			}
			return true;
		}
	private:
		// TODO make use of pandora::Vec isntead soon
		std::vector<Elt> _q;
		/// Index of last node populated
		Idx _last;

		/// 
		const Pred _cmp;
	};

	/**
	template<class T, class Pred>
	const Pred stdx::BiHeap<T, Pred>::_cmp = Pred();
	/**/
	template<class T>
	using MaxHeap = BiHeap<T, std::greater<>>;

	template<class T>
	using MinHeap = BiHeap<T>;

#if 0
	// used to need it to support lambda predicates
	// works fine now without, though.
	template<class Func>
	struct _hSortHelper {
		template<class... A>
		auto operator()( const A&... a ) const
		{
			return f( a... );
		}
		_hSortHelper( Func in ) : f( std::move( in ) ) {}
		_hSortHelper() : f() {}
		Func f;
	};
#endif

	/// HeapSort in-place; Default comparison requires 'operator<' defined
	/// But allows functor, function pointer and lambda predicates as well
	/// given that they guarantee a total order for the data-type being sorted.
	template<class Data = void, class Func = std::less<>>
	bool heap_sort_ip( std::vector<Data> *const inout, Func fun = std::less<>() )
	{
		BiHeap<Data, Func> heap( fun );

		for ( auto&& elt : *inout ) {
			heap.push( std::move( elt ) );
		}

		for ( auto& out : *inout ) {
			out = heap.pull().unwrap();
		}
		return true;
	}

	// dummy
	inline void x()
	{
		auto s = BiHeap<int>::newEmpty();
		std::vector<float> ff( 999, 99 );
		heap_sort_ip( &ff ); // pointers to make it more obvious
		// at place of use.
	}
}