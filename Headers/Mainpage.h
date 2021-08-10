#pragma once
#include "ImageObject.h"
#include "Game2D.h"

namespace jm {
	class Mainpage {
	public:
		ImageObject sentence;
		ImageObject startbutton;
		ImageObject exitbutton;
		Mainpage() {
			sentence.init("images/sentence.png", 0, 0, 0);
			startbutton.init("images/GameStart.png", 0, 0, 0);
			exitbutton.init("images/exit.png", 0, 0, 0);
		}
		void draw() {
			// draw game title 
			beginTransformation();
			translate(0.0f, 0.6f);
			scale(0.3f, 0.3f);
			sentence.draw();
			endTransformation();
			// draw start button
			beginTransformation();
			translate(0.3f, -0.4f);
			scale(0.15f, 0.15f);
			startbutton.draw();
			endTransformation();
			// draw exit button
			beginTransformation();
			translate(0.3f, -0.55f);
			scale(0.15f, 0.15f);
			exitbutton.draw();
			endTransformation();
		}
	};
}