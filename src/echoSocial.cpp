#include "echoSocial.h"

echoSocialUI::echoSocialUI()
{
	position = ofVec2f(214,180);
	offsetX = 124;
	offsetY = 146;
	interpolationSpeed = 0.04;
	currentTransparency = 0.0;
	targetTransparency = 0.0;
	isVisible = false;
	isShutdown = true;
	isActive = false;
	buttons = new echoButtons(0.05);
	createButtons();
}

echoSocialUI::~echoSocialUI()
{
	delete buttons;
}

void echoSocialUI::update()
{
	if(isActive)
	{
		currentTransparency = ofLerp(currentTransparency,targetTransparency,interpolationSpeed);
		buttons->setOpacity(currentTransparency);
		if(isVisible) buttons->update();
		else buttons->hoverFadeOut();

		if(!isVisible && currentTransparency < 0.5 && !isShutdown)
		{
			isActive = false;
			isShutdown = true;
			currentTransparency = 0.0;
		}
	}
}

void echoSocialUI::draw()
{
	if(isActive) buttons->draw();
}

void echoSocialUI::createButtons()
{
	buttons->createButton(new ofImage("gui/buttons/facebook.png"),new ofImage("gui/buttons/facebook_hover.png"),"FACEBOOK",ofVec2f(position.x,position.y));
	buttons->createButton(new ofImage("gui/buttons/twitter.png"),new ofImage("gui/buttons/twitter_hover.png"),"TWITTER",ofVec2f(position.x + offsetX,position.y));
	buttons->createButton(new ofImage("gui/buttons/youtube.png"),new ofImage("gui/buttons/youtube_hover.png"),"YOUTUBE",ofVec2f(position.x + 2*offsetX,position.y));
	buttons->createButton(new ofImage("gui/buttons/vk.png"),new ofImage("gui/buttons/vk_hover.png"),"VK",ofVec2f(position.x,position.y + offsetY));
	buttons->createButton(new ofImage("gui/buttons/google.png"),new ofImage("gui/buttons/google_hover.png"),"GOOGLE",ofVec2f(position.x + offsetX,position.y + offsetY));
	buttons->createButton(new ofImage("gui/buttons/instagram.png"),new ofImage("gui/buttons/instagram_hover.png"),"INSTAGRAM",ofVec2f(position.x + 2*offsetX,position.y + offsetY));
}

void echoSocialUI::buttonEvents(string strId)
{
	if(strId == "FACEBOOK")
	{
		ShellExecute(NULL, L"open", L"http://facebook.com", NULL, NULL, SW_SHOWNORMAL);
	}
	else if(strId == "TWITTER")
	{
		ShellExecute(NULL, L"open", L"http://twitter.com", NULL, NULL, SW_SHOWNORMAL);
	}
	else if(strId == "YOUTUBE")
	{
		ShellExecute(NULL, L"open", L"http://youtube.com", NULL, NULL, SW_SHOWNORMAL);
	}
	else if(strId == "VK")
	{
		ShellExecute(NULL, L"open", L"http://vk.com", NULL, NULL, SW_SHOWNORMAL);
	}
	else if(strId == "GOOGLE")
	{
		ShellExecute(NULL, L"open", L"http://plus.google.com", NULL, NULL, SW_SHOWNORMAL);
	}
	else if(strId == "INSTAGRAM")
	{
		ShellExecute(NULL, L"open", L"http://instagram.com", NULL, NULL, SW_SHOWNORMAL);
	}
}

void echoSocialUI::mouseReleased(int x, int y, int button)
{
	if(isVisible)
	{
		if(buttons->getHoverButtonId() != "NONE")
		{
			if(buttons->getHoverButtonId() == buttons->getClickedButtonId())
			{
				buttonEvents(buttons->getHoverButtonId());
				buttons->clearButtonInfo();
			}
		}
	}
}

void echoSocialUI::showUI()
{
	targetTransparency = 255.0;
	isVisible = true;
	isShutdown = false;
	isActive = true;
}

void echoSocialUI::hideUI()
{
	targetTransparency = 0.0;
	isVisible = false;
	isShutdown = false;
}

bool echoSocialUI::getActiveStatus()
{
	return isActive;
}