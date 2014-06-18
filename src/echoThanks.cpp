#include "echoThanks.h"

echoThanksUI::echoThanksUI()
{
	position = ofVec2f(19,110);
	interpolationSpeed = 0.04;
	currentTransparency = 0.0;
	targetTransparency = 0.0;
	offsetSpeed = 0.3;
	offsetY = 15.0;
	isVisible = false;
	isShutdown = true;
	isActive = false;
	isSlideActive = false;

	createLine("You know what you are",14,763);
	createLine("",12,763);
	createLine("I tried to sneak myself through, tried to get to the other side",12,763);
	createLine("I had to patch up the cracks and the holes that I have to hide",12,763);
	createLine("For a little bit of time even made it work okay",12,763);
	createLine("Just long enough to really make it hurt",12,763);
	createLine("When they figured me out and it all just rotted away",12,763);
	createLine("",12,763);
	createLine("Don't you fucking know what you are? [x3]",12,763);
	createLine("Go on get back to where you belong!",12,763);
	createLine("",12,763);
	createLine("You better take a good look cause I'm full of shit",12,763);
	createLine("With every bit of my heart I've tried to believe in it",12,763);
	createLine("You can dress it all up, you can try to pretend",12,763);
	createLine("But you can't change anything",12,763);
	createLine("You can't change anything",12,763);
	createLine("In the end",12,763);
	createLine("",12,763);
	createLine("Don't you fucking know what you are? [x3]",12,763);
	createLine("Go on get back to where you belong!",12,763);
	createLine("",12,763);
	createLine("Remember where you came from, remember what you are [x9]",12,763);
	createLine("",12,763);
	createLine("Don't you fucking know what you are?",12,763);
	createLine("Go on get back to where you belong!",12,763);

	fboText.allocate(763,434,GL_RGBA);
 
    fboText.begin();
		ofClear(0,0,0,255);
    fboText.end();
}

echoThanksUI::~echoThanksUI()
{
	
}

void echoThanksUI::createLine(string strLine, int fontSize, int width)
{
	ofxTextBlock *newText = new ofxTextBlock();
	newText->init("gui/fonts/NewMediaFett.ttf", fontSize);
	newText->setText(strLine);
	newText->wrapTextX(width);
	lines.push_back(newText);
	newText = NULL;
}

void echoThanksUI::update()
{
	if(isActive)
	{
		currentTransparency = ofLerp(currentTransparency,targetTransparency,interpolationSpeed);

		if(isSlideActive)
		{
			textPivot.y -= offsetSpeed; 
			if(textPivot.y < END_POINT) resetTextSlide();
		}

		if(!isVisible && currentTransparency < 0.5 && !isShutdown)
		{
			isActive = false;
			isShutdown = true;
			currentTransparency = 0.0;
			stopTextSlide();
			resetTextSlide();
		}
	}
}

void echoThanksUI::draw()
{
	if(isActive)
	{
		fboText.begin();
			ofPushStyle();
				ofClear(0, 0, 0, 0); 
				ofEnableAlphaBlending();
					for(int i = 0; i < lines.size(); i++) lines[i]->drawCenter(textPivot.x,textPivot.y + (offsetY * i) );
				ofDisableAlphaBlending();
			ofPopStyle();
		fboText.end();

		ofPushStyle();
			ofEnableAlphaBlending();
				ofSetColor(255,255,255,currentTransparency);
				fboText.draw(position.x,position.y);
			ofDisableAlphaBlending();
		ofPopStyle();
	}
}

void echoThanksUI::showUI()
{
	targetTransparency = 200.0;
	isVisible = true;
	isShutdown = false;
	isActive = true;
	resetTextSlide();
	startTextSlide();
}

void echoThanksUI::hideUI()
{
	targetTransparency = 0.0;
	isVisible = false;
	isShutdown = false;
}

void echoThanksUI::startTextSlide()
{
	isSlideActive = true;
}

void echoThanksUI::stopTextSlide()
{
	isSlideActive = false;
}

void echoThanksUI::resetTextSlide()
{
	textPivot = ofVec2f(fboText.getWidth()/2,fboText.getHeight());
}

bool echoThanksUI::getActiveStatus()
{
	return isActive;
}