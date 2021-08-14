#pragma once
#include "Game2D.h"
#include "ImageObject.h"
#include "Block.h"
#include "Bar.h"

namespace jm {
	class Item {
	public:
		ImageObject* img = nullptr;
		vec2 pos = vec2{ 0.0f, 1.2f };

		int Function(const Bar& bar) {
			// 바에 아이템이 부딪혔는지 판단.
			if (img != nullptr) {
				if (Item::pos.y - bar.pos.y <= 0.0001f && Item::pos.y - bar.pos.y >= 0.0f &&
					Item::pos.x >= bar.pos.x - 0.05f && Item::pos.x <= bar.pos.x + 0.05f)
				{
					delete img;
					img = nullptr;
					return 1;
				}
				else if (pos.y < -1.5f)
				{
					delete img;
					img = nullptr;
					return 0;
				}
			}
			return -1;
			// 아이템을 먹었으면 1 반환, 못먹었으면 0 반환, 떨어지는 중이면 -1 반환 
		}

		void draw() {
			if (img != nullptr) {
				beginTransformation();
				translate(Item::pos);
				scale(0.1f, 0.1f);
				img->draw();
				endTransformation();
			}
		}

		void move(const float& dt)  {
			Item::pos.y -= 0.3f * dt;
		}
		void reset(const float& x) {
			Item::pos = vec2{x, 1.2f };
		}
	};

	class Faster : public Item {
	public:
		Faster() {
			Item::img = new ImageObject;
			Item::img->init("Images/Faster.png", 255, 255, 255);
		}
	};

	class Multiply : public Item {
	public:
		Multiply() {
			Item::img = new ImageObject;
			Item::img->init("Images/double.png", 255, 255, 255);
		}
	};
}