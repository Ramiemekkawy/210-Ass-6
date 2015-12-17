#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <cstdio>
#include <functional>

#include "Utils.hh"
#include "priority-queue.hh"


void test()
{
	using namespace pandora;
	using namespace stdx;
	auto console_lines = RGetLines();
	auto qu = MaxHeap<std::string>();
	foreach( console_lines, [&qu]( auto x ) { qu.push( x ); } );

	std::istream* ins = &std::cin;

	auto rng = pandora::iota( 1, 10 );
	pandora::foreach( rng, []( auto x ) {
		std::cout << x;
	} );

	pandora::RGetLines gline;

	while ( !gline.empty() ) {
		auto ln = gline.pull().unwrap();
		// parse the line for year + 12 temps
		auto year = ln.substr( 0, 4 );
		// actually do the input
	}
}

struct GSMap {
	struct Pt {
		int j, i;
		Pt( const Pt& ) = default;
		Pt() = default;
		friend std::ostream& operator<< ( std::ostream& from, Pt const in )
		{
			from << "(x: " << (in.i) << ", y: " << (in.j) << ")";
			return from;
		}
	};

	struct Pixel {
		Pt pos;
		float val;
		bool operator<( Pixel const& rhs ) const
		{
			return this->val < rhs.val;
		}
		Pixel() = default;
	};
public:
	GSMap( const int n )
		: _map( n, std::vector<float>( n ) )
	{

	}

	std::pair<Pixel, Pixel> closest_pair() const
	{
		auto const abs = []( auto x ) {
			return (x < 0 ? -x : x);
		};

		std::vector<Pixel> out;

		out.reserve( _map.size() * _map[0].size() );

		stdx::MinHeap<Pixel> heap{};

		auto const jsize = _map.size();

		auto const isize = _map[0].size();

		for ( auto j = 0; j < jsize; ++j ) {
			for ( auto i = 0; i < isize; ++i ) {
				heap.push( { {j, i}, _map[j][i] } );
			}
		}

		while ( !heap.empty() ) {
			auto elt = heap.pull().unwrap();
			out.push_back( elt );
		}

		auto diff = abs( out[1].val - out[0].val );
		size_t id1 = 0, id2 = 1;

		/**/
		for ( size_t i = 2; i < out.size(); ++i ) {
			auto diff2 = abs( out[i].val - out[i - 1].val );
			//fmt::print( "\n{} : {}", diff, diff2 );
			if ( diff2 < diff ) {
				diff = diff2;
				id1 = i - 1;
				id2 = i;
			}
		}
		/**/

		return{ out[id1], out[id2] };
	}

	void re_populate()
	{
		std::uniform_real_distribution<float> distr( 0.0, 100.0 );
		std::random_device rd;
		std::mt19937 eng( rd() );

		for ( auto& j : _map ) {
			for ( auto& i : j ) {
				i = distr( eng );
			}
		}
	}

	void print_ascii() const
	{
		using std::cout;
		for ( auto const& row : _map ) {
			for ( auto const& elt : row ) {
				float d = 100.f / 4;

				if ( elt <= d ) {
					cout << ".";
				}
				else if ( elt <= 2 * d ) {
					cout << ":";
				}
				else if ( elt <= 3 * d ) {
					cout << "-";
				}
				else if ( elt <= 4 * d ) {
					cout << "=";
				}
				//cout << " ";
			}
			cout << "\n";
		}
	}

private:
	std::vector<std::vector<float>> _map;
};

bool problem_1()
{
	int board_dim = 40;

	fmt::print( "Please input a number for the dimensions of the distribution\n>> " );
	std::cin >> board_dim;

	while ( board_dim < 2 || board_dim > 640 ) {
		fmt::print( "Error, {} is not a valid size."
					"\nPlease enter a number between 2 and 640\n>> ", board_dim );
		std::cin >> board_dim;
	}

	// board of the specified dimension; unpopulated so far
	auto example_board = GSMap( board_dim );

	// create a randomized population on the board;
	example_board.re_populate();

	example_board.print_ascii();

	// magic happened! Found the two points of the closest values.
	auto points = example_board.closest_pair();

	fmt::print( "\n> Closest points at positions {} and {}"
				"\nof values {} and {}.\n",
				points.first.pos, points.second.pos,
				points.first.val, points.second.val
				);

	return true;
}

bool problem_2()
{
	struct YearTemp {
		std::vector<std::array<int, 13>> table;
		YearTemp()
			: table( 0 )
		{}
		void add_row()
		{
			table.push_back( {} );
		}
	};

	YearTemp yt;

	std::ifstream ifs;
	std::string inpath;

	{
		std::cin.ignore();
		fmt::print( "\nPlease enter path to input file of temperature data\n>>" );
		std::getline( std::cin, inpath, '\n' );
		ifs.open( inpath );

		while ( !ifs.is_open() || ifs.fail() ) {
			fmt::print( "\n<!> Error: File path not found or corrupt!\n>>" );
			std::getline( std::cin, inpath, '\n' );
			ifs.open( inpath );
		}
	}

	int* buff;

	std::string tok = "";

	ifs >> tok;

	if ( tok.find( "START" ) == std::string::npos ) {
		fmt::print( "\n<!> Error: Format of file in path \"{}\" is invalid.\n", inpath );
		return false;
	}

	// parse input
	for ( int i = 0; ; ++i ) {
		ifs >> tok;

		if ( tok == "END" )
			break;
		else {
			yt.add_row();

			buff = yt.table[i].data();

			buff[0] = std::stoi( tok );
		}

		// no validation because streams are a headache;
		// I want a C++ streams `scanf` :'(
		for ( int i = 1; i <= 12; ++i ) {
			ifs >> buff[i];
		}
	}

	struct MonthTemp {
		int
			temp,
			month,
			year;
	};

	std::vector<MonthTemp> temps;

	for ( auto& arr : yt.table ) {
		for ( size_t i = 1; i < 13; ++i ) {
			temps.push_back( { arr[i], cast( int )(i), arr[0] } );
		}
	}

	stdx::heap_sort_ip( &temps, []( auto x, auto y ) { return x.temp < y.temp; } );

	/**/
	fmt::print( "\n> Global 10 minimum temperature changes:\n" );
	for ( size_t idx = 0; idx < 10 && idx < temps.size(); ++idx ) {
		auto current = temps.at( idx );
		fmt::print( "{}) {}/{} : {}C\n",
					idx + 1,
					current.year, current.month, current.temp );
	}

	fmt::print( "\n> Global 10 maximum temperature changes:\n" );
	for ( size_t end = temps.size(),
		  idx = end - 1;
		  idx >= end - 10; --idx ) {
		auto current = temps.at( idx );
		fmt::print( "{}) {}/{} : {}C\n",
					end - idx,
					current.year, current.month, current.temp );
	}

	stdx::heap_sort_ip( &temps, []( auto x, auto y ) {
		return (x.year == y.year
				 ? x.month < y.month
				 : x.year < y.year);
	} );

	std::vector<double> accu( temps.size() + 5, 0.0 );

	size_t
		idmax = 1,
		idmin = 1;

	for ( size_t idx = 1; idx <= temps.size(); ++idx ) {
		accu[idx] = temps[idx - 1].temp + accu[idx - 1];
		if ( accu[idx] > accu[idmax] )
			idmax = idx;
		if ( accu[idx] < accu[idmin] )
			idmin = idx;
	}

	// in case the found max and min are in the wrong order
	// I need to find a contiguous subarray with the min index
	// strictly before the max index for the accumulation array
	// to make sense.
	// So here I look for another subarray if the first search fails.
	if ( idmax < idmin ) {
		size_t
			min2 = 1,
			max2 = idmin + 1;
		for ( size_t i = 1; i < idmax; ++i ) {
			if ( accu[i] < accu[min2] )
				min2 = i;
		}

		for ( size_t i = max2; i <= temps.size(); ++i ) {
			if ( accu[i] > accu[max2] )
				max2 = i;
		}

		if ( max2 - idmin > idmax - min2 ) {
			idmax = max2;
		}
		else
			idmin = min2;
	}

	fmt::print( "> Maximum sum in subarray occurred within the range:"
				"\n{}/{} <-> {}/{}\n",
				temps[idmin].year, temps[idmin].month,
				temps[idmax - 1].year, temps[idmax - 1].month );

	return true;
}

int main()
{
	bool
		p1 = problem_1(),
		p2 = problem_2();

	return 0;
}