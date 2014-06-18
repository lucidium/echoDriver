#pragma once

#include "ofMain.h"

class echoRingAnimation{
	public:
		echoRingAnimation();
		~echoRingAnimation();

		static echoRingAnimation& getInstance();

		void setup(float scale);
		
		void draw(int x, int y);
		void play();

		bool getCompleteStatus();

	private:

		vector<ofImage*> images;

		float sequenceFPS;
		float startFrameTime;
		float currentFrameTime;
		float scale;

		int frameAmount;
		int frameIndex;
		int prevFrameIndex;

		bool isComplete;
};