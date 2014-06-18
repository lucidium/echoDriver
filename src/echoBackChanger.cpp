#include "echoBackChanger.h"

//ofxBackTimer class
ofxBackTimer::ofxBackTimer(int seconds)
{
	remainingTime = seconds;
	elapsedTime = 0.0;
	startTime = 0.0;
	timeIsOut = false;
}

ofxBackTimer::~ofxBackTimer()
{

}

void ofxBackTimer::update()
{
	if(isCountdownBegan)
	{
		elapsedTime = abs(ofGetElapsedTimef() - startTime);
		if(elapsedTime > remainingTime)
		{
			timeIsOut = true;
			isCountdownBegan = false;
		}
	}
}

void ofxBackTimer::beginCountdown()
{
	startTime = ofGetElapsedTimef();
	elapsedTime = 0.0;
	isCountdownBegan = true;
	timeIsOut = false;
}

bool ofxBackTimer::getCompleteness()
{
	return timeIsOut;
}

//echoBackChanger class
echoBackChanger::echoBackChanger(string data_path, float lerp_speed, int time_in_seconds)
{
	isDataLoaded = false;

	ofDirectory dir(data_path);
	dir.allowExt("png");
	dir.listDir();
	dir.sort();

	for(int i = 0; i < dir.numFiles(); i++)
	{
		ofImage *img = new ofImage();
		img->loadImage(dir.getPath(i));
		images.push_back(img);
		img = NULL;
	}

	if(images.size() > 0) isDataLoaded = true;

	backOpacity = FULL_VISIBLE;

	currentImageNumber = 0;
	nextImageNumber = 1;
	this->lerpSpeed = lerp_speed;
	this->timeInSeconds = time_in_seconds;
	isTransitionFinished = true;
	isTimerBegan = false;
	timer = new ofxBackTimer(timeInSeconds);
}

echoBackChanger::~echoBackChanger()
{
	for(int i = 0; i < images.size(); i++)
	{ 
		images[i] = NULL;
		delete images[i];
	}
}

void echoBackChanger::update()
{
	if(!isTransitionFinished)
	{
		backOpacity = ofLerp(backOpacity,FULL_TRANSPARENT,lerpSpeed);
		if(backOpacity < FULL_TRANSPARENT + THRESHOLD)
		{
			backOpacity = FULL_VISIBLE;

			currentImageNumber = nextImageNumber;

			if(currentImageNumber == images.size() - 1) nextImageNumber = 0;
			else nextImageNumber = nextImageNumber + 1;

			isTransitionFinished = true;
			isTimerBegan = false;
		}
	}
	else
	{
		if(!isTimerBegan)
		{
			timer->beginCountdown();
			isTimerBegan = true;
		}
		timer->update();
		if(timer->getCompleteness()) nextBackground();
	}
}

void echoBackChanger::draw()
{
	if(isDataLoaded)
	{
		ofPushStyle();
			ofEnableAlphaBlending();
				ofSetColor(255,255,255,backOpacity);
					images[currentImageNumber]->draw(0,0);
				ofSetColor(255,255,255,255 - backOpacity);
					images[nextImageNumber]->draw(0,0);
			ofDisableAlphaBlending();
		ofPopStyle();
	}
}

void echoBackChanger::nextBackground()
{
	if(isTransitionFinished)
	{
		isTransitionFinished = false;
	}
}