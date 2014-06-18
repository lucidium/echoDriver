#include "echoRingAnimation.h"

echoRingAnimation::echoRingAnimation()
{
	
}

echoRingAnimation::~echoRingAnimation()
{
	for(int i = 0; i < images.size(); i++) 
	{
		images[i]->clear();
		images[i] = NULL;
	}
}

echoRingAnimation& echoRingAnimation::getInstance(){

	static echoRingAnimation animation;
	return animation;
}

void echoRingAnimation::setup(float scale)
{
	//Загружаем кадры анимации
	ofDirectory dir;
    
    int nFiles = dir.listDir("gui/animation");
    if(nFiles) 
	{        
        for(int i=0; i<dir.numFiles(); i++) 
		{             
			ofImage* imgFrame = new ofImage();
			string filePath = dir.getPath(i);
			imgFrame->loadImage(filePath);
			images.push_back(imgFrame);
			imgFrame = NULL;  
        }      
    } 
    else printf("ERROR> Couldn't load animation frames!");

	//Масштаб
	this->scale = scale;
	
	//Framerate анимации
	sequenceFPS = 30.0;

	//Количество кадров
	frameAmount = images.size();

	//Изначально анимация не воспроизводится
	isComplete = true;
}

void echoRingAnimation::draw(int x, int y)
{
	if(!isComplete)
	{
		float totalTime = images.size() / sequenceFPS;
		frameIndex = 0;

		//Время
		currentFrameTime = ofGetElapsedTimef() - startFrameTime;

		//Текущий кадр анимации
		frameIndex = (int)(currentFrameTime * sequenceFPS) % images.size();
		//Проверка на завершенность
		if(frameIndex < prevFrameIndex) 
		{
			isComplete = true;
			return;
		}

		//Отрисовываем кадр анимации
		ofPushMatrix();
			ofTranslate(x, y);
			ofScale(scale,scale);
			ofEnableAlphaBlending();
			ofPushStyle();
			ofSetColor(255,255,255,128);
				//ofEnableBlendMode(OF_BLENDMODE_ADD);
				//ofEnableBlendMode(OF_BLENDMODE_SCREEN);
				ofEnableBlendMode(OF_BLENDMODE_ADD);
					images[frameIndex]->draw(-images[0]->getWidth()/2, -images[0]->getHeight()/2, images[0]->getWidth(), images[0]->getHeight());
				ofDisableBlendMode();
			ofPopStyle();
			ofDisableAlphaBlending();
		ofPopMatrix();

		prevFrameIndex = frameIndex;
	}
}

void echoRingAnimation::play()
{
	startFrameTime = ofGetElapsedTimef();
	isComplete = false;
	prevFrameIndex = 0;
}

bool echoRingAnimation::getCompleteStatus()
{
	return isComplete;
}