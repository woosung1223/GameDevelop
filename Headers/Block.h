#pragma once
#include "Game2D.h"
#include "RandomNumberGenerator.h"
#include "ball.h"
#include <vector>
#include <cmath>
#define EPSILON 0.000001
#define PI 3.141592
namespace jm {
	class Block {
	public:
		vec2 pos = vec2(0.0f, 0.0f);
		int life = 3;
		bool deleting = false;
		bool rigid = false; // true인 경우 부서지지 않는 벽돌 
		float for_fade = 0.0f;
		Block(const float& x, const float& y) {
			/*RandomNumberGenerator RN;
			pos = { RN.getFloat(-0.5f, 0.5f), RN.getFloat(0.7f, 1.0f) };*/
			pos = vec2(x, y);
		}
		void draw() {
			if (rigid) {
				beginTransformation();
				translate(pos);
				drawFilledBox(Colors::gray, 0.03f, 0.03f);
				drawWiredBox(Colors::white, 0.03f, 0.03f);
				endTransformation();
			}
			else {
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
		}

		bool FadeOut() {
			this->deleting = true; // 삭제되는 요소이니 공이 튕겨나가는 걸 무효화
			const float alpha = for_fade;
			const RGB blended_color = {
				Colors::life2.r * alpha + 1.0f * (1.0f - alpha),
				Colors::life2.g * alpha + 1.0f * (1.0f - alpha),
				Colors::life2.b * alpha + 1.0f * (1.0f - alpha) };

			beginTransformation();
			translate(pos);
			drawFilledBox(blended_color, 0.03f, 0.03f);
			endTransformation();
			for_fade += 0.05f;
			if (alpha >= 1.0f) {
				for_fade = 0.0f;
				return true; // 삭제되어야 함. 
			}
			else
				return false;
		}
	};
	
