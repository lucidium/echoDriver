#pragma once

#define SCREEN_WIDTH 763
#define SCREEN_HEIGHT 434
#define THRESHOLD 0.05

#include "ofMain.h"

class ofxTimer{
	public:
		ofxTimer(int seconds);
		~ofxTimer();

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

class ofxCarousel{
	public:
		ofxCarousel(string data_path, float lerp_speed, int time_in_seconds);
		~ofxCarousel();
		
		void update();
		void draw();
		void nextSlide();
		void reset();

	private:
		vector<ofImage*> images;
		ofVec2f pos;
		float lerpSpeed;
		int currentImageNumber;
		int nextImageNumber;
		int timeInSeconds;
		bool isDataLoaded;
		bool isTransitionFinished;
		bool isTimerBegan;

		ofxTimer *timer;
};