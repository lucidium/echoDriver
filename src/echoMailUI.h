#pragma once

#define COLOR_BACK_ALPHA 25
#define COLOR_OUTLINE_ALPHA 100
#define COLOR_OUTLINE_HIGHLIGHT_ALPHA 200
#define COLOR_FILL_ALPHA 200
#define COLOR_FILL_HIGHLIGHT_ALPHA 255
#define COLOR_PADDED_ALPHA 100
#define COLOR_PADDED_OUTLINE_ALPHA 200

#include "ofMain.h"
#include "ofxUI.h"
//ofxhGui
#include "hEvents.h"
#include "hObject.h"
#include "hGui_all.h"
//Mail
#include "SMTPS.h"
#include "wininet.h"
#pragma comment(lib,"Wininet.lib")
//Animation
#include "echoRingAnimation.h"

extern struct UIopacity{
	float colorBack;
	float colorOutline;
	float colorOutlineHighlight;
	float colorFill;
	float colorFillHighlight;
	float colorPadded;
	float colorPaddedOutline;
};

class echoMailUI : public hObject{
	public:
		echoMailUI();
		~echoMailUI();

		void update();
		void draw();

		void showUI();
		void hideUI();

		bool getActiveStatus();

		void guiEvent(ofxUIEventArgs &e);

	private:
		void initializeValiables();
		void guiSetup();
		void setTransparency();
		void updateUIOpacity(bool isGoingToBeVisible);
		void setUIWidgetOpacity(string strWidget, bool isGoingToBeVisible);
		void setHUIOpacity(bool isGoingToBeVisible);

		bool isFieldsCorrect();
		bool isConnectedToInternet();

		int getSendStatus();

		ofxUICanvas *gui;
		hGui *hui;
		hPanel *mainPanel;
		hEvents *events;
		hTextBox *textBox1;
		hTextBox *textBox2;
		hTextArea *textArea1;
		UIopacity *uiOpacityControl;

		ofImage imgTip;
	
		bool isGuiInitialized;
		bool isVisible;
		bool isShutdown;
		bool isActive;

		string strSample;

		int errorCode;

		float targetTransparency;
		float interpolationSpeed;
		float currentTransparency;
		float hUItransparency;

		//MailThread
		ofxSMTPthread mailThread;

		//Animation
		echoRingAnimation *okAnimation;

};