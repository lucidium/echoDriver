#pragma once

#define COLOR_BACK_ALPHA 25
#define COLOR_OUTLINE_ALPHA 100
#define COLOR_OUTLINE_HIGHLIGHT_ALPHA 200
#define COLOR_FILL_ALPHA 200
#define COLOR_FILL_HIGHLIGHT_ALPHA 255
#define COLOR_PADDED_ALPHA 100
#define COLOR_PADDED_OUTLINE_ALPHA 200
#define SLIDER_WIDTH 300
#define SLIDER_HEIGHT 20
#define DEFAULT_PRESET "settings/default.edp"

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"
#include "echoFilesystem.h"

extern struct UIwidget{
	float colorBack;
	float colorOutline;
	float colorOutlineHighlight;
	float colorFill;
	float colorFillHighlight;
	float colorPadded;
	float colorPaddedOutline;
};

class echoUI{
	public:
		echoUI(string startupFilePath);
		~echoUI();

		void update();
		void draw();

		void setVisible(bool value);
		void showUI();
		void hideUI();
		bool getActiveStatus();

		void guiEvent(ofxUIEventArgs &e);

		bool getGuiInitStatus();
		bool getSendPermission();
		bool getErrorState();

		string getCurrentPresetFile();

		//ѕараметры
		float gainX;
		float gainY;
		float gainZ;
		float generalFactor;
		float lerpSpeed;
		float decay;

		int lowThreshold;
		int highThreshold;

		bool isAxisLocked;
		bool isXInvert;
		bool isYInvert;
		bool isZInvert;

	private:
		void guiSetup();
		void initializeVariables(string startupFileName);
		void setDataToGUI(ofVec3f gain, float factor, float lerpSpeed, float decay, int lowT, int highT, bool isAxisLckd, bool isXInv, bool isYInv, bool isZInv);
		string getSettingsFile(string startupFile);
		void loadDefaults();
		void openPreset();
		void savePreset();
		void setTransparency();
		void updateUIOpacity(bool isGoingToBeVisible);
		void setUIWidgetOpacity(string strWidget, bool isGoingToBeVisible);
		void replace_v1(std::string& s, char c1, char c2);

		bool isGuiVisible;
		bool isAllowedToSend;
		bool isGuiInitialized;
		bool isInErrorState;

		//”правление по€влением/исчезновением
		//—корость интерпол€ции дл€ изменени€ прозрачности
		float interpolationSpeed;
		float targetTransparency;
		float currentTransparency;
		bool isVisible;
		bool isShutdown;
		bool isActive;


		ofxUICanvas *gui;
		ofxXmlSettings xml;
		FileSystem *fileSystem;
		UIwidget *uiOpacityControl;

		string presetFile;
		string newPresetPath;
};