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
		bool rigid = false; // true�� ��� �μ����� �ʴ� ���� 
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
			this->deleting = true; // �����Ǵ� ����̴� ���� ƨ�ܳ����� �� ��ȿȭ
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
				return true; // �����Ǿ�� ��. 
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
				34 * 30 ũ���� ��ϵ��� ����.
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
			for (int i = 0; i < 34; i++) { // ����
				for (int j = -3; j < 28; j++) // ����
				{
					Block* blk = new Block(-0.5f + 0.03 * i, 0.0f + 0.03f * j);
					//�������� ȸ����� ����
					//int RandomNumber_j = RN.getInt(-3, 27);
					//int RandomNumber_i = RN.getInt(0, 33); // �� ���� ��� ���� 
					//if (j == RandomNumber_j || i == RandomNumber_i)
					//	blk->rigid = true;
					
					// ���� ���� ��
					if (i == 0 || i == 29 || ((j == -3 || j == 27) && i % 2 == 0))
						blk->rigid = true;
					if(i < 30)
						blocks.push_back(blk);
				}
			}
		}
		void CheckCollision(BallHandler& ballhandler) {
			for (auto& block : Blocks) {
				if (block != nullptr && !block->deleting) { // ������ ����� �ƴϰų� ������ ������ ����� �ƴ� ��쿡�� 
					for (auto& ball : ballhandler.balls) {
						if (ball != nullptr && (block->pos - ball->pos).getMagnitude() < 0.1f) {
							vec2 left[2] = { vec2(block->pos.x - 0.02f, block->pos.y - 0.015f), vec2(block->pos.x - 0.015f, block->pos.y + 0.015f) };
							vec2 right[2] = { vec2(block->pos.x + 0.015f, block->pos.y - 0.015f),vec2(block->pos.x + 0.02f, block->pos.y + 0.015f) };
							vec2 down[2] = { vec2(block->pos.x - 0.015f, block->pos.y - 0.02f), vec2(block->pos.x + 0.015f, block->pos.y - 0.015f) };
							vec2 up[2] = { vec2(block->pos.x - 0.015f, block->pos.y + 0.015f), vec2(block->pos.x + 0.015f, block->pos.y + 0.02f) };
							vec2 inside[2] = { vec2(block->pos.x - 0.0075f, block->pos.y - 0.0075f), vec2(block->pos.x + 0.0075f, block->pos.y + 0.0075f) };
							// boundary check�� ���� space ����

							// �𼭸����� 4���� �ε����� �� -> ��� �ȿ� ���Դ���.
							// ���� ��
							// �𼭸��� �浹 �� ball->vel�� 1������ ȸ����Ŵ. �����ϰ� �Դٰ����ϴ� ���� �����ϱ� ����. 
							if (ball->pos + vec2(ball->radius, 0.0f) >= left[0] && ball->pos + vec2(ball->radius, 0.0f) <= left[1]) {
								if(!block->rigid)
									block->life -= 1;

								if (ball->vel.x >= 0.0f)
									ball->vel.x *= -1.0f * coef_res;
							}

							// ������ ��
							else if (ball->pos - vec2(ball->radius, 0.0f) >= right[0] && ball->pos - vec2(ball->radius, 0.0f) <= right[1]) {
								if (!block->rigid)
									block->life -= 1;

								if (ball->vel.x <= 0.0f)
									ball->vel.x *= -1.0f * coef_res;
							}

							// �Ʒ��� ��
							else if (ball->pos + vec2(0.0f, ball->radius) >= down[0] && ball->pos + vec2(0.0f, ball->radius) <= down[1]) {
								if (!block->rigid)
									block->life -= 1;

								if (ball->vel.y >= 0.0f)
									ball->vel.y *= -1.0f * coef_res;
							}

							// ���� ��
							else if (ball->pos - vec2(0.0f, ball->radius) >= up[0] && ball->pos - vec2(0.0f, ball->radius) <= up[1]) {
								if (!block->rigid)
									block->life -= 1;

								if (ball->vel.y <= 0.0f)
									ball->vel.y *= -1.0f * coef_res;
							}

							// ���� ��� �𼭸�
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
							// ���� ��� �𼭸�
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

							// ���� �ϴ� �𼭸�
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
							// ���� �ϴ� �𼭸�
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