#pragma once

#define CAROUSEL_PATH "gui/carousel"

#include "ofMain.h"
#include "echoCarousel.h"
#include "ofxMaskControl.h"

class echoHomeUI{
	public:
		echoHomeUI();
		~echoHomeUI();

		void update();
		void draw();

		void showUI();
		void hideUI();

		bool getActiveStatus();

	private:
		ofxCarousel *carousel;
		ofxMaskControl *mask;

		float targetTransparency;
		float interpolationSpeed;
		float currentTransparency;

		bool isVisible;
		bool isShutdown;
		bool isActive;

		ofVec2f position;
};