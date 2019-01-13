#include "Effect.h"

#include "engine.hpp"


void FireEffect::Update(int pos) {
	time += 0.01f;
	col.setValue(col.getValue() - 0.01f);
	bg.setValue(bg.getValue() - 0.01f);

	if (time > 1.0f) {
		erase = true;
	}
}