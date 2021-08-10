#pragma once
#include "Game2D.h"
#include "Vector2.h"

namespace jm {
	class Bar {
	public:
		vec2 pos = vec2{ 0.0f, -0.8f };

		void draw() {
			beginTransformation();
			translate(pos);
			drawFilledBox(Colors::gold, 0.1f, 0.02f);
			drawWiredBox(Colors::white, 0.1f, 0.02f);
			endTransformation();
		}

		void update(const float& dt) {
			if(pos.x + 0.3 * dt <= 0.47f && pos.x + 0.3 * dt >= -0.47f)
				pos.x += 0.3 * dt;
		}
	};
}