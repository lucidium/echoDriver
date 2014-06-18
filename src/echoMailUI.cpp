#include "echoMailUI.h"

echoMailUI::echoMailUI()
{
	initializeValiables();
	guiSetup();	
}

echoMailUI::~echoMailUI()
{
	delete gui;
	delete okAnimation;
	delete uiOpacityControl;
	mailThread.stop();
	imgTip.clear();
}

void echoMailUI::initializeValiables()
{
	isGuiInitialized = false;
	isActive = false;
	isVisible = false;
	isShutdown = true;
	interpolationSpeed = 0.04;
	currentTransparency = 0.0;
	targetTransparency = 0.0;
	hUItransparency = 0.0;
	uiOpacityControl = new UIopacity();
	uiOpacityControl->colorBack = 0.0;
	uiOpacityControl->colorOutline = 0.0;
	uiOpacityControl->colorOutlineHighlight = 0.0;
	uiOpacityControl->colorFill = 0.0;
	uiOpacityControl->colorFillHighlight = 0.0;
	uiOpacityControl->colorPadded = 0.0;
	uiOpacityControl->colorPaddedOutline = 0.0;
}

void echoMailUI::update()
{
	if(isActive) setTransparency();
}

void echoMailUI::draw()
{
	if(isActive)
	{
		ofPushStyle();
			okAnimation->draw(575,285);
				ofPushStyle();
					ofEnableAlphaBlending();
					ofSetColor(255,255,255,currentTransparency);
						imgTip.draw(400,250);
					ofDisableAlphaBlending();
				ofPopStyle();
		ofPopStyle();
	}
}

void echoMailUI::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	if(name == "Send")
	{
		ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
		if(button->getValue() == 0)
		{
			int code = getSendStatus();
			if(code == 1)
			{
				string strMessage = textBox1->getLabel() + "\n\n" + textArea1->getText();
				mailThread.start();
				mailThread.setInformation("markusstanton@uac.com", textBox2->getLabel(), strMessage);
				textBox1->setLabel("");
				textBox2->setLabel("");
				textArea1->setText("");
				textBox2->unselect();
				textBox1->unselect();
				textArea1->unselect();
				okAnimation->play();
			}
			else if(code == 2)
			{

			}
		}
	}
}

void echoMailUI::guiSetup()
{

	//hgui
	hui = hGui::getInstance();
	hui->setup("gui/fonts/NewMediaFett.ttf", 9);

	mainPanel = hui->addPanel("mainPanel", NULL, HGUI_ABSOLUTE_POSITION, hui->margin1, hui->margin1+hui->margin3, 400, 400, true);
	mainPanel->setVisibleBackground(false);
	mainPanel->setVisibleBorder(false);
	hui->setRootWidget(mainPanel);
	hui->addListeners();

	events = hEvents::getInstance();
	events->setup();

	events->addObject("echoMailUI", this);

	textBox1 = hui->addTextBox("textBox1", mainPanel, HGUI_ABSOLUTE_POSITION, 40, 150, 200, "");
	textBox1->setVisibleBorder(false);

	textBox2 = hui->addTextBox("textBox2", mainPanel, HGUI_ABSOLUTE_POSITION, 40, 200, 200, "");
	textBox2->setVisibleBorder(false);

	textArea1 = hui->addTextArea("textArea1", mainPanel, HGUI_ABSOLUTE_POSITION, 40, 250, 300, 200);
	textArea1->setColor(255);
	textArea1->setVisibleBorder(false);
	textArea1->setText("", false);

	hui->hide();
	

	//ofxUI
	gui = new ofxUICanvas();
	gui->setVisible(false);
	gui->setDrawBack(false);

	gui->addWidget(new ofxUILabel(40, 130, "Your name", OFX_UI_FONT_SMALL));
	gui->addWidget(new ofxUILabel(40, 180, "Your email", OFX_UI_FONT_SMALL));
	gui->addWidget(new ofxUILabel(40, 230, "Message", OFX_UI_FONT_SMALL));
	gui->addWidget(new ofxUILabelButton(40, 460, 100, false, "Send", OFX_UI_FONT_SMALL));

	gui->autoSizeToFitWidgets(); 
	ofAddListener(gui->newGUIEvent,this,&echoMailUI::guiEvent);
	isGuiInitialized = true;

	//Animation
	okAnimation = new echoRingAnimation();
	okAnimation->setup(0.5);

	//Mail tip image
	imgTip.loadImage("gui/frames/mailtip.png");

}

bool echoMailUI::isFieldsCorrect()
{
	bool result = true;
	//Проверка на заполненность полей
	if(textBox1->getLabel().size() == 0 || textBox2->getLabel().size() == 0 || textArea1->getText().size() == 0) result = false;
	//Проверка на корректность заполнения e-mail поля
	size_t posAt = textBox2->getLabel().find("@");
	size_t posDot = textBox2->getLabel().find(".");
	if(posAt == std::string::npos || posDot == std::string::npos) result = false;

	return result;
}

bool echoMailUI::isConnectedToInternet()
{
	if(InternetCheckConnection(L"http://www.google.com",FLAG_ICC_FORCE_CONNECTION,0)) return true;
	else return false;
}

int echoMailUI::getSendStatus()
{
	int errorCode = 0;

	if(isConnectedToInternet())
	{
		if(isFieldsCorrect()) errorCode = 1;
		else errorCode = 2;
	}
	else errorCode = 3;

	return errorCode;
}

void echoMailUI::showUI()
{
	targetTransparency = 255.0;
	isVisible = true;
	isActive = true;
	gui->setVisible(true);
	hui->show();

}

