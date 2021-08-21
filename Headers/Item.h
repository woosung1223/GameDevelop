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
					Item::pos.x >= bar.pos.x - bar.width / 2.0f && Item::pos.x <= bar.pos.x + bar.width / 2.0f)
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

		virtual  ImageObject* ImageForLaser() {
			// laser 자식 클래스만 사용함 
			return nullptr;
		}
		virtual int WhatType() = 0;
	};

	class Faster : public Item {
	public:
		Faster() {
			Item::img = new ImageObject;
			Item::img->init("Images/Faster.png", 255, 255, 255);
		}
		int WhatType() override {
			return 0;
		}
	};

	class Multiply : public Item {
	public:
		Multiply() {
			Item::img = new ImageObject;
			Item::img->init("Images/double.png", 255, 255, 255);
		}
		int WhatType() override {
			return 1;
		}
	};

	class BigBar : public Item {
	public:
		BigBar() {
			Item::img = new ImageObject;
			Item::img->init("Images/bigbar.png", 255, 255, 255);
		}
		int WhatType() override {
			return 2;
		}
	};

	class Laser : public Item {
	public:
		Laser() {
			Item::img = new ImageObject;
			Item::img->init("Images/Laser.png", 255, 255, 255);
		}
		int WhatType() override {
			return 3;
		}
		ImageObject* ImageForLaser() override {
			ImageObject* temp = new ImageObject;
			temp->init("Images/Pointer.png", 255, 255, 255);
			return temp;
		}
	};
	class ItemHandler {
	public:
		RandomNumberGenerator RN;

		 Item* GetItem() {
			// 아이템 종류를 랜덤으로 초기화해서 반환. 
			 Item* itemlist[4] = { new Multiply(), new Faster(), new BigBar(), new Laser() };
			 Item * temp = itemlist[RN.getInt(0, 3)];
			temp->reset(RN.getFloat(-0.5f, 0.5f));
			return temp;
		}
	};
}