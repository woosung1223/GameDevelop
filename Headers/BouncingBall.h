#pragma once
#include "Game2D.h"
#include "ImageObject.h"
#include "Mainpage.h"
#include "Bar.h"
#include "Ball.h"
#include "Block.h"
#include "Item.h"
#include "RandomNumberGenerator.h"
#include "SoundEngine.h"
#include <vector>
#define EXIT 2
#define START 1
namespace jm
{
	class BouncingBall : public Game2D
	{
	public:

		ImageObject background;
		ImageObject* PointerImage = nullptr;
		Mainpage* mpg = nullptr;
		Bar* bar = nullptr;
		BlockHandler* blockhandler = nullptr;
		BallHandler* ballhandler = nullptr;
		ItemHandler* itemhandler = nullptr;
		Item* item = nullptr;
		float dt = 0.0f;
		float timeForItem = 0.0f;
		float PointerRadian = 90.0f; // Laser 아이템을 먹었을 때 Pointer 이미지의 각도를 조절
		int count_for_pointer = 0; // Laser 아이템효과를 위한 변수
		float dt_for_Laser = 0.0f;
		bool gameover = false;
		bool startclock[4] = { false, };
		bool type3_activated = false;
		BouncingBall()
			: Game2D("Bouncing Ball", 500, 960, false) // MUST initialize Game2D
		{
			SoundEngine_Singleton& sound_engine = *SoundEngine_Singleton::getInstance();
			sound_engine.createSound("background.mp3", "background_music", true);
			sound_engine.playSound("background_music");
			//background.init("images/Background_img.png");
			background.init("images/Background_img_3.jpg");
			mpg = new Mainpage;
			bar = new Bar;

			blockhandler = new BlockHandler;
			ballhandler = new BallHandler;
			itemhandler = new ItemHandler;
			item = itemhandler->GetItem();

			ballhandler->PushBall();

		}
		~BouncingBall() {
		}
		void update() override
		{
			// draw background
			beginTransformation();
			scale(2.0f, 2.0f);
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
			else if (gameover && !type3_activated) { // 공을 다 놓쳐서 끝난 경우. laser 아이템을 먹었으면 예외.
					ImageObject gameoverimg;
					ImageObject sentence;
					gameoverimg.init("images/gameover.png", 0, 0, 0);
					sentence.init("images/pressbutton.png", 0, 0, 0);
					beginTransformation();
					translate(0.0f, 0.5f);
					scale(0.3f, 0.3f);
					gameoverimg.draw();
					endTransformation();

					beginTransformation();
					translate(0.0f, 0.15f);
					scale(0.1f, 0.1f);
					sentence.draw();
					endTransformation();
			}
			else if (blockhandler->AllBlockDeleted()) { // 블록이 다 부서져서 게임이 끝난 경우
				ImageObject gameoverimg;
				gameoverimg.init("images/gameover2.png", 0, 0, 0);
				beginTransformation();
				translate(0.0f, 0.4f);
				scale(0.6f, 0.6f);
				gameoverimg.draw();
				endTransformation();
			}
			// game start
			else {
				bar->draw();
				if (isKeyPressed(GLFW_KEY_A)) bar->update(-getTimeStep());
				if (isKeyPressed(GLFW_KEY_D))  bar->update(getTimeStep());
				if (isKeyPressed(GLFW_KEY_Q) && PointerImage != nullptr && PointerRadian < 150.0f) PointerRadian += 1.0f;
				if (isKeyPressed(GLFW_KEY_E) && PointerImage != nullptr && PointerRadian > 30.0f) PointerRadian -= 1.0f;
				// ball

				ballhandler->draw();
				if (PointerImage != nullptr)
				{
					beginTransformation();
					translate(0.0f, 0.1f);
					translate(bar->pos);
					scale(0.2f, 0.2f);
					translate(0.0f, -0.3f);
					rotate(PointerRadian - 90.0f);
					translate(0.0f, 0.3f);
					PointerImage->draw();
					endTransformation();
				}
				ballhandler->update(getTimeStep() * 0.1f);
				ballhandler->PopBall(); // 삭제될 ball이 있는지 검사 
				ballhandler->CheckCollision(*bar);
				if (ballhandler->GameOver()) {
					// gameover
					gameover = true;
				}
				// block
				blockhandler->CheckCollision(*ballhandler);
				blockhandler->draw();

				//make item(random)
				// item
				if (item == nullptr && timeForItem >= 20.0f) { 
					// 10.0f 가 넘어가면 아이템이 젠됨. 
					item = itemhandler->GetItem(); 
					timeForItem = 0.0f;
				}
				if (item != nullptr) {
					item->move(getTimeStep());
					item->draw();
					int flag = item->Function(*bar);
					int what_type = item->WhatType(); // 무슨 아이템인지 판단
					// 0이면 Faster, 1이면 Multiply, 2이면 Bigbar.
					if (flag == 1) {
						dt = 0.0f;
						startclock[what_type] = true;
						if (what_type == 0)
							ballhandler->Faster();

						else if (what_type == 1) {
							ballhandler->Multiply();
							delete item;
							item = nullptr;
							}
						else if (what_type == 3) {
							type3_activated = true;
						}
					}
					else if (flag == 0) // 아이템을 못먹었을 때 
					{
						delete item;
						item = nullptr;
					}
					if (what_type == 0 && startclock[0]) { // Faster 아이템이고, startclock이 true라면,
						dt += getTimeStep();
						if (dt > 10.0f) {
							ballhandler->Slower();
							startclock[0] = false;
							delete item;
							item = nullptr;
						}
					}
					else if (what_type == 2 && startclock[2]) // BigBar 아이템이고, startclock이 true라면, 
					{
						bar->width += 0.01f * getTimeStep();
						if (bar->width > 0.3f) {
							bar->width = 0.1f;
							delete item;
							item = nullptr;
							startclock[2] = false;
						}
					}

				}
				if (type3_activated) {
					if(PointerImage == nullptr)
						PointerImage = item->ImageForLaser();

					if (isKeyPressedAndReleased(GLFW_KEY_SPACE)) {
						dt_for_Laser = 0.0f;
						count_for_pointer = 1;
					}
					if (count_for_pointer) {
						if (count_for_pointer >= 4) {
							count_for_pointer = 0;
							delete item;
							delete PointerImage;
							PointerImage = nullptr;
							item = nullptr;
							type3_activated = false;
						}
						else if(dt_for_Laser >= 0.3f){
								Ball* temp = new Ball;
								temp->pos = bar->pos + vec2(0.0f, 0.04f);
								temp->vel = vec2(0.0f, 1.5f);
								float tempx = temp->vel.x;
								float tempy = temp->vel.y;
								temp->vel.x = tempx * cos((PointerRadian * PI / 180) - 1.5708) - tempy * sin((PointerRadian * PI / 180) - 1.5708);
								temp->vel.y = tempx * sin((PointerRadian * PI / 180) - 1.5708) + tempy * cos((PointerRadian * PI / 180) - 1.5708);
								/*std::cout << temp->vel.x << std::endl;
								std::cout << temp->vel.y << std::endl;*/
								ballhandler->balls.push_back(temp);
								ballhandler->ballnum++;
								count_for_pointer++;
								dt_for_Laser = 0.0f;
								gameover = false;
						}
					}
				}
				timeForItem += getTimeStep();
				dt_for_Laser += getTimeStep();
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