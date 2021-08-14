#pragma once
#include "Game2D.h"
#include "ImageObject.h"
#include "Mainpage.h"
#include "Bar.h"
#include "Ball.h"
#include "Block.h"
#include "Item.h"
#include "RandomNumberGenerator.h"
#include <vector>
#define EXIT 2
#define START 1

namespace jm
{
	class BouncingBall : public Game2D
	{
	public:

		ImageObject background;
		Mainpage* mpg = nullptr;
		Bar* bar = nullptr;
		vector<Ball*> ball;
		BlockManager* blockmanager = nullptr;
		RandomNumberGenerator RN;
		bool itemmaked = false;
		float dt = 0.0f;
		int clock = 0;
		bool timerStarted = false;
		Item* itemtype[2] = { new Faster(), new Multiply() };
		Item* item = nullptr;
		BouncingBall()
			: Game2D("Bouncing Ball", 500, 960, false) // MUST initialize Game2D
		{
			//background.init("images/Background_img.png");
			background.init("images/Background_img_2.jpg");
			mpg = new Mainpage;
			bar = new Bar;
			ball.push_back(new Ball);
			blockmanager = new BlockManager;
		}

		void update() override
		{
			// draw background
			beginTransformation();
			scale(1.5f, 2.0f);
			background.draw();
			endTransformation();

			// mainpage (START or EXIT)
			if (mpg != nullptr)
			{
				mpg->draw();
				switch (IsButtonClicked_Mainpage()) {
				case EXIT:
					exit(0);
					break;
				case START:
					delete mpg;
					mpg = nullptr;
					break;
				}
			}
			// game start
			else {
				bar->draw();
				if (isKeyPressed(GLFW_KEY_A)) bar->update(-getTimeStep());
				if (isKeyPressed(GLFW_KEY_D))  bar->update(getTimeStep());

				// ball
				for (Ball*& i : ball) {
					i->draw();
					i->update(getTimeStep() * 0.1f);
					i->CheckCollision(*bar);
				}

				// block
				for (Ball*& i : ball)
					blockmanager->CheckCollision(*i);
				blockmanager->draw();


				//make item(random)
				// item
				//int randomtime = RN.getInt(0, 10000);
				//if (randomtime > 9000 && item == nullptr) {
				//	std::cout << "a" << std::endl;
				//	itemmaked = true;
				//}

				//if (itemmaked && item == nullptr) {
				//	std::cout << "b" << std::endl;
				//	item = itemtype[RN.getInt(0, 1)];
				//	item->reset(RN.getFloat(0.0f, 0.5f));
				//	/*std::cout << item->pos << std::endl;*/
				//}
				//if (item != nullptr)
				//{
				//	item->draw();
				//	item->move(getTimeStep());
				//	int status = item->Function(*bar);
				//	if (status == 1) { // 아이템을 먹었다면 
				//		for (Ball*& i : ball)
				//			i->vel *= 2.0f;
				//		dt = 0.0f; // 타이머 시작 
				//		timerStarted = true;
				//		clock += 5;
				//	}

				//	else if (status == 0) // 아이템 먹지 못했을 때 
				//	{
				//		itemmaked = false;
				//		item = nullptr;
				//	}

				//	if (timerStarted) {
				//		if (dt > clock && clock > 0) {
				//			clock -= 5;
				//			dt = 0.0f;
				//			for (Ball*& i : ball)
				//				i->vel /= 2.0f;

				//			if (clock == 0) {
				//				//std::cout << "a";
				//				timerStarted = false;
				//				itemmaked = false;
				//				item = nullptr;
				//			}
				//		}
				dt += getTimeStep();
			}
		}
		int IsButtonClicked_Mainpage() {
			vec2 cursorpos = getCursorPos(true);

			vec2 StartButtonRange[2] = { vec2(0.13f, -0.5f), vec2(0.48f, -0.38f) };
			vec2 ExitButtonRange[2] = { vec2(0.25f, -0.6f), vec2(0.37f, -0.53f) };

			if (isMouseButtonPressedAndReleased(GLFW_MOUSE_BUTTON_1)) {
				if (cursorpos >= StartButtonRange[0] && cursorpos <= StartButtonRange[1])
				{
					return START;
				}
				else if (cursorpos >= ExitButtonRange[0] && cursorpos <= ExitButtonRange[1])
					return EXIT;
			}
			return -1; // 클릭이 없거나 다른 곳을 클릭한 경우
		}
	};
}