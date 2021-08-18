#pragma once
#include "Game2D.h"
#include "RandomNumberGenerator.h"
#include <vector>
namespace jm {
	class Ball {
	public:
		vec2 pos =vec2(0.0f, -0.3f);
		vec2 vel = vec2(-1.0f, -2.0f);
		RGB color = RGB(130, 182, 149);
		float radius = 0.01f;

		~Ball() {
			std::cout << "deleted" << std::endl;
		}
		void draw() {
			beginTransformation();
			{
				translate(pos);
				drawFilledCircle(color, radius - 1e-3f);
				setLineWidth(1.0f);
			/*	drawWiredCircle(Colors::white, radius);*/
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
		int ballnum = 0;
		const float coef_res = 1.0f; // 반발계수 

		BallHandler() {
			balls.reserve(100);
			// 100개의 공까지 할당 가능
		}
		void Faster() {
			for (auto& ball : balls) {
				if(ball != nullptr)
					ball->vel *= 1.5f;
			}
		}
		void Slower() {
			for (auto& ball : balls) {
				if (ball != nullptr)
					ball->vel /= 1.5f;
			}
		}
		void Multiply() {
			int first;
			int pre_ballnum = ballnum;
			for (int i = 0; i < pre_ballnum; i++) {
				for (int i = 0; i < pre_ballnum; i++) {
					if (balls[i] != nullptr) {
						first = i;
						break;
					} // 첫번째 공을 기준으로 multiply
				}
				if (balls[i] != nullptr) {
					Ball* temp = new Ball;
					temp->pos = balls[first]->pos;
					temp->vel = vec2(RN.getFloat(-1.0f, 1.0f) + 2.0f, RN.getFloat(-1.0f, 1.0f) + 2.0f);
					balls.push_back(temp);
					ballnum++;
				}
			}
		}
		void PushBall() {
			ballnum++;
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
			// 게임이 끝났는지, 공이 전부 없어졌으면 게임오버
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
			const float  barup = bar.pos.y + bar.height / 2.0f;
			const float barright = bar.pos.x  + bar.width / 2.0f;
			const float  barleft = bar.pos.x - bar.width / 2.0f;
			for (auto& i : balls) {
				if (i != nullptr) {
					if (i->pos.y < barup - 0.01f)
						continue;
					else if (i->pos.x >= barleft && i->pos.x <= barright && i->pos.y <= barup) // 바와 충돌
					{
						i->pos.y = barup + i->radius;
						// 바를 중심으로 좌측에 충돌하면 좌측으로 튕겨내고 우측에 충돌하면 우측으로 튕겨냄 
						if (i->vel.y <= 0.0f) {
								i->vel.y *= -1.0f * coef_res;

							// 좌측에 맞은 경우
							if (i->pos.x <= bar.pos.x) {
								if (i->vel.x >= 0.0f)
									i->vel.x *= -1.0f * coef_res;
							}
							// 우측에 맞은 경우
							else if (i->pos.x > bar.pos.x) {
								if (i->vel.x <= 0.0f)
									i->vel.x *= -1.0f * coef_res;
							}
						}
					}
					else if (i->pos.y >= 1.0f) // 위쪽 벽과 충돌
					{
						i->pos.y = 1.0f - i->radius;

						if (i->vel.y >= 0.0f)
							i->vel.y *= -1.0f * coef_res;
					}

					else if (i->pos.x >= 0.515f) // 오른쪽 벽과 충돌
					{
						i->pos.x = 0.515f - i->radius;

						if (i->vel.x >= 0.0f)
							i->vel.x *= -1.0f * coef_res;
					}

					else if (i->pos.x <= -0.515f) // 왼쪽 벽과 충돌
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