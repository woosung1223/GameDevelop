#pragma once
#include "Game2D.h"
#include "ImageObject.h"
#include "Mainpage.h"
#include "Bar.h"
#include "Ball.h"
#include "Block.h"

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
		Ball* ball = nullptr;
		BlockManager* blockmanager = nullptr;
		float dt = 0.0f;
		BouncingBall()
			: Game2D("Bouncing Ball", 500, 960, false) // MUST initialize Game2D
		{
			background.init("images/Background_img.jpg");
			mpg = new Mainpage;
			bar = new Bar;
			ball = new Ball;
			blockmanager = new BlockManager;
		}

		void update() override
		{
			// draw background
			beginTransformation();
			scale(1.0f, 2.0f);
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
			else {
				bar->draw();
				if (isKeyPressed(GLFW_KEY_A)) bar->update(-getTimeStep());
				if (isKeyPressed(GLFW_KEY_D))  bar->update(getTimeStep());

				ball->draw();
				ball->update(getTimeStep() * 0.1f);
				ball->CheckCollision(*bar);

				blockmanager->CheckCollision(*ball);
				blockmanager->draw();
			}
			dt += getTimeStep();
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