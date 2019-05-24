#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "Field3D.hpp"

#define FOR_EACH_CELL(field)                        \
	for(int i = 0; i < field.XSize(); ++i){         \
		for(int j = 0; j < field.YSize(); ++j){     \
			for(int k = 0; k < field.ZSize(); ++k)

#define END_FOR }}

#define FILL_FIELD(field,expression) \
	FOR_EACH_CELL(field){ field(i,j,k) = (expression); } END_FOR

TEST_CASE( "Verify X/Y/Z Size fields", "[Field3D]" ){

	int X = 20, Y = 30, Z = 40;
	Field3D field(X,Y,Z);

	REQUIRE( field.XSize() == X );
	REQUIRE( field.YSize() == Y );
	REQUIRE( field.ZSize() == Z );
}

TEST_CASE( "Verify Last X/Y/Z Index fields", "[Field3D]" ){

	int X = 20, Y = 30, Z = 40;
	Field3D field(X,Y,Z);

	REQUIRE( field.XLast() == X-1 );
	REQUIRE( field.YLast() == Y-1 );
	REQUIRE( field.ZLast() == Z-1 );
}

TEST_CASE( "Negative indexing", "[Field3D::operator()]" ){

	double c1 = 321.456, c2 = 666.555, c3 = 876.543, c4 = 7.89;
	int X = 20, Y = 30, Z = 40;
	Field3D f(X,Y,Z);

	f(-1, 5, 5) = c1;
	f(5, -1, 5) = c2;
	f(5, 5, -1) = c3;
	f(-1, -1, -1) = c4;

	REQUIRE( f(f.XLast(), 5, 5) == c1 );
	REQUIRE( f(5, f.YLast(), 5) == c2 );
	REQUIRE( f(5, 5, f.ZLast()) == c3 );
	REQUIRE( f(f.XLast(), f.YLast(), f.ZLast()) == c4 );
	
}

TEST_CASE( "Successful copy between 2 fields", "[Field3D::copyFrom]" ){

	int X = 20, Y = 30, Z = 40;
	Field3D f(X,Y,Z), g(X,Y,Z);

	FILL_FIELD(f, 1 + i*j*k);
	g.copyFrom(f);

	FOR_EACH_CELL(g){
		if(g(i,j,k) != f(i,j,k))
			FAIL("copyFrom failed");
		g(i,j,k) += 1.0;
		if(g(i,j,k) == f(i,j,k))
			FAIL("Fields entangled");
	}END_FOR
}
