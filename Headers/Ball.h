#pragma once
#include "Game2D.h"

namespace jm {
	class Ball {
	public:
		vec2 pos =vec2(0.0f, 2.0f);
		vec2 vel = vec2(1.0f, 2.0f);
		float radius = 0.01f;
		const float coef_res = 1.0f; // 반발계수 

		void draw() {
			beginTransformation();
			{
				translate(pos);
				drawFilledCircle(Colors::green, radius - 1e-3f);
				setLineWidth(1.0f);
				drawWiredCircle(Colors::white, radius);
			}
			endTransformation();
		}

		void update(const float& dt) {
			pos = pos + vel * dt;
		}

		void CheckCollision(const Bar & bar) {
			const float  barup = bar.pos.y + 0.01f;
			const float barright = bar.pos.x + 0.05f;
			const float  barleft = bar.pos.x - 0.05f;

			if (this->pos.y < barup - 0.01f)
				return;
			if (this->pos.x >= barleft && this->pos.x <= barright && this->pos.y <= barup) // 바와 충돌
			{
				this->pos.y = barup + radius;

				if (vel.y <= 0.0f)
					vel.y *= -1.0f * coef_res;
			}
			if (this->pos.y >= 1.0f) // 위쪽 벽과 충돌
			{
				this->pos.y = 1.0f - radius;

				if (vel.y >= 0.0f)
					vel.y *= -1.0f * coef_res;
			}

			if (this->pos.x >= 0.515f) // 오른쪽 벽과 충돌
			{
				this->pos.x = 0.515f - radius;

				if (vel.x >= 0.0f)
					vel.x *= -1.0f * coef_res;
			}

			if (this->pos.x <= -0.515f) // 왼쪽 벽과 충돌
			{
				this->pos.x = -0.515f + radius;

				if (vel.x <= 0.0f)
					vel.x *= -1.0f * coef_res;
			}
		}
	};
}