	class BlockHandler {
	public:
		vector <Block*> Blocks;
		const float coef_res = 1.0f;
		RandomNumberGenerator RN;
		BlockHandler() {
			/*
				34 * 30 크기의 블록들을 생성.
			*/
			BlockHandler::LoadMap(Blocks);
		}
		bool AllBlockDeleted() {
			bool tf = true;
			for (const auto& i : Blocks)
				if (i != nullptr && !i ->rigid) tf = false;
			return tf;
		}
		void draw() {
			for (auto& i : Blocks) {
				if (i != nullptr) {
					if (i->life <= 0) {
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
		void LoadMap(std::vector<Block*>& blocks) {
			for (int i = 0; i < 34; i++) { // 가로
				for (int j = -3; j < 28; j++) // 세로
				{
					Block* blk = new Block(-0.5f + 0.03 * i, 0.0f + 0.03f * j);
					//랜덤으로 회색블록 생성
					//int RandomNumber_j = RN.getInt(-3, 27);
					//int RandomNumber_i = RN.getInt(0, 33); // 더 많은 블록 생성 
					//if (j == RandomNumber_j || i == RandomNumber_i)
					//	blk->rigid = true;
					
					// 내가 만든 맵
					if (i == 0 || i == 29 || ((j == -3 || j == 27) && i % 2 == 0))
						blk->rigid = true;
					if(i < 30)
						blocks.push_back(blk);
				}
			}
		}
		void CheckCollision(BallHandler& ballhandler) {
			for (auto& block : Blocks) {
				if (block != nullptr && !block->deleting) { // 삭제된 블록이 아니거나 삭제될 예정인 블록이 아닌 경우에만 
					for (auto& ball : ballhandler.balls) {
						if (ball != nullptr && (block->pos - ball->pos).getMagnitude() < 0.1f) {
							vec2 left[2] = { vec2(block->pos.x - 0.02f, block->pos.y - 0.015f), vec2(block->pos.x - 0.015f, block->pos.y + 0.015f) };
							vec2 right[2] = { vec2(block->pos.x + 0.015f, block->pos.y - 0.015f),vec2(block->pos.x + 0.02f, block->pos.y + 0.015f) };
							vec2 down[2] = { vec2(block->pos.x - 0.015f, block->pos.y - 0.02f), vec2(block->pos.x + 0.015f, block->pos.y - 0.015f) };
							vec2 up[2] = { vec2(block->pos.x - 0.015f, block->pos.y + 0.015f), vec2(block->pos.x + 0.015f, block->pos.y + 0.02f) };
							vec2 inside[2] = { vec2(block->pos.x - 0.0075f, block->pos.y - 0.0075f), vec2(block->pos.x + 0.0075f, block->pos.y + 0.0075f) };
							// boundary check를 위한 space 생성

							// 모서리방향 4방향 부딪혔을 때 -> 블록 안에 들어왔는지.
							// 왼쪽 벽
							// 모서리에 충돌 시 ball->vel을 1도정도 회전시킴. 무한하게 왔다갔다하는 것을 방지하기 위함. 
							if (ball->pos + vec2(ball->radius, 0.0f) >= left[0] && ball->pos + vec2(ball->radius, 0.0f) <= left[1]) {
								if(!block->rigid)
									block->life -= 1;

								if (ball->vel.x >= 0.0f)
									ball->vel.x *= -1.0f * coef_res;
							}

							// 오른쪽 벽
							else if (ball->pos - vec2(ball->radius, 0.0f) >= right[0] && ball->pos - vec2(ball->radius, 0.0f) <= right[1]) {
								if (!block->rigid)
									block->life -= 1;

								if (ball->vel.x <= 0.0f)
									ball->vel.x *= -1.0f * coef_res;
							}

							// 아래쪽 벽
							else if (ball->pos + vec2(0.0f, ball->radius) >= down[0] && ball->pos + vec2(0.0f, ball->radius) <= down[1]) {
								if (!block->rigid)
									block->life -= 1;

								if (ball->vel.y >= 0.0f)
									ball->vel.y *= -1.0f * coef_res;
							}

							// 위쪽 벽
							else if (ball->pos - vec2(0.0f, ball->radius) >= up[0] && ball->pos - vec2(0.0f, ball->radius) <= up[1]) {
								if (!block->rigid)
									block->life -= 1;

								if (ball->vel.y <= 0.0f)
									ball->vel.y *= -1.0f * coef_res;
							}

							// 우측 상단 모서리
							else if (ball->pos - vec2(ball->radius, ball->radius) >= inside[0] && ball->pos - vec2(ball->radius, ball->radius) <= inside[1]) {
								if (!block->rigid)
									block->life -= 1;

								ball->pos += vec2(ball->radius, ball->radius);
								if (ball->vel.x <= 0.0f) {
									float tempx = ball->vel.x;
									float tempy = ball->vel.y;
									ball->vel.x = tempx * cos(PI / 180.0f) - tempy * sin(PI / 180.0f);
									ball->vel.y = tempx * sin(PI / 180.0f) + tempy * cos(PI / 180.0f);
									ball->vel *= -1.0f * coef_res;
								}
							}
							// 좌측 상단 모서리
							else if (ball->pos + vec2(ball->radius, -ball->radius) >= inside[0] && ball->pos + vec2(ball->radius, -ball->radius) <= inside[1]) {
								if (!block->rigid)
									block->life -= 1;

								ball->pos += vec2(-ball->radius, ball->radius);
								if (ball->vel.x >= 0.0f) {
									float tempx = ball->vel.x;
									float tempy = ball->vel.y;
									ball->vel.x = tempx * cos(PI / 180.0f) - tempy * sin(PI / 180.0f);
									ball->vel.y = tempx * sin(PI / 180.0f) + tempy * cos(PI / 180.0f);
									ball->vel *= -1.0f * coef_res;
								}
							}

							// 우측 하단 모서리
							else if (ball->pos + vec2(-ball->radius, ball->radius) >= inside[0] && ball->pos + vec2(-ball->radius, ball->radius) <= inside[1]) {
								if (!block->rigid)
									block->life -= 1;

								ball->pos += vec2(ball->radius, -ball->radius);
								if (ball->vel.x <= 0.0f) {
									float tempx = ball->vel.x;
									float tempy = ball->vel.y;
									ball->vel.x = tempx * cos(PI / 180.0f) - tempy * sin(PI / 180.0f);
									ball->vel.y = tempx * sin(PI / 180.0f) + tempy * cos(PI / 180.0f);
									ball->vel *= -1.0f * coef_res;
								}
							}
							// 좌측 하단 모서리
							else if (ball->pos + vec2(ball->radius, ball->radius) >= inside[0] && ball->pos + vec2(ball->radius, ball->radius) <= inside[1]) {
								if (!block->rigid)
									block->life -= 1;

								ball->pos += vec2(-ball->radius, -ball->radius);
								if (ball->vel.x >= 0.0f) {
									float tempx = ball->vel.x;
									float tempy = ball->vel.y;
									ball->vel.x = tempx * cos(PI / 180.0f) - tempy * sin(PI / 180.0f);
									ball->vel.y = tempx * sin(PI / 180.0f) +tempy * cos(PI / 180.0f);
									ball->vel *= -1.0f * coef_res;
								}
							}
						}
					}
				}
			}
		}
	};
}