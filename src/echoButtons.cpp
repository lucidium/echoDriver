#include "echoButtons.h"

//echoButton class
echoButton::echoButton(ofImage *imgIdle, ofImage *imgHover, string strId, ofVec2f pos)
{
	this->imgIdle	= imgIdle;
	this->imgHover	= imgHover;
	this->strId		= strId;
	this->pos		= pos;
	isPointerOn		= false;
	isClicked		= false;
	hoverOpacity	= 0.0;
}

echoButton::~echoButton()
{
	delete imgIdle;
	delete imgHover;
}

void echoButton::mouseHoverCheck(ofVec2f mousePos)
{
	bool result = false;
	if(ofInRange(mousePos.x,pos.x,pos.x + imgIdle->getWidth()))
	{
		if(ofInRange(mousePos.y,pos.y,pos.y + imgIdle->getHeight())) result = true;
	}
	isPointerOn = result;
}

//echoButtons class
echoButtons::echoButtons(float lerpSpeed)
{
	this->lerpSpeed = lerpSpeed;
	isButtonClicked = false;
	strHoverButtonId = "NONE";
	strPressedButtonId = "NONE";
	clickedButton = -1;
	opacity = 255.0;
}

echoButtons::~echoButtons()
{
	for(int i = 0; i < buttons.size(); i++) delete buttons[i];
}

void echoButtons::update()
{
	strHoverButtonId = "NONE";

	for(int i = 0; i < buttons.size(); i++)
	{
		ofVec2f mousePos = ofVec2f(ofGetMouseX(),ofGetMouseY());
		buttons[i]->mouseHoverCheck(mousePos);
		if(buttons[i]->isPointerOn) 
		{
			if(!isButtonClicked) buttons[i]->hoverOpacity = ofLerp(buttons[i]->hoverOpacity,255.0,lerpSpeed);
			strHoverButtonId = buttons[i]->strId;
		}
		else
		{ 
			buttons[i]->hoverOpacity = ofLerp(buttons[i]->hoverOpacity,0.0,lerpSpeed);
		}
	}

	if(ofGetMousePressed(0))
	{
		for(int i = 0; i < buttons.size(); i++)
		{
			if(buttons[i]->isPointerOn) 
			{
				buttons[i]->hoverOpacity = ofLerp(buttons[i]->hoverOpacity,0.0,lerpSpeed);
				buttons[i]->isClicked = true;
				isButtonClicked = true;
				strPressedButtonId = buttons[i]->strId;
				clickedButton = i;
			}
		}
	}
	else
	{
		for(int i = 0; i < buttons.size(); i++)
		{
			buttons[i]->isClicked = false;
			isButtonClicked = false;
		}
	}

	//if(!isButtonClicked) strPressedButtonId = "NONE";

}

void echoButtons::hoverFadeOut()
{
	for(int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->hoverOpacity = ofLerp(buttons[i]->hoverOpacity,0.0,lerpSpeed);
	}
}

void echoButtons::draw()
{
	ofPushStyle();
	ofEnableAlphaBlending();
	for(int i = 0; i < buttons.size(); i++)
	{
		ofSetColor(255,255,255,opacity);
		buttons[i]->imgIdle->draw(buttons[i]->pos.x,buttons[i]->pos.y,buttons[i]->imgIdle->getWidth(),buttons[i]->imgIdle->getHeight());
		ofSetColor(255,255,255,buttons[i]->hoverOpacity);
		buttons[i]->imgHover->draw(buttons[i]->pos.x,buttons[i]->pos.y,buttons[i]->imgHover->getWidth(),buttons[i]->imgHover->getHeight());
	}
	ofDisableAlphaBlending();
	ofPopStyle();
}

void echoButtons::createButton(ofImage *imgIdle, ofImage *imgHover, string strId, ofVec2f pos)
{
	echoButton *newButton;
	newButton = new echoButton(imgIdle,imgHover,strId,pos);
	buttons.push_back(newButton);
	newButton = NULL;
}

string echoButtons::getHoverButtonId()
{
	return strHoverButtonId;
}

string echoButtons::getClickedButtonId()
{
	return strPressedButtonId;
}

void echoButtons::clearButtonInfo()
{
	strPressedButtonId = "NONE";
}

void echoButtons::setOpacity(float value)
{
	this->opacity = value;
}