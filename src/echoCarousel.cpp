#include "echoCarousel.h"

//ofxTimer
ofxTimer::ofxTimer(int seconds)
{
	remainingTime = seconds;
	elapsedTime = 0.0;
	startTime = 0.0;
	timeIsOut = false;
}

ofxTimer::~ofxTimer()
{

}

void ofxTimer::update()
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

void ofxTimer::beginCountdown()
{
	startTime = ofGetElapsedTimef();
	elapsedTime = 0.0;
	isCountdownBegan = true;
	timeIsOut = false;
}

bool ofxTimer::getCompleteness()
{
	return timeIsOut;
}

//ofxCarousel
ofxCarousel::ofxCarousel(string data_path, float lerp_speed, int time_in_seconds)
{
	isDataLoaded = false;

	ofDirectory dir(data_path);
	dir.allowExt("jpg");
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

	pos = ofVec2f(0,0);
	currentImageNumber = 0;
	nextImageNumber = 1;
	this->lerpSpeed = lerp_speed;
	this->timeInSeconds = time_in_seconds;
	isTransitionFinished = true;
	isTimerBegan = false;
	timer = new ofxTimer(timeInSeconds);
}

ofxCarousel::~ofxCarousel()
{
	for(int i = 0; i < images.size(); i++)
	{ 
		delete images[i];
		images[i] = NULL;
	}
}

void ofxCarousel::update()
{
	if(!isTransitionFinished)
	{
		pos.x = ofLerp(pos.x,-SCREEN_WIDTH,lerpSpeed);
		if(pos.x < -SCREEN_WIDTH + THRESHOLD)
		{
			pos.x = pos.x + SCREEN_WIDTH;

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
		if(timer->getCompleteness()) nextSlide();
	}
}

void ofxCarousel::draw()
{
	if(isDataLoaded)
	{
		images[currentImageNumber]->draw(pos.x,pos.y,SCREEN_WIDTH,SCREEN_HEIGHT);
		images[nextImageNumber]->draw(pos.x + SCREEN_WIDTH,pos.y,SCREEN_WIDTH,SCREEN_HEIGHT);
	}
}

void ofxCarousel::nextSlide()
{
	if(isTransitionFinished)
	{
		isTransitionFinished = false;
	}
}

void ofxCarousel::reset()
{
	currentImageNumber = 0;
	nextImageNumber = 1;
	timer->beginCountdown();
	isTimerBegan = true;
}