void echoMailUI::hideUI()
{
	textBox1->setLabel("");
	textBox2->setLabel("");
	textArea1->setText("");
	textBox2->unselect();
	textBox1->unselect();
	textArea1->unselect();
	targetTransparency = 0.0;
	isVisible = false;
	isShutdown = false;
}

void echoMailUI::setTransparency()
{
	currentTransparency = ofLerp(currentTransparency,targetTransparency,interpolationSpeed);
	updateUIOpacity(isVisible);
	setUIWidgetOpacity("Your name", isVisible);
	setUIWidgetOpacity("Your email", isVisible);
	setUIWidgetOpacity("Message", isVisible);
	setUIWidgetOpacity("Send", isVisible);
	setUIWidgetOpacity("Send LABEL", isVisible);
	setHUIOpacity(isVisible);	

	if(!isVisible && currentTransparency < 0.5 && !isShutdown)
	{
		isShutdown = true;
		isActive = false;
		currentTransparency = 0.0;
		gui->setVisible(false);
		hui->hide();
	}
}

void echoMailUI::updateUIOpacity(bool isGoingToBeVisible)
{
	if(isGoingToBeVisible)
	{
		uiOpacityControl->colorBack = ofLerp(uiOpacityControl->colorBack,COLOR_BACK_ALPHA,interpolationSpeed);
		uiOpacityControl->colorOutline = ofLerp(uiOpacityControl->colorOutline,COLOR_OUTLINE_ALPHA,interpolationSpeed);
		uiOpacityControl->colorOutlineHighlight = ofLerp(uiOpacityControl->colorOutlineHighlight,COLOR_OUTLINE_HIGHLIGHT_ALPHA,interpolationSpeed);
		uiOpacityControl->colorFill = ofLerp(uiOpacityControl->colorFill,COLOR_FILL_ALPHA,interpolationSpeed);
		uiOpacityControl->colorFillHighlight = ofLerp(uiOpacityControl->colorFillHighlight,COLOR_FILL_HIGHLIGHT_ALPHA,interpolationSpeed);
		uiOpacityControl->colorPadded = ofLerp(uiOpacityControl->colorPadded,COLOR_PADDED_ALPHA,interpolationSpeed);
		uiOpacityControl->colorPaddedOutline = ofLerp(uiOpacityControl->colorPaddedOutline,COLOR_PADDED_OUTLINE_ALPHA,interpolationSpeed);
	}
	else
	{
		uiOpacityControl->colorBack = ofLerp(uiOpacityControl->colorBack,0,interpolationSpeed);
		uiOpacityControl->colorOutline = ofLerp(uiOpacityControl->colorOutline,0,interpolationSpeed);
		uiOpacityControl->colorOutlineHighlight = ofLerp(uiOpacityControl->colorOutlineHighlight,0,interpolationSpeed);
		uiOpacityControl->colorFill = ofLerp(uiOpacityControl->colorFill,0,interpolationSpeed);
		uiOpacityControl->colorFillHighlight = ofLerp(uiOpacityControl->colorFillHighlight,0,interpolationSpeed);
		uiOpacityControl->colorPadded = ofLerp(uiOpacityControl->colorPadded,0,interpolationSpeed);
		uiOpacityControl->colorPaddedOutline = ofLerp(uiOpacityControl->colorPaddedOutline,0,interpolationSpeed);
	}
}

void echoMailUI::setUIWidgetOpacity(string strWidget, bool isGoingToBeVisible)
{
	if(isGoingToBeVisible)
	{
		gui->getWidget(strWidget)->setColorBack(ofColor(0,uiOpacityControl->colorBack));
		gui->getWidget(strWidget)->setColorFill(ofColor(255,255,255,uiOpacityControl->colorFill));
		gui->getWidget(strWidget)->setColorFillHighlight(ofColor(255,255,255,uiOpacityControl->colorFillHighlight));
		gui->getWidget(strWidget)->setColorOutline(ofColor(255, 255, 255, uiOpacityControl->colorOutline));
		gui->getWidget(strWidget)->setColorOutlineHighlight(ofColor(255, 255, 255, uiOpacityControl->colorOutlineHighlight));
		gui->getWidget(strWidget)->setColorPadded(ofColor(0, uiOpacityControl->colorPadded));
		gui->getWidget(strWidget)->setColorPaddedOutline(ofColor(255, uiOpacityControl->colorPaddedOutline));
	}
	else
	{
		gui->getWidget(strWidget)->setColorBack(ofColor(0,uiOpacityControl->colorBack));
		gui->getWidget(strWidget)->setColorFill(ofColor(255,255,255,uiOpacityControl->colorFill));
		gui->getWidget(strWidget)->setColorFillHighlight(ofColor(255,255,255,uiOpacityControl->colorFillHighlight));
		gui->getWidget(strWidget)->setColorOutline(ofColor(255, 255, 255, uiOpacityControl->colorOutline));
		gui->getWidget(strWidget)->setColorOutlineHighlight(ofColor(255, 255, 255, uiOpacityControl->colorOutlineHighlight));
		gui->getWidget(strWidget)->setColorPadded(ofColor(0, uiOpacityControl->colorPadded));
		gui->getWidget(strWidget)->setColorPaddedOutline(ofColor(255, uiOpacityControl->colorPaddedOutline));
	}
}

void echoMailUI::setHUIOpacity(bool isGoingToBeVisible)
{
	if(isGoingToBeVisible)
	{
		hUItransparency = ofLerp(hUItransparency,25,interpolationSpeed);
		hui->setCustomOpacity(hUItransparency);
	}
	else
	{
		hUItransparency = ofLerp(hUItransparency,0,interpolationSpeed);
		hui->setCustomOpacity(hUItransparency);
	}
}

bool echoMailUI::getActiveStatus()
{
	return isActive;
}