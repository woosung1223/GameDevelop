#pragma once
#include "Game2D.h"
#include "RandomNumberGenerator.h"
#include <vector>
namespace jm {
	class Ball {
	public:
		vec2 pos =vec2(0.0f, 1.0f);
		vec2 vel = vec2(-1.0f, -2.0f);
		float radius = 0.01f;

		~Ball() {
			std::cout << "deleted" << std::endl;
		}
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
	};

	class BallHandler {
	public:
		// balls[0]은 main Ball. multiply 는 balls[0]를 기준으로 진행.
		vector <Ball*> balls;
		RandomNumberGenerator RN;
		const float coef_res = 1.0f; // 반발계수 

		BallHandler() {
			balls.reserve(100);
			// 100개의 공까지 할당 가능
		}
		void PushBall() {
			int startidx = balls.size();
			balls.push_back(new Ball);
			
			if (balls.size() != 1) {
				for (int i = startidx; i < balls.size(); i++) {
					balls[i]->pos = balls[0]->pos;
					balls[i]->vel = { RN.getFloat(-1.0f, 1.0f) + 2.0f, RN.getFloat(-1.0f, 1.0f) + 2.0f};
				}
			}
		}

		void PopBall() {
			// 각각의 ball을 검사해서 영역을 벗어났으면 삭제
			for (auto& i : balls) {
				if (i != nullptr && i->pos.y <= -1.2f) {
					delete i;
					i = nullptr;
				}
			}
		}
		bool GameOver() {
			// 게임이 끝났는지. 공이 전부 없어졌으면 게임오버
			for (auto& i : balls) {
				if (i != nullptr) return false;
			}
			return true;
		}
		void draw() {
			for (auto& i : balls) {
				if (i != nullptr)
					i->draw();
			}
		}	
		void update(const float& dt) {
			for (auto& i : balls) {
				if (i != nullptr)
					i->pos += i->vel * dt;
			}
		}
		void CheckCollision(const Bar& bar) {
			const float  barup = bar.pos.y + 0.01f;
			const float barright = bar.pos.x + 0.05f;
			const float  barleft = bar.pos.x - 0.05f;
			for (auto& i : balls) {
				if (i != nullptr) {
					if (i->pos.y < barup - 0.01f)
						continue;
					if (i->pos.x >= barleft && i->pos.x <= barright && i->pos.y <= barup) // 바와 충돌
					{
						i->pos.y = barup + i->radius;

						if (i->vel.y <= 0.0f)
							i->vel.y *= -1.0f * coef_res;
					}
					if (i->pos.y >= 1.0f) // 위쪽 벽과 충돌
					{
						i->pos.y = 1.0f - i->radius;

						if (i->vel.y >= 0.0f)
							i->vel.y *= -1.0f * coef_res;
					}

					if (i->pos.x >= 0.515f) // 오른쪽 벽과 충돌
					{
						i->pos.x = 0.515f - i->radius;

						if (i->vel.x >= 0.0f)
							i->vel.x *= -1.0f * coef_res;
					}

					if (i->pos.x <= -0.515f) // 왼쪽 벽과 충돌
					{
						i->pos.x = -0.515f + i->radius;

						if (i->vel.x <= 0.0f)
							i->vel.x *= -1.0f * coef_res;
					}
				}
			}
		}
	};
}