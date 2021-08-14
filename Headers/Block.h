#pragma once
#include "Game2D.h"
#include "RandomNumberGenerator.h"
#include "Ball.h"
#include <vector>
#define EPSILON 0.000001
namespace jm {
	class Block {
	public:
		vec2 pos = vec2(0.0f, 0.0f);
		int life = 3;
		bool deleting = false;
		float for_fade = 0.0f;
		Block(const float& x, const float& y) {
			/*RandomNumberGenerator RN;
			pos = { RN.getFloat(-0.5f, 0.5f), RN.getFloat(0.7f, 1.0f) };*/
			pos = vec2(x, y);
		}
		void draw() {
			beginTransformation();
			translate(pos);
			switch (life) {
			case 1:
				drawFilledBox(Colors::life1, 0.03f, 0.03f);
				break;
			case 2:
				drawFilledBox(Colors::life2, 0.03f, 0.03f);
				break;
			case 3:
				drawFilledBox(Colors::life3, 0.03f, 0.03f);
				break;
			}
			drawWiredBox(Colors::white, 0.03f, 0.03f);
			endTransformation();
		}

		bool FadeOut() {
			this->deleting = true; // 삭제되는 요소이니 공이 튕겨나가는 걸 무효화
			const float alpha = for_fade;
			const RGB blended_color = {
				Colors::life1.r * alpha + 1.0f * (1.0f - alpha),
				Colors::life1.g * alpha + 1.0f * (1.0f - alpha),
				Colors::life1.b * alpha + 1.0f * (1.0f - alpha) };

			beginTransformation();
			translate(pos);
			drawFilledBox(blended_color, 0.03f, 0.03f);
			endTransformation();
			for_fade += 0.03f;
			if (alpha >= 1.0f) {
				for_fade = 0.0f;
				return true; // 삭제되어야 함. 
			}
			else
				return false;
		}
	};

	class BlockManager {
	public:
		vector <Block*> Blocks;
		const float coef_res = 1.0f;
		BlockManager() {
			/*
				34 * 30 크기의 블록들을 생성.
			*/
			for (int i = 0; i < 34; i++) { // 세로
				for (int j = 0; j < 30; j++) // 가로
						Blocks.push_back(new Block(-0.5f + 0.03 * i, 0.0f + 0.03f * j));
			}
		}
		void draw() {
			for (auto& i : Blocks) {
				if (i != nullptr) {
					if (i->life == 0) {
						bool temp = i->FadeOut();
						if (temp) {
							delete i;
							i = nullptr;
						}
					}
					else
						i->draw();
				}
			}
		}

		void CheckCollision(Ball& ball) {
			for (auto& block : Blocks) {
				if (block != nullptr && !block->deleting) { // 삭제된 블록이 아니거나 삭제될 예정인 블록이 아닌 경우에만 
					vec2 left[2] = { vec2(block->pos.x - 0.02f, block->pos.y - 0.015f), vec2(block->pos.x - 0.015f, block->pos.y + 0.015f) };
					vec2 right[2] = { vec2(block->pos.x + 0.015f, block->pos.y - 0.015f),vec2(block->pos.x + 0.02f, block->pos.y + 0.015f)};
					vec2 down[2] = { vec2(block->pos.x - 0.015f, block->pos.y - 0.02f), vec2(block->pos.x + 0.015f, block->pos.y - 0.015f)};
					vec2 up[2] = { vec2(block->pos.x - 0.015f, block->pos.y + 0.015f), vec2(block->pos.x + 0.015f, block->pos.y + 0.02f)};
					vec2 inside[2] = { vec2(block->pos.x - 0.0075f, block->pos.y - 0.0075f), vec2(block->pos.x + 0.0075f, block->pos.y + 0.0075f) };
					// boundary check를 위한 space 생성

					// 모서리방향 4방향 부딪혔을 때 -> 블록 안에 들어왔는지.
					// 왼쪽 벽
					if (ball.pos + vec2(ball.radius, 0.0f) >= left[0] && ball.pos + vec2(ball.radius, 0.0f) <= left[1]) {
						block->life -= 1;

						std::cout << "left" << std::endl;
						if (ball.vel.x >= 0.0f)
							ball.vel.x *= -1.0f * coef_res;
					}

					// 오른쪽 벽
					else if (ball.pos - vec2(ball.radius, 0.0f) >= right[0] && ball.pos - vec2(ball.radius, 0.0f) <= right[1]) {
						block->life -= 1;

						std::cout << "right" << std::endl;
						if (ball.vel.x <= 0.0f)
							ball.vel.x *= -1.0f * coef_res;
					}

					// 아래쪽 벽
					else if (ball.pos + vec2(0.0f, ball.radius) >= down[0] && ball.pos + vec2(0.0f, ball.radius) <= down[1]) {
						block->life -= 1;

						std::cout << "down" << std::endl;
						if (ball.vel.y >= 0.0f)
							ball.vel.y *= -1.0f * coef_res;
					}

					// 위쪽 벽
					else if (ball.pos - vec2(0.0f, ball.radius) >= up[0] && ball.pos - vec2(0.0f, ball.radius) <= up[1]) {
						block->life -= 1;

						std::cout << "up" << std::endl;
						if (ball.vel.y <= 0.0f)
							ball.vel.y *= -1.0f * coef_res;
					}

					// 우측 상단 모서리
					else if (ball.pos - vec2(ball.radius, ball.radius) >= inside[0] && ball.pos - vec2(ball.radius, ball.radius) <= inside[1]) {
						block->life -= 1;

						ball.pos += vec2(ball.radius, ball.radius);
						std::cout << "right up" << std::endl;
						if (ball.vel.x<= 0.0f)
							ball.vel *= -1.0f * coef_res;
					}
					// 좌측 상단 모서리
					else if (ball.pos + vec2(ball.radius, -ball.radius) >= inside[0] && ball.pos + vec2(ball.radius, -ball.radius) <= inside[1]) {
						block->life -= 1;

						ball.pos += vec2(-ball.radius, ball.radius);
						std::cout << "left up" << std::endl;
						if (ball.vel.x >= 0.0f)
							ball.vel *= -1.0f * coef_res;
					}

					// 우측 하단 모서리
					else if (ball.pos + vec2(-ball.radius, ball.radius) >= inside[0] && ball.pos + vec2(-ball.radius, ball.radius) <= inside[1]) {
						block->life -= 1;

						ball.pos += vec2(ball.radius, -ball.radius);
						std::cout << "right down" << std::endl;
						if (ball.vel.x <= 0.0f)
							ball.vel *= -1.0f * coef_res;
					}
					// 좌측 하단 모서리
					else if (ball.pos + vec2(ball.radius, ball.radius) >= inside[0] && ball.pos + vec2(ball.radius, ball.radius) <= inside[1]) {
						block->life -= 1;

						ball.pos += vec2(-ball.radius, -ball.radius);
						std::cout << "left down" << std::endl;
						if (ball.vel.x >= 0.0f)
							ball.vel *= -1.0f * coef_res;
					}
				}
			}
		}
	};
}