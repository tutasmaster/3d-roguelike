#pragma once
#include <libtcod.hpp>
#include "Map.h"
class Effect
{
public:
	bool erase = false;
	char ch = '^';
	TCODColor bg = TCODColor::gold;
	TCODColor col = TCODColor::red;

	Map::Pos pos;
	float time = 0.0f;

	virtual void Update(int pos) = 0;
};

class FireEffect : public Effect {
public:
	void Update(int pos) override;
};
