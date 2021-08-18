#pragma once
#include "Game2D.h"
#include "Vector2.h"

namespace jm {
	class Bar {
	public:
		vec2 pos = vec2{ 0.0f, -0.8f };
		float width = 0.1f;
		float height = 0.02f;
		void draw() {
			beginTransformation();
			translate(pos);
			drawFilledBox(RGB(242, 226, 198), width, height);
			drawWiredBox(Colors::white, width, height);
			endTransformation();
		}

		void update(const float& dt) {
			if(pos.x + 0.3 * dt <= 0.47f && pos.x + 0.3 * dt >= -0.47f)
				pos.x += 0.3 * dt;
		}
	};
}