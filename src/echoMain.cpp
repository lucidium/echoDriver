#include "echoMain.h"

void echoMain::setup()
{
	ofSetFrameRate(100);
	ofSetVerticalSync(false);
	ofBackground(0,0,0);

	ofSetWindowPosition((ofGetScreenWidth() - ofGetWindowWidth())/2,(ofGetScreenHeight() - ofGetWindowHeight())/2);
	width = ofGetWindowWidth();
	height = ofGetWindowHeight();

	font.loadFont("gui/fonts/DIN.otf", 8);

	transferData = new coreData();
	core = new echoCore();
	core->connectToFirstDevice();
	mouse = new echoMouseDriver();
	keyboard = new echoKeyboardDriver();

	opacityManager = new deviceOpacity();
	opacityManager->currentTransparency = 255.0;
	opacityManager->targetTransparency = 255.0;
	opacityManager->lerpSpeed = 0.04;

	//Мышь активна изначально
	isMouseActive = true;
	//Никакие кнопки на контроллере не нажаты
	isReloadButtonPressed = false;
	isFireModeButtonPressed = false;
	isFireButtonPressed = false;
	isZoomButtonPressed = false;

	//Загрузка ресурсов графического пользовательского интерфейса
	userInterface = new echoUI("settings/startup.edc");
	//Кнопки
	buttons = new echoButtons(0.05);
	createButtons();
	//Background
	backChanger = new echoBackChanger(BACK_PATH,0.008,3);
	//Frames
	imgFrames.loadImage("gui/frames/frames.png");
	//Toggle
	imgToggle.loadImage("gui/buttons/toggle.png");
	//Mouse
	imgMouse.loadImage("gui/buttons/mouse.png");
	//EchoDevice
	imgEchoDevice.loadImage("gui/buttons/echodevice.png");
	//Интерфейс формы отправки e-mail
	mailUI = new echoMailUI();
	//Интерфейс доступа к социальным сетям
	socialUI = new echoSocialUI();
	//Интерфейс инфо-панели благодарностей
	thanksUI = new echoThanksUI();
	//Интерфейс основного экрана
	homeUI = new echoHomeUI();
}

void echoMain::createButtons()
{
	buttons->createButton(new ofImage("gui/buttons/close.png"),new ofImage("gui/buttons/close_hover.png"),"CLOSE",ofVec2f(756,3));
	buttons->createButton(new ofImage("gui/buttons/minimize.png"),new ofImage("gui/buttons/minimize_hover.png"),"MINIMIZE",ofVec2f(728,3));
	buttons->createButton(new ofImage("gui/buttons/settings.png"),new ofImage("gui/buttons/settings_hover.png"),"SETTINGS",ofVec2f(350,20));
	buttons->createButton(new ofImage("gui/buttons/social.png"),new ofImage("gui/buttons/social_hover.png"),"SOCIAL",ofVec2f(440,20));
	buttons->createButton(new ofImage("gui/buttons/thanks.png"),new ofImage("gui/buttons/thanks_hover.png"),"THANKS",ofVec2f(530,23));
	buttons->createButton(new ofImage("gui/buttons/contact.png"),new ofImage("gui/buttons/contact_hover.png"),"CONTACT",ofVec2f(620,24));
	buttons->createButton(new ofImage("gui/buttons/home.png"),new ofImage("gui/buttons/home_hover.png"),"HOME",ofVec2f(260,21));
	buttons->createButton(new ofImage("gui/buttons/url.png"),new ofImage("gui/buttons/url_hover.png"),"URL",ofVec2f(307,564));
}

void echoMain::update()
{
	core->update();
	userInterface->update();
	collectData();
	backChanger->update();
	buttons->update();
	mailUI->update();
	socialUI->update();
	thanksUI->update();
	homeUI->update();

	updateDeviceIconTransparency();

	if(!isMouseActive)
	{
		mouse->mouseMove(core->getInterpolatedAcceleration().z,core->getInterpolatedAcceleration().y);
		checkEchoButtons();
	}
}

void echoMain::draw(){
	ofEnableAlphaBlending();

	backChanger->draw();
	imgToggle.draw(580,566);
	
	ofPushStyle();
		ofSetColor(255,255,255,opacityManager->currentTransparency);
		imgMouse.draw(100,566);
		ofSetColor(255,255,255,255.0 - opacityManager->currentTransparency);
		imgEchoDevice.draw(70,572);
	ofPopStyle();

	mailUI->draw();
	socialUI->draw();
	thanksUI->draw();
	homeUI->draw();
	imgFrames.draw(0,0,width,height);
	buttons->draw();

	ofDisableAlphaBlending();


	userInterface->draw();
}

void echoMain::testDataDraw()
{
	ofPushStyle();
		ofSetColor(0,255,0,255);
		font.drawString("fps = " + ofToString(ofGetFrameRate()),20,20);
		ofSetColor(255,255,255,255);

		ofSetColor(0,255,255,255);
		font.drawString("xRaw = " + ofToString(core->getRawX()),20,30);
		font.drawString("yRaw = " + ofToString(core->getRawY()),20,40);
		font.drawString("zRaw = " + ofToString(core->getRawZ()),20,50);
		ofSetColor(255,255,255,255);

		ofSetColor(255,255,0,255);
		font.drawString("xFiltered = " + ofToString(core->getFilteredX()),100,30);
		font.drawString("yFiltered = " + ofToString(core->getFilteredY()),100,40);
		font.drawString("zFiltered = " + ofToString(core->getFilteredZ()),100,50);
		ofSetColor(255,255,255,255);

		ofSetColor(255,0,255,255);
		font.drawString("xSmooth = " + ofToString(core->getSmoothedX()),200,30);
		font.drawString("ySmooth = " + ofToString(core->getSmoothedY()),200,40);
		font.drawString("zSmooth = " + ofToString(core->getSmoothedZ()),200,50);
		ofSetColor(255,255,255,255);

		ofSetColor(128,128,255,255);
		font.drawString("xAcc = " + ofToString(core->getAcceleration().x),300,30);
		font.drawString("yAcc = " + ofToString(core->getAcceleration().y),300,40);
		font.drawString("zAcc = " + ofToString(core->getAcceleration().z),300,50);
		ofSetColor(255,255,255,255);

		ofSetColor(255,128,64,255);
		font.drawString("xInterAcc = " + ofToString(core->getInterpolatedAcceleration().x),400,30);
		font.drawString("yInterAcc = " + ofToString(core->getInterpolatedAcceleration().y),400,40);
		font.drawString("zInterAcc = " + ofToString(core->getInterpolatedAcceleration().z),400,50);
		ofSetColor(255,255,255,255);

		ofSetColor(255,64,64,255);
		font.drawString("button1 = " + ofToString(core->isBtn1Pressed()),500,30);
		font.drawString("button2 = " + ofToString(core->isBtn2Pressed()),500,40);
		font.drawString("button3 = " + ofToString(core->isBtn3Pressed()),500,50);
		font.drawString("button4 = " + ofToString(core->isBtn4Pressed()),500,60);
		ofSetColor(255,255,255,255);
		
	ofPopStyle();
}

void echoMain::checkEchoButtons()
{
	//Кнопки мыши
	//Fire
	if(core->isBtn2Pressed())
	{ 
		mouse->mouseButton1Event(true);
		isFireButtonPressed = true;
	}
	if(!core->isBtn2Pressed() && isFireButtonPressed)
	{
		mouse->mouseButton1Event(false);
		isFireButtonPressed = false;
	}

	//Zoom
	if(core->isBtn4Pressed())
	{
		mouse->mouseButton2Event(true);
		isZoomButtonPressed = true;
	}
	if(!core->isBtn4Pressed() && isZoomButtonPressed)
	{
		mouse->mouseButton2Event(false);
		isZoomButtonPressed = false;
	}

	//Кнопки клавиатуры
	//Reload
	if(core->isBtn1Pressed() && !isReloadButtonPressed)
	{
		keyboard->pressReloadKey();
		isReloadButtonPressed = true;
	}
	//Switch fire mode
	if(core->isBtn3Pressed() && !isFireModeButtonPressed)
	{
		keyboard->pressFireModeKey();
		isFireModeButtonPressed = true;
	}

	//Разрешаем повторное нажатие кнопок только в том
	//случае, если их отпустили
	if(!core->isBtn1Pressed() && isReloadButtonPressed) isReloadButtonPressed = false;
	if(!core->isBtn3Pressed() && isFireModeButtonPressed) isFireModeButtonPressed = false;
}

void echoMain::keyPressed(int key){
	if(key == '1') enableMouseDevice();
	if(key == '2') enableEchoDevice();
}

void echoMain::keyReleased(int key){

}

void echoMain::mouseMoved(int x, int y ){

}

void echoMain::mouseDragged(int x, int y, int button){

}

void echoMain::mousePressed(int x, int y, int button){
	
}

void echoMain::mouseReleased(int x, int y, int button)
{
	if(buttons->getHoverButtonId() != "NONE")
	{
		if(buttons->getHoverButtonId() == buttons->getClickedButtonId())
		{
			buttonCallbacks(buttons->getHoverButtonId());
			buttons->clearButtonInfo();
		}
	}
	socialUI->mouseReleased(x,y,button);
}

void echoMain::windowResized(int w, int h){

}

void echoMain::gotMessage(ofMessage msg){

}

void echoMain::dragEvent(ofDragInfo dragInfo){ 

}

void echoMain::collectData()
{
	if(userInterface->getSendPermission() && userInterface->getGuiInitStatus())
	{
		//Получаем свежие данные о настройках от пользователя
		transferData->gainX			= userInterface->gainX;
		transferData->gainY			= userInterface->gainY;
		transferData->gainZ			= userInterface->gainZ;
		transferData->generalFactor = userInterface->generalFactor;
		transferData->lerpSpeed		= userInterface->lerpSpeed;
		transferData->decay			= userInterface->decay;
		transferData->isAxisLocked	= userInterface->isAxisLocked;
		transferData->isXInvert		= userInterface->isXInvert;
		transferData->isYInvert		= userInterface->isYInvert;
		transferData->isZInvert		= userInterface->isZInvert;
		transferData->lowThreshold	= userInterface->lowThreshold;
		transferData->highThreshold = userInterface->highThreshold;
		
		//Отправляем данные в ядро
		core->userData = this->transferData;
	}	
}

void echoMain::buttonCallbacks(string strId)
{
	if(strId == "CLOSE")
	{
		ofExit();
	}
	else if(strId == "MINIMIZE")
	{
		HWND hwnd = GetActiveWindow();
		ShowWindow(hwnd,SW_MINIMIZE);
	}
	else if(strId == "URL")
	{
		ShellExecute(NULL, L"open", L"http://echocontroller.com", NULL, NULL, SW_SHOWNORMAL);
	}
	else if(strId == "HOME")
	{
		if(!homeUI->getActiveStatus())
		{
			if(userInterface->getActiveStatus()) userInterface->hideUI();
			if(mailUI->getActiveStatus()) mailUI->hideUI();
			if(socialUI->getActiveStatus()) socialUI->hideUI();
			if(thanksUI->getActiveStatus()) thanksUI->hideUI();
			homeUI->showUI();
		}
	}
	else if(strId == "SETTINGS")
	{
		if(!userInterface->getActiveStatus())
		{
			if(homeUI->getActiveStatus()) homeUI->hideUI();
			if(mailUI->getActiveStatus()) mailUI->hideUI();
			if(socialUI->getActiveStatus()) socialUI->hideUI();
			if(thanksUI->getActiveStatus()) thanksUI->hideUI();
			userInterface->showUI();
		}
	}
	else if(strId == "SOCIAL")
	{
		if(!socialUI->getActiveStatus())
		{
			if(homeUI->getActiveStatus()) homeUI->hideUI();
			if(mailUI->getActiveStatus()) mailUI->hideUI();
			if(userInterface->getActiveStatus()) userInterface->hideUI();
			if(thanksUI->getActiveStatus()) thanksUI->hideUI();
			socialUI->showUI();
		}
	}
	else if(strId == "THANKS")
	{
		if(!thanksUI->getActiveStatus())
		{
			if(homeUI->getActiveStatus()) homeUI->hideUI();
			if(mailUI->getActiveStatus()) mailUI->hideUI();
			if(userInterface->getActiveStatus()) userInterface->hideUI();
			if(socialUI->getActiveStatus()) socialUI->hideUI();
			thanksUI->showUI();
		}
	}
	else if(strId == "CONTACT")
	{
		if(!mailUI->getActiveStatus())
		{
			if(homeUI->getActiveStatus()) homeUI->hideUI();
			if(thanksUI->getActiveStatus()) thanksUI->hideUI();
			if(userInterface->getActiveStatus()) userInterface->hideUI();
			if(socialUI->getActiveStatus()) socialUI->hideUI();
			mailUI->showUI();
		}
	}

}

void echoMain::updateDeviceIconTransparency()
{
	opacityManager->currentTransparency = ofLerp(opacityManager->currentTransparency,opacityManager->targetTransparency,opacityManager->lerpSpeed);
}

void echoMain::enableEchoDevice()
{
	isMouseActive = false;
	opacityManager->targetTransparency = 0.0;
}

void echoMain::enableMouseDevice()
{
	isMouseActive = true;
	opacityManager->targetTransparency = 255.0;
}

void echoMain::exit()
{
	imgFrames.clear();
	imgToggle.clear();
	core->disconnect();
	delete core;
	delete mouse;
	delete userInterface;
	delete backChanger;
	delete transferData;
	delete mailUI;
	delete socialUI;
	delete thanksUI;
	delete homeUI;
}