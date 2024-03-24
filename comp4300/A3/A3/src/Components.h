#pragma once

#include "Animation.h"
#include "Assets.h"

struct Component
{	
	bool has = false;
};

struct CTransform : public Component
{
	Vec2 pos		= { 0.0, 0.0 };
	Vec2 prevPos	= { 0.0, 0.0 };
	Vec2 scale		= { 1.0, 1.0 };
	Vec2 velocity   = { 0.0, 0.0 };
	float angle     = 0;

	CTransform() = default;
	CTransform(const Vec2& p)
};