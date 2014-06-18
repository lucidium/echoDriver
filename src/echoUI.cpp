#include "echoUI.h"

echoUI::echoUI(string startupFilePath)
{
	initializeVariables(startupFilePath);
	guiSetup();
}

echoUI::~echoUI()
{
	delete gui;
	delete uiOpacityControl;
	//Потенциальный crash
	delete fileSystem;
}

void echoUI::initializeVariables(string startupFileName)
{
	fileSystem = new FileSystem();
	//Инициализация внутренних переменных
	isGuiVisible = true;
	isGuiInitialized = false;
	isAllowedToSend = true;
	isInErrorState = false;
	isVisible = false;
	isShutdown = true;
	isActive = false;
	presetFile = "NULL";
	uiOpacityControl = new UIwidget();
	uiOpacityControl->colorBack = 0.0;
	uiOpacityControl->colorOutline = 0.0;
	uiOpacityControl->colorOutlineHighlight = 0.0;
	uiOpacityControl->colorFill = 0.0;
	uiOpacityControl->colorFillHighlight = 0.0;
	uiOpacityControl->colorPadded = 0.0;
	uiOpacityControl->colorPaddedOutline = 0.0;
	interpolationSpeed = 0.04;
	currentTransparency = 0.0;
	targetTransparency = 0.0;
	
	//Инициализация пользовательских значений
	//Получаем путь к файлу с последней конфигурацией
	string edpPath = getSettingsFile(startupFileName);
	//Загружаем конфигурацию
	if(xml.loadFile(edpPath))
	{
		gainX = xml.getValue("preset:gainX",0.0);
		gainY = xml.getValue("preset:gainY",0.0);
		gainZ = xml.getValue("preset:gainZ",0.0);
		generalFactor = xml.getValue("preset:generalFactor",0.0);
		lowThreshold = xml.getValue("preset:lowThreshold",0);
		highThreshold = xml.getValue("preset:highThreshold",0);
		lerpSpeed = xml.getValue("preset:lerpSpeed",0.0);
		decay = xml.getValue("preset:decay",0.0);
		isAxisLocked = xml.getValue("preset:isAxisLocked",false);
		isXInvert = xml.getValue("preset:isXInvert",false);
		isYInvert = xml.getValue("preset:isYInvert",false);
		isZInvert = xml.getValue("preset:isZInvert",false);
		presetFile = edpPath;
		isInErrorState = false;
	}

	if(presetFile == "NULL") isInErrorState = true;

}

string echoUI::getSettingsFile(string startupFile)
{
	if(xml.loadFile(startupFile))
	{
		return xml.getValue("startupConfig:file"," ");
	}
	else return "NULL";
}

void echoUI::update()
{
	//if(isGuiInitialized) gui->update();
	setTransparency();
}

void echoUI::draw()
{
	//ofPushStyle();
	//ofSetColor(255,255,255,64);
	//if(isGuiInitialized) gui->draw();
	//ofSetColor(255,255,255,255);
	//ofPopStyle();
}

