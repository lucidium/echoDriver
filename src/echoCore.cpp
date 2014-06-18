#include "echoCore.h"

echoCore::echoCore()
{
	cout << "=== Echo Core v0.2b ===" << endl;

	//������������� ����������
	initializeVariables();

	//��������� ��������
	this->isConnectionEstablished = false;

	//������ ������� �������������
	kalmanX = new echoKalmanFiltering(2,2,1,1);
	kalmanY = new echoKalmanFiltering(2,2,1,1);
	kalmanZ = new echoKalmanFiltering(2,2,1,1);

	kalmanX->setState(-1,0.1);
	kalmanY->setState(-1,0.1);
	kalmanZ->setState(1,0.1);

	//����������� �� ������� �������������
	smoothX = new echoSmoothValue();
	smoothY = new echoSmoothValue();
	smoothZ = new echoSmoothValue();
	smoothX->setup(5);
	smoothY->setup(5);
	smoothZ->setup(5);
}

echoCore::~echoCore()
{
	delete kalmanX;
	delete kalmanY;
	delete kalmanZ;
	delete smoothX;
	delete smoothY;
	delete smoothZ;
}

void echoCore::initializeVariables()
{
	rawX = 0;
	rawY = 0;
	rawZ = 0;

	xLastCoord = 0;
	yLastCoord = 0;
	zLastCoord = 0;

	filteredX = 0;
	filteredY = 0;
	filteredZ = 0;

	smoothedX = 0;
	smoothedY = 0;
	smoothedZ = 0;

	acceleration = ofVec3f(0.0,0.0,0.0);
	lerpAcceleration = ofVec3f(0.0,0.0,0.0);

	bytesTotalAmount = 0;
	bytesDifference = 0;

	btn1Pressed = false;
	btn2Pressed = false;
	btn3Pressed = false;
	btn4Pressed = false;

	//��������� ������������� ����������
	pDecay = DECAY;
	pFactor = 300.0;
	pLerpSpeed = INTERPOLATION_SPEED;
	pLowThreshold = THRESHOLD;
	pUpThreshold = MAX_DIFF;

	userData = new coreData();

	tick = false;
}

void echoCore::connect(int portID, int baudRate)
{
	//���� ��� ����������, �� ����������� ����������
	if( this->isConnectionEstablished ) disconnect();
	//����������� �����������, ���� ���� ��������
	if( serial.setup(portID, baudRate) )
	{
		this->isConnectionEstablished = true;
	}
}

void echoCore::connectToFirstDevice(int baudRate)
{
	//���� ��� ����������, �� ����������� ����������
	if( this->isConnectionEstablished ) disconnect();
	//����������� �����������, ���� ���� ��������
	if( serial.setup(0, baudRate) )
	{
		this->isConnectionEstablished = true;
	}
}

void echoCore::disconnect()
{
	if(isConnectionEstablished)
	{
		this->isConnectionEstablished = false;
		serial.close();
	}
}

void echoCore::update()
{
	if(isConnectionEstablished)
	{
		//�������� ������
		string str;
		do 
		{
			str = ofxGetSerialString(serial,'#');
			if (str == "")
			{
				serial.writeByte('1');
				continue;
			}
			receivedString = str;
		} while (str!="");

		//������ ���������� ������
		parseString(receivedString);

		//���������� ��������
		kalmanFiltering(rawX,rawY,rawZ);

		//������ ���������
		accelerationCalculation(smoothedX,smoothedY,smoothedZ);

		//������ ����������� ���������
		lerpAccelerationCalculation();
	}
}

void echoCore::parseString(string rawData)
{
	//������ ������� ���������� ����� ��������
	//�������� ������ �������� � ����������� �������
	length = rawData.length();

	if(length > STRING_LENGTH)
	{
		//��������� ������
		rawStr = rawData;
		//������� ������� ��� ����������
		posX = rawStr.find("x");
		posY = rawStr.find("y");
		posZ = rawStr.find("z");
		posB = rawStr.find("b");
		//��������� ������� � ������ �� ����������
		if(posX != std::string::npos && posY != std::string::npos && posZ != std::string::npos && posB != std::string::npos)
		{
			//������ ������� ��������
			xValue = rawStr.substr(posX + DATA_OFFSET,posY - (posX + DATA_OFFSET));
			yValue = rawStr.substr(posY + DATA_OFFSET,posZ - (posY + DATA_OFFSET));
			if(posZ + DATA_OFFSET > length) zValue = "0";
			else zValue = rawStr.substr(posZ + DATA_OFFSET,posB - (posZ + DATA_OFFSET));

			//��������� ������� ������
			if(rawStr.substr(posB + DATA_OFFSET,1) == "0") btn1Pressed = false;
			else btn1Pressed = true;
			if(rawStr.substr(posB + DATA_OFFSET + 1,1) == "0") btn2Pressed = false;
			else btn2Pressed = true;
			if(rawStr.substr(posB + DATA_OFFSET + 2,1) == "0") btn3Pressed = false;
			else btn3Pressed = true;
			if(rawStr.substr(posB + DATA_OFFSET + 3,1) == "0") btn4Pressed = false;
			else btn4Pressed = true;

			//��������� � integer
			rawX = atoi(xValue.c_str());
			rawY = atoi(yValue.c_str());
			rawZ = atoi(zValue.c_str());
		}
	}
}

void echoCore::kalmanFiltering(int x, int y, int z)
{
	//������������ ��������
	kalmanX->correct(x);
	kalmanY->correct(y);
	kalmanZ->correct(z);

	//��������� ������
	filteredX = kalmanX->getState();
	filteredY = kalmanY->getState();
	filteredZ = kalmanZ->getState();

	//���������� �� �������
	smoothedX = smoothX->getSmoothValue(filteredX);
	smoothedY = smoothY->getSmoothValue(filteredY);
	smoothedZ = smoothZ->getSmoothValue(filteredZ);
}

void echoCore::accelerationCalculation(int x, int y, int z)
{
	//��������� ��������� �����������
	xCoord = x;
	yCoord = y;
	zCoord = z;

	xVelocity = xCoord - xLastCoord;
	yVelocity = yCoord - yLastCoord;
	zVelocity = zCoord - zLastCoord;

	//�������� ��������� �������� � ���������� ���������

	if(xVelocity < -userData->lowThreshold || xVelocity > userData->lowThreshold) acceleration.x = ofMap(xVelocity,-userData->highThreshold,userData->highThreshold,-1,1);
	else acceleration.x = 0.0;

	if(yVelocity < -userData->lowThreshold || yVelocity > userData->lowThreshold) acceleration.y = ofMap(yVelocity,-userData->highThreshold,userData->highThreshold,-1,1);
	else acceleration.y = 0.0;

	if(zVelocity < -userData->lowThreshold || zVelocity > userData->lowThreshold) acceleration.z = ofMap(zVelocity,-userData->highThreshold,userData->highThreshold,-1,1);
	else acceleration.z = 0.0;

	//�������� �������������
	if(userData->isXInvert) acceleration.x *= -userData->gainX;
	else acceleration.x *= userData->gainX;

	if(userData->isYInvert) acceleration.y *= -userData->gainY;
	else acceleration.y *= userData->gainY;

	if(userData->isZInvert) acceleration.z *= -userData->gainZ;
	else acceleration.z *= -userData->gainZ;
	
	//����� ��������
	acceleration *= userData->generalFactor;

	//���������� ��������� ����������
	xLastCoord = xCoord;
	yLastCoord = yCoord;
	zLastCoord = zCoord;
}

void echoCore::lerpAccelerationCalculation()
{
	//�������� ������������ ���������� ��������� - ������� ����������� ��������� 
	lerpAcceleration.x = ofLerp(lerpAcceleration.x,acceleration.x,userData->lerpSpeed);
	lerpAcceleration.y = ofLerp(lerpAcceleration.y,acceleration.y,userData->lerpSpeed);
	lerpAcceleration.z = ofLerp(lerpAcceleration.z,acceleration.z,userData->lerpSpeed);

	//���������
	lerpAcceleration *= userData->decay;
}

vector<ofSerialDeviceInfo> echoCore::getDeviceList()
{
	serial.listDevices();
	deviceList = serial.getDeviceList();
	return deviceList;
}

bool echoCore::isConnected()
{
	return isConnectionEstablished;
}

int echoCore::getRawX()
{
	if(isConnectionEstablished) return rawX;
	else return 0;
}

int echoCore::getRawY()
{
	if(isConnectionEstablished) return rawY;
	else return 0;
}

int echoCore::getRawZ()
{
	if(isConnectionEstablished) return rawZ;
	else return 0;
}

int echoCore::getFilteredX()
{
	return filteredX;
}

int echoCore::getFilteredY()
{
	return filteredY;
}

int echoCore::getFilteredZ()
{
	return filteredZ;
}

int echoCore::getSmoothedX()
{
	return smoothedX;
}

int echoCore::getSmoothedY()
{
	return smoothedY;
}

int echoCore::getSmoothedZ()
{
	return smoothedZ;
}

ofVec3f echoCore::getAcceleration()
{
	return acceleration;
}

ofVec3f echoCore::getInterpolatedAcceleration()
{
	return lerpAcceleration;
}

void echoCore::setDecay(float decay)
{
	this->pDecay = decay;
}

void echoCore::setFactor(float newfactor)
{
	this->pFactor = newfactor;
}

void echoCore::setLerpSpeed(float speed)
{
	this->pLerpSpeed = speed;
}

void echoCore::setLowThreshold(int newLowThreshold)
{
	this->pLowThreshold = newLowThreshold;
}

void echoCore::setUpThreshold(int newUpThreshold)
{
	this->pUpThreshold = newUpThreshold;
}

bool echoCore::isBtn1Pressed()
{
	return btn1Pressed;
}

bool echoCore::isBtn2Pressed()
{
	return btn2Pressed;
}

bool echoCore::isBtn3Pressed()
{
	return btn3Pressed;
}

bool echoCore::isBtn4Pressed()
{
	return btn4Pressed;
}
