#pragma once

#define ECHO_VERSION_MAJOR 1
#define ECHO_VERSION_MINOR 1
#define ECHO_VERSION_PATCH 1

#define BACK_PATH "gui/backgrounds"
#define FRAMES_PATH "gui/frames"

#include "ofMain.h"
#include "echoCore.h"
#include "echoMouseDriver.h"
#include "echoUI.h"
#include "echoCarousel.h"
#include "echoBackChanger.h"
#include "echoButtons.h"
#include "echoMailUI.h"
#include "echoSocial.h"
#include "echoThanks.h"
#include "echoHome.h"
#include "echoKeyboardDriver.h"

extern struct deviceOpacity{
	float currentTransparency;
	float targetTransparency;
	float lerpSpeed;
};

class echoMain : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void collectData();
		void createButtons();
		void buttonCallbacks(string strId);
		void updateDeviceIconTransparency();
		void enableMouseDevice();
		void enableEchoDevice();
		void checkEchoButtons();

		void testDataDraw();
		
		ofTrueTypeFont font;

		coreData *transferData;
		echoCore *core;
		echoMouseDriver *mouse;
		echoKeyboardDriver *keyboard;
		echoUI *userInterface;
		echoButtons *buttons;
		echoMailUI *mailUI;
		echoSocialUI *socialUI;
		echoThanksUI *thanksUI;
		echoHomeUI *homeUI;

		//GUI
		//Background
		echoBackChanger *backChanger;
		//Device transparency
		deviceOpacity *opacityManager;
		//Frames
		ofImage imgFrames;
		//Toggle
		ofImage imgToggle;
		//Interface images
		ofImage imgMouse;
		ofImage imgEchoDevice;

		//Сервисные переменные
		int width;
		int height;

		//Подключена ли мышь
		bool isMouseActive;
		//Нажата ли кнопка на контроллере
		bool isReloadButtonPressed;
		bool isFireModeButtonPressed;
		//Нажат ли спусковой крючок, либо кнопка под ним
		bool isFireButtonPressed;
		bool isZoomButtonPressed;
};
