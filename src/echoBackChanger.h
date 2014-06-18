#pragma once

#define FULL_TRANSPARENT 0.0
#define FULL_VISIBLE 255.0
#define THRESHOLD 0.05

#include "ofMain.h"

class ofxBackTimer{
	public:
		ofxBackTimer(int seconds);
		~ofxBackTimer();

		void update();
		void beginCountdown();

		bool getCompleteness();

	private:
		float remainingTime;
		float elapsedTime;
		float startTime;
		bool timeIsOut;
		bool isCountdownBegan;
};

class echoBackChanger{
	public:
		echoBackChanger(string data_path, float lerp_speed, int time_in_seconds);
		~echoBackChanger();

		void update();
		void draw();
		void nextBackground();

	private:
		vector<ofImage*> images;
		ofVec2f pos;
		int currentImageNumber;
		int nextImageNumber;
		int timeInSeconds;
		bool isDataLoaded;
		bool isTransitionFinished;
		bool isTimerBegan;

		float lerpSpeed;
		float backOpacity;

		ofxBackTimer *timer;
};