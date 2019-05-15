#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "Field3D.hpp"

TEST_CASE( "Verify X/Y/Z Size fields", "[Field3D]" ){

	int X = 20, Y = 30, Z = 40;
	Field3D field(X,Y,Z);

	REQUIRE( field.XSize == X );
	REQUIRE( field.YSize == Y );
	REQUIRE( field.ZSize == Z );
}

TEST_CASE( "Verify Last X/Y/Z Index fields", "[Field3D]" ){

	int X = 20, Y = 30, Z = 40;
	Field3D field(X,Y,Z);

	REQUIRE( field.LastXIndex == X-1 );
	REQUIRE( field.LastYIndex == Y-1 );
	REQUIRE( field.LastZIndex == Z-1 );
}
