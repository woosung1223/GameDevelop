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
			this->deleting = true; // �����Ǵ� ����̴� ���� ƨ�ܳ����� �� ��ȿȭ
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
				return true; // �����Ǿ�� ��. 
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
				34 * 30 ũ���� ��ϵ��� ����.
			*/
			for (int i = 0; i < 34; i++) { // ����
				for (int j = 0; j < 30; j++) // ����
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
				if (block != nullptr && !block->deleting) { // ������ ����� �ƴϰų� ������ ������ ����� �ƴ� ��쿡�� 
					vec2 left[2] = { vec2(block->pos.x - 0.02f, block->pos.y - 0.015f), vec2(block->pos.x - 0.015f, block->pos.y + 0.015f) };
					vec2 right[2] = { vec2(block->pos.x + 0.015f, block->pos.y - 0.015f),vec2(block->pos.x + 0.02f, block->pos.y + 0.015f)};
					vec2 down[2] = { vec2(block->pos.x - 0.015f, block->pos.y - 0.02f), vec2(block->pos.x + 0.015f, block->pos.y - 0.015f)};
					vec2 up[2] = { vec2(block->pos.x - 0.015f, block->pos.y + 0.015f), vec2(block->pos.x + 0.015f, block->pos.y + 0.02f)};
					vec2 inside[2] = { vec2(block->pos.x - 0.0075f, block->pos.y - 0.0075f), vec2(block->pos.x + 0.0075f, block->pos.y + 0.0075f) };
					// boundary check�� ���� space ����

					// �𼭸����� 4���� �ε����� �� -> ��� �ȿ� ���Դ���.
					// ���� ��
					if (ball.pos + vec2(ball.radius, 0.0f) >= left[0] && ball.pos + vec2(ball.radius, 0.0f) <= left[1]) {
						block->life -= 1;

						std::cout << "left" << std::endl;
						if (ball.vel.x >= 0.0f)
							ball.vel.x *= -1.0f * coef_res;
					}

					// ������ ��
					else if (ball.pos - vec2(ball.radius, 0.0f) >= right[0] && ball.pos - vec2(ball.radius, 0.0f) <= right[1]) {
						block->life -= 1;

						std::cout << "right" << std::endl;
						if (ball.vel.x <= 0.0f)
							ball.vel.x *= -1.0f * coef_res;
					}

					// �Ʒ��� ��
					else if (ball.pos + vec2(0.0f, ball.radius) >= down[0] && ball.pos + vec2(0.0f, ball.radius) <= down[1]) {
						block->life -= 1;

						std::cout << "down" << std::endl;
						if (ball.vel.y >= 0.0f)
							ball.vel.y *= -1.0f * coef_res;
					}

					// ���� ��
					else if (ball.pos - vec2(0.0f, ball.radius) >= up[0] && ball.pos - vec2(0.0f, ball.radius) <= up[1]) {
						block->life -= 1;

						std::cout << "up" << std::endl;
						if (ball.vel.y <= 0.0f)
							ball.vel.y *= -1.0f * coef_res;
					}

					// ���� ��� �𼭸�
					else if (ball.pos - vec2(ball.radius, ball.radius) >= inside[0] && ball.pos - vec2(ball.radius, ball.radius) <= inside[1]) {
						block->life -= 1;

						ball.pos += vec2(ball.radius, ball.radius);
						std::cout << "right up" << std::endl;
						if (ball.vel.x<= 0.0f)
							ball.vel *= -1.0f * coef_res;
					}
					// ���� ��� �𼭸�
					else if (ball.pos + vec2(ball.radius, -ball.radius) >= inside[0] && ball.pos + vec2(ball.radius, -ball.radius) <= inside[1]) {
						block->life -= 1;

						ball.pos += vec2(-ball.radius, ball.radius);
						std::cout << "left up" << std::endl;
						if (ball.vel.x >= 0.0f)
							ball.vel *= -1.0f * coef_res;
					}

					// ���� �ϴ� �𼭸�
					else if (ball.pos + vec2(-ball.radius, ball.radius) >= inside[0] && ball.pos + vec2(-ball.radius, ball.radius) <= inside[1]) {
						block->life -= 1;

						ball.pos += vec2(ball.radius, -ball.radius);
						std::cout << "right down" << std::endl;
						if (ball.vel.x <= 0.0f)
							ball.vel *= -1.0f * coef_res;
					}
					// ���� �ϴ� �𼭸�
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