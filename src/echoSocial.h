#pragma once

#include "ofMain.h"
#include "echoButtons.h"

class echoSocialUI{
	public:
		echoSocialUI();
		~echoSocialUI();

		void update();
		void draw();

		void showUI();
		void hideUI();

		void mouseReleased(int x, int y, int button);
		bool getActiveStatus();

	private:
		void createButtons();
		void buttonEvents(string strId);

		echoButtons *buttons;

		ofVec2f position;

		int offsetX;
		int offsetY;

		float targetTransparency;
		float interpolationSpeed;
		float currentTransparency;

		bool isVisible;
		bool isShutdown;
		bool isActive;
};