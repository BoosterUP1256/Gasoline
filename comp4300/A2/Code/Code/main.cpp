#include <SFML/Graphics.hpp>

#include "Game.h"

int main()
{
	// Test Vec2 class

	assert(Vec2(0, 0) == Vec2(0, 0));
	assert(Vec2(0, 1) != Vec2(5, 5));

	assert(Vec2(5, 5) + Vec2(5, 5) == Vec2(10, 10));
	assert(Vec2(5, 5) - Vec2(5, 5) == Vec2(0, 0));
	assert(Vec2(5, 5) / 5 == Vec2(1, 1));
	assert(Vec2(5, 5) * 5 == Vec2(25, 25));

	Vec2 testVeca(5, 5);
	testVeca += Vec2(1, 1);
	assert(testVeca == Vec2(6, 6));

	Vec2 testVecb(5, 5);
	testVecb -= Vec2(1, 1);
	assert(testVecb == Vec2(4, 4));

	Vec2 testVecc(5, 5);
	testVecc *= 3;
	assert(testVecc == Vec2(15, 15));

	Vec2 testVecd(6, 6);
	testVecd /= 2;
	assert(testVecd == Vec2(3, 3));

	// Run game

	Game g("config.txt");
	g.run();
}