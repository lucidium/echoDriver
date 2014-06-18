#pragma once

#define END_POINT -380.0

#include "ofMain.h"
#include "ofxTextSuite.h"


class echoThanksUI{
	public:
		echoThanksUI();
		~echoThanksUI();

		void update();
		void draw();

		void showUI();
		void hideUI();

		bool getActiveStatus();

	private:
		void createLine(string strLine, int fontSize, int width);
		void startTextSlide();
		void stopTextSlide();
		void resetTextSlide();

		ofVec2f position;
		ofVec2f textPivot;

		float targetTransparency;
		float interpolationSpeed;
		float currentTransparency;
		float offsetY;
		float offsetSpeed;

		bool isVisible;
		bool isShutdown;
		bool isActive;

		bool isSlideActive;

		ofFbo fboText;

		vector<ofxTextBlock*> lines;
};