void echoUI::guiSetup()
{
	int originX = 40;
	int originY = 130;
	int offset = 40;
	int ascention = 20;
	int margin = 350;

	gui = new ofxUICanvas();
	gui->setVisible(false);
	gui->setDrawPadding(false);
	gui->setDrawBack(false);
	//gui->addLabel("GENERAL", OFX_UI_FONT_MEDIUM);
	//New
	gui->addWidget(new ofxUILabel(originX, originY, "GENERAL", OFX_UI_FONT_MEDIUM));
	gui->addWidget(new ofxUISlider("Gain X", 1.0, 10.0, gainX, SLIDER_WIDTH, SLIDER_HEIGHT,originX,originY+offset - ascention));
	gui->addWidget(new ofxUISlider("Gain Y", 1.0, 10.0, gainY, SLIDER_WIDTH, SLIDER_HEIGHT,originX,originY+2*offset - ascention));
	gui->addWidget(new ofxUISlider("Gain Z", 1.0, 10.0, gainZ, SLIDER_WIDTH, SLIDER_HEIGHT,originX,originY+3*offset - ascention));
	gui->addWidget(new ofxUISlider("Factor", 1.0, 1000.0, generalFactor, SLIDER_WIDTH, SLIDER_HEIGHT,originX,originY+4*offset- ascention));

	gui->addWidget(new ofxUILabel(originX, originY+5*offset, "FILTERING", OFX_UI_FONT_MEDIUM));

	gui->addWidget(new ofxUISlider("Low threshold", 1, 50, lowThreshold, SLIDER_WIDTH, SLIDER_HEIGHT,originX,originY+6*offset- ascention));
	gui->addWidget(new ofxUISlider("High threshold", 1, 500, highThreshold, SLIDER_WIDTH, SLIDER_HEIGHT,originX,originY+7*offset- ascention));
	gui->addWidget(new ofxUISlider("Interpolation speed", 0.01, 1.0, lerpSpeed, SLIDER_WIDTH, SLIDER_HEIGHT,originX,originY+8*offset- ascention));
	gui->addWidget(new ofxUISlider("Decay", 0.9, 1.0, decay, SLIDER_WIDTH, SLIDER_HEIGHT,originX,originY+9*offset- ascention));

	gui->addWidget(new ofxUILabel(originX+margin, originY, "MODIFIERS", OFX_UI_FONT_MEDIUM));

	gui->addWidget(new ofxUIToggle("Lock axis", isAxisLocked, 20, 20, originX+margin, originY+offset- ascention));
	gui->addWidget(new ofxUIToggle("X invert", isXInvert, 20, 20, originX+margin, originY+2*offset- ascention));
	gui->addWidget(new ofxUIToggle("Y invert", isYInvert, 20, 20, originX+margin, originY+3*offset- ascention));
	gui->addWidget(new ofxUIToggle("Z invert", isZInvert, 20, 20, originX+margin, originY+4*offset- ascention));

	gui->addWidget(new ofxUILabel(originX+margin, originY+5*offset, "PRESETS", OFX_UI_FONT_MEDIUM));

	gui->addWidget(new ofxUILabelButton("Load defaults", false, SLIDER_WIDTH, SLIDER_HEIGHT,originX+margin,originY+6*offset- ascention,OFX_UI_FONT_LARGE));
	gui->addWidget(new ofxUILabelButton("Load preset", false, SLIDER_WIDTH, SLIDER_HEIGHT,originX+margin,originY+7*offset- ascention,OFX_UI_FONT_LARGE));
	gui->addWidget(new ofxUILabelButton("Save preset", false, SLIDER_WIDTH, SLIDER_HEIGHT,originX+margin,originY+8*offset- ascention,OFX_UI_FONT_LARGE));


	//New

    //gui->addSpacer();

	/*gui->addSlider("Gain X", 1.0, 10.0, gainX, SLIDER_WIDTH, SLIDER_HEIGHT);
	gui->addSlider("Gain Y", 1.0, 10.0, gainY, SLIDER_WIDTH, SLIDER_HEIGHT);
	gui->addSlider("Gain Z", 1.0, 10.0, gainZ, SLIDER_WIDTH, SLIDER_HEIGHT);
	gui->addSlider("Factor", 1.0, 1000.0, generalFactor, SLIDER_WIDTH, SLIDER_HEIGHT);*/

	/*gui->addSpacer();
	gui->addLabel("FILTERING", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();*/

	/*gui->addSlider("Low threshold", 1, 50, lowThreshold, SLIDER_WIDTH, SLIDER_HEIGHT);
	gui->addSlider("High threshold", 1, 500, highThreshold, SLIDER_WIDTH, SLIDER_HEIGHT);
	gui->addSlider("Interpolation speed", 0.01, 1.0, lerpSpeed, SLIDER_WIDTH, SLIDER_HEIGHT);
	gui->addSlider("Decay", 0.9, 1.0, decay, SLIDER_WIDTH, SLIDER_HEIGHT);*/

	/*gui->addSpacer();
	gui->addLabel("MODIFIERS", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();*/

	/*gui->addToggle("Lock axis", isAxisLocked);
	gui->addToggle("X invert", isXInvert);
	gui->addToggle("Y invert", isYInvert);
	gui->addToggle("Z invert", isZInvert);

	gui->addSpacer();
	gui->addLabel("PRESETS", OFX_UI_FONT_MEDIUM);
    gui->addSpacer();*/

	/*gui->addLabelButton("Load defaults", false, SLIDER_WIDTH, SLIDER_HEIGHT);
	gui->addLabelButton("Load preset", false, SLIDER_WIDTH, SLIDER_HEIGHT);
	gui->addLabelButton("Save preset", false, SLIDER_WIDTH, SLIDER_HEIGHT);*/

	gui->autoSizeToFitWidgets();  
    ofAddListener(gui->newGUIEvent,this,&echoUI::guiEvent);

	isGuiInitialized = true;
}

void echoUI::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();

	//GENERAL
	if(name == "Gain X")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		gainX = slider->getScaledValue();
	}
	else if(name == "Gain Y")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		gainY = slider->getScaledValue();
	}
	else if(name == "Gain Z")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		gainZ = slider->getScaledValue();
	}
	else if(name == "Factor")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		generalFactor = slider->getScaledValue();
	}

	//FILTERING
	else if(name == "Low threshold")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		lowThreshold = slider->getScaledValue();
	}
	else if(name == "High threshold")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		highThreshold = slider->getScaledValue();
	}
	else if(name == "Interpolation speed")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		lerpSpeed = slider->getScaledValue();
	}
	else if(name == "Decay")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		decay = slider->getScaledValue();
	}

	//MODIFIERS
	else if(name == "Lock axis")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		isAxisLocked = toggle->getValue();
	}
	else if(name == "X invert")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		isXInvert = toggle->getValue();
	}
	else if(name == "Y invert")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		isYInvert = toggle->getValue();
	}
	else if(name == "Z invert")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		isZInvert = toggle->getValue();
	}

	//PRESETS
	else if(name == "Load defaults")
	{
		ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
		if(button->getValue() == 0)
		{
			loadDefaults();
		}
	}
	else if(name == "Load preset")
	{
		ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
		if(button->getValue() == 0)
		{
			openPreset();
		}
	}
	else if(name == "Save preset")
	{
		ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
		if(button->getValue() == 0)
		{
			savePreset();
		}
	}

}

bool echoUI::getSendPermission()
{
	return isAllowedToSend;
}

bool echoUI::getGuiInitStatus()
{
	return isGuiInitialized;
}

bool echoUI::getErrorState()
{
	return isInErrorState;
}

void echoUI::loadDefaults()
{
	if(xml.loadFile(DEFAULT_PRESET))
	{
		gainX = xml.getValue("preset:gainX",0.0);
		gainY = xml.getValue("preset:gainY",0.0);
		gainZ = xml.getValue("preset:gainZ",0.0);
		generalFactor = xml.getValue("preset:generalFactor",0.0);
		lowThreshold = xml.getValue("preset:lowThreshold",0);
		highThreshold = xml.getValue("preset:highThreshold",0);
		lerpSpeed = xml.getValue("preset:lerpSpeed",0.0);
		decay = xml.getValue("preset:decay",0.0);
		isAxisLocked = xml.getValue("preset:isAxisLocked",false);
		isXInvert = xml.getValue("preset:isXInvert",false);
		isYInvert = xml.getValue("preset:isYInvert",false);
		isZInvert = xml.getValue("preset:isZInvert",false);
		presetFile = newPresetPath;
		isInErrorState = false;
		setDataToGUI(ofVec3f(gainX,gainY,gainZ),generalFactor,lerpSpeed,decay,lowThreshold,highThreshold,isAxisLocked,isXInvert,isYInvert,isZInvert);
	}
	else isInErrorState = true;
}

void echoUI::openPreset()
{
	newPresetPath = fileSystem->openXmlFile();
	if(newPresetPath != "")
	{
		replace_v1(newPresetPath,'\\','//');
		if(xml.loadFile(newPresetPath))
		{
			gainX = xml.getValue("preset:gainX",0.0);
			gainY = xml.getValue("preset:gainY",0.0);
			gainZ = xml.getValue("preset:gainZ",0.0);
			generalFactor = xml.getValue("preset:generalFactor",0.0);
			lowThreshold = xml.getValue("preset:lowThreshold",0);
			highThreshold = xml.getValue("preset:highThreshold",0);
			lerpSpeed = xml.getValue("preset:lerpSpeed",0.0);
			decay = xml.getValue("preset:decay",0.0);
			isAxisLocked = xml.getValue("preset:isAxisLocked",false);
			isXInvert = xml.getValue("preset:isXInvert",false);
			isYInvert = xml.getValue("preset:isYInvert",false);
			isZInvert = xml.getValue("preset:isZInvert",false);
			presetFile = newPresetPath;
			isInErrorState = false;
			setDataToGUI(ofVec3f(gainX,gainY,gainZ),generalFactor,lerpSpeed,decay,lowThreshold,highThreshold,isAxisLocked,isXInvert,isYInvert,isZInvert);
		}
		else isInErrorState = true;
	}
}

void echoUI::savePreset()
{
	newPresetPath = fileSystem->saveFile();
	if(newPresetPath != "")
	{
		replace_v1(newPresetPath,'\\','//');
		size_t pos = newPresetPath.find(".edp");
		if (pos == std::string::npos) newPresetPath = newPresetPath + ".edp";
		//Формируем xml документ
		xml.pushTag("preset");
			xml.setValue("gainX",this->gainX);
			xml.setValue("gainY",this->gainY);
			xml.setValue("gainZ",this->gainZ);
			xml.setValue("generalFactor",this->generalFactor);
			xml.setValue("lowThreshold",this->lowThreshold);
			xml.setValue("highThreshold",this->highThreshold);
			xml.setValue("lerpSpeed",this->lerpSpeed);
			xml.setValue("decay",this->decay);
			xml.setValue("isAxisLocked",this->isAxisLocked);
			xml.setValue("isXInvert",this->isXInvert);
			xml.setValue("isYInvert",this->isYInvert);
			xml.setValue("isZInvert",this->isZInvert);
		xml.popTag();
		xml.saveFile(newPresetPath);
	}
}

void echoUI::setDataToGUI(ofVec3f gain, float factor, float lerpSpeed, float decay, int lowT, int highT, bool isAxisLckd, bool isXInv, bool isYInv, bool isZInv)
{
	this->gainX = gain.x;
	this->gainY = gain.y;
	this->gainZ = gain.z;
	this->generalFactor = factor;
	this->decay = decay;
	this->lowThreshold = lowT;
	this->highThreshold = highT;
	this->isAxisLocked = isAxisLckd;
	this->isXInvert = isXInv;
	this->isYInvert = isYInv;
	this->isZInvert = isZInv;

	//GENERAL
	ofxUISlider *slider = (ofxUISlider *)gui->getWidget("Gain X");
	slider->setValue(gain.x);
	slider = (ofxUISlider *)gui->getWidget("Gain Y");
	slider->setValue(gain.y);
	slider = (ofxUISlider *)gui->getWidget("Gain Z");
	slider->setValue(gain.z);
	slider = (ofxUISlider *)gui->getWidget("Factor");
	slider->setValue(factor);
	//FILTERING
	slider = (ofxUISlider *)gui->getWidget("Low threshold");
	slider->setValue(lowT);
	slider = (ofxUISlider *)gui->getWidget("High threshold");
	slider->setValue(highT);
	slider = (ofxUISlider *)gui->getWidget("Interpolation speed");
	slider->setValue(lerpSpeed);
	slider = (ofxUISlider *)gui->getWidget("Decay");
	slider->setValue(decay);
	//MODIFIERS
	ofxUIToggle *toggle = (ofxUIToggle *) gui->getWidget("Lock axis");
	toggle->setValue(isAxisLckd);
	toggle = (ofxUIToggle *) gui->getWidget("X invert");
	toggle->setValue(isXInv);
	toggle = (ofxUIToggle *) gui->getWidget("Y invert");
	toggle->setValue(isYInv);
	toggle = (ofxUIToggle *) gui->getWidget("Z invert");
	toggle->setValue(isZInv);

}

string echoUI::getCurrentPresetFile()
{
	return presetFile;
}

void echoUI::replace_v1(std::string& s, char c1, char c2)
{
	std::replace(s.begin(), s.end(), c1, c2);
}

void echoUI::setVisible(bool value)
{
	gui->setVisible(value);
}

void echoUI::showUI()
{
	targetTransparency = 255.0;
	isVisible = true;
	isActive = true;
	gui->setVisible(true);

}

void echoUI::hideUI()
{
	targetTransparency = 0.0;
	isVisible = false;
	isShutdown = false;
}

void echoUI::setTransparency()
{
	currentTransparency = ofLerp(currentTransparency,targetTransparency,interpolationSpeed);
	updateUIOpacity(isVisible);
	setUIWidgetOpacity("GENERAL", isVisible);
	setUIWidgetOpacity("Gain X", isVisible);
	setUIWidgetOpacity("Gain X LABEL", isVisible);
	setUIWidgetOpacity("Gain Y", isVisible);
	setUIWidgetOpacity("Gain Y LABEL", isVisible);
	setUIWidgetOpacity("Gain Z", isVisible);
	setUIWidgetOpacity("Gain Z LABEL", isVisible);
	setUIWidgetOpacity("Factor", isVisible);
	setUIWidgetOpacity("Factor LABEL", isVisible);
	setUIWidgetOpacity("FILTERING", isVisible);
	setUIWidgetOpacity("Low threshold", isVisible);
	setUIWidgetOpacity("Low threshold LABEL", isVisible);
	setUIWidgetOpacity("High threshold", isVisible);
	setUIWidgetOpacity("High threshold LABEL", isVisible);
	setUIWidgetOpacity("Interpolation speed", isVisible);
	setUIWidgetOpacity("Interpolation speed LABEL", isVisible);
	setUIWidgetOpacity("Decay", isVisible);
	setUIWidgetOpacity("Decay LABEL", isVisible);
	setUIWidgetOpacity("MODIFIERS", isVisible);
	setUIWidgetOpacity("Lock axis", isVisible);
	setUIWidgetOpacity("Lock axis LABEL", isVisible);
	setUIWidgetOpacity("X invert", isVisible);
	setUIWidgetOpacity("X invert LABEL", isVisible);
	setUIWidgetOpacity("Y invert", isVisible);
	setUIWidgetOpacity("Y invert LABEL", isVisible);
	setUIWidgetOpacity("Z invert", isVisible);
	setUIWidgetOpacity("Z invert LABEL", isVisible);
	setUIWidgetOpacity("PRESETS", isVisible);
	setUIWidgetOpacity("Load defaults", isVisible);
	setUIWidgetOpacity("Load preset", isVisible);
	setUIWidgetOpacity("Save preset", isVisible);
	setUIWidgetOpacity("Load defaults LABEL", isVisible);
	setUIWidgetOpacity("Load preset LABEL", isVisible);
	setUIWidgetOpacity("Save preset LABEL", isVisible);

	if(!isVisible && currentTransparency < 0.5 && !isShutdown)
	{
		isShutdown = true;
		gui->setVisible(false);
		currentTransparency = 0.0;
		isActive = false;
	}
}

void echoUI::setUIWidgetOpacity(string strWidget, bool isGoingToBeVisible)
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

void echoUI::updateUIOpacity(bool isGoingToBeVisible)
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

bool echoUI::getActiveStatus()
{
	return isActive;
}
