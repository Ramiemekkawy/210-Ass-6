#include "Edge.h"



bool Edge::operator<=( const Edge &e )
{
	return (w <= e.w);
}

bool Edge::operator<( const Edge &e )
{
	return (w < e.w);
}
