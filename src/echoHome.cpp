#include "echoHome.h"

echoHomeUI::echoHomeUI()
{
	position = ofVec2f(19,110);
	interpolationSpeed = 0.04;
	currentTransparency = 200.0;
	targetTransparency = 200.0;

	isVisible = true;
	isShutdown = false;
	isActive = true;

	carousel = new ofxCarousel("gui/carousel",0.05,5);
	mask = new ofxMaskControl(763,434,0.08,0.05,0.1);
}

echoHomeUI::~echoHomeUI()
{
	delete carousel;
}

void echoHomeUI::update()
{
	if(isActive)
	{
		currentTransparency = ofLerp(currentTransparency,targetTransparency,interpolationSpeed);

		carousel->update();
		mask->update();


		if(!isVisible && currentTransparency < 0.5 && !isShutdown)
		{
			isActive = false;
			isShutdown = true;
			currentTransparency = 0.0;
		}
	}
}

void echoHomeUI::draw()
{
	if(isActive)
	{
		ofPushStyle();
		mask->begin();
		ofEnableAlphaBlending();
			carousel->draw();
		ofDisableAlphaBlending();
		mask->end();
		ofPopStyle();

		ofPushStyle();
		ofEnableAlphaBlending();
			ofSetColor(255,255,255,currentTransparency);
				mask->draw(position.x,position.y,763,434);
		ofDisableAlphaBlending();
		ofPopStyle();
	}
}

void echoHomeUI::showUI()
{
	carousel->reset();
	targetTransparency = 200.0;
	isVisible = true;
	isShutdown = false;
	isActive = true;
}

void echoHomeUI::hideUI()
{
	targetTransparency = 0.0;
	isVisible = false;
	isShutdown = false;
}

bool echoHomeUI::getActiveStatus()
{
	return isActive;
}