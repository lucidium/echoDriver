#pragma once

#include "ofMain.h"

class echoButton{
	public:
		echoButton(ofImage *imgIdle, ofImage *imgHover, string strId, ofVec2f pos);
		~echoButton();

		void mouseHoverCheck(ofVec2f mousePos);

		ofImage *imgIdle;
		ofImage *imgHover;
		string strId;
		ofVec2f pos;
		bool isPointerOn;
		bool isClicked;
		float hoverOpacity;
};

class echoButtons{
	public:
		echoButtons(float lerpSpeed);
		~echoButtons();

		void update();
		void draw();

		void createButton(ofImage *imgIdle, ofImage *imgHover, string strId, ofVec2f pos);
		void clearButtonInfo();

		void setOpacity(float value);
		void hoverFadeOut();

		string getClickedButtonId();
		string getHoverButtonId();

	private:
		vector<echoButton*> buttons;
		int clickedButton;
		float lerpSpeed;
		float opacity;
		bool isButtonClicked;
		bool isButtonHover;
		string strHoverButtonId;
		string strPressedButtonId;
};