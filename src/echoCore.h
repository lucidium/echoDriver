#pragma once

#define STRING_LENGTH 20
#define DATA_OFFSET 2
#define MAX_DIFF 214
#define THRESHOLD 5
#define INTERPOLATION_SPEED 0.02
#define DECAY 0.95

#include "ofMain.h"
#include <string>
#include "ofxExtras.h"
#include "echoKalmanFiltering.h"

extern struct coreData{
	float gainX;
	float gainY;
	float gainZ;
	float generalFactor;
	float lerpSpeed;
	float decay;

	bool isAxisLocked;
	bool isXInvert;
	bool isYInvert;
	bool isZInvert;

	int lowThreshold;
	int highThreshold;
};

class echoCore{

	public:
		echoCore();
		~echoCore();

		void update();

		//��������� ����������� � ������������ �����
		void connect(int portID, int baudRate = 9600);
		//��������� ����������� � ������� �������� ������������ �����
		void connectToFirstDevice(int baudRate = 9600);
		//��������� ������� ��������� ����������
		void disconnect();

		//��������� ��������� �������� ���������
		void setDecay(float decay);
		//��������� ��������� �������� ��������������� ���������
		void setFactor(float factor);
		//��������� ��������� �������� �������� ������������
		void setLerpSpeed(float speed);
		//��������� ��������� ������ ������� ������������ ��������
		void setLowThreshold(int newLowThreshold);
		//��������� ��������� ������� ������� ������������ ��������
		void setUpThreshold(int newUpThreshold);

		//������� ���������� ������ ����������� ������ � ������������
		vector<ofSerialDeviceInfo> getDeviceList();
		//������� ���������� ����� �������� ��������� X
		int getRawX();
		//������� ���������� ����� �������� ��������� Y
		int getRawY();
		//������� ���������� ����� �������� ��������� Z
		int getRawZ();
		//������� ���������� �������� ��������� X ����� ����������
		int getFilteredX();
		//������� ���������� �������� ��������� Y ����� ����������
		int getFilteredY();
		//������� ���������� �������� ��������� Z ����� ����������
		int getFilteredZ();
		//������� ���������� ���������� �� ������� �������� ��������� X
		int getSmoothedX();
		//������� ���������� ���������� �� ������� �������� ��������� Y
		int getSmoothedY();
		//������� ���������� ���������� �� ������� �������� ��������� Z
		int getSmoothedZ();
		//������� ���������� ���������� ������ ��������� �������
		ofVec3f getAcceleration();
		//������� ���������� ���������� ������ ������������������ ��������� ��������� �������
		ofVec3f getInterpolatedAcceleration();
		//������� ���������� ������ ���������� ������������ �����
		bool isConnected();
		//������� ���������� ������ ������� ������ �1
		bool isBtn1Pressed();
		//������� ���������� ������ ������� ������ �2
		bool isBtn2Pressed();
		//������� ���������� ������ ������� ������ �3
		bool isBtn3Pressed();
		//������� ���������� ������ ������� ������ �3
		bool isBtn4Pressed();

		//��������� �� gui
		coreData *userData;

	private:
		//������������� ����������
		void initializeVariables();

		//��������� �������� ������ � ������������ �����
		//void parseString(char *rawData);
		void parseString(string rawData);

		//��������� ����������
		void kalmanFiltering(int x, int y, int z);

		//��������� ���������� ���������
		void accelerationCalculation(int x, int y, int z);

		//��������� ���������� ����������� ��������� ������� ����� �������� ������������ ���������� ��������
		void lerpAccelerationCalculation();

		//������� ���������� ����������
		bool isConnectionEstablished;

		//���������� ������ ��������� �� ����������� �����
		int bytesTotalAmount;
		int bytesDifference;

		//������ ������ � ������������ COM
		string receivedString;

		//��������������� ���������� ��� �������� ������
		string rawStr;

		string xValue;
		string yValue;
		string zValue;

		size_t posX;
		size_t posY;
		size_t posZ;
		size_t posB;
		size_t posBegin;
		size_t posEnd;

		int length;

		//����� �������� � �������������
		int rawX;
		int rawY;
		int rawZ;

		//��������������� �������� � �������������
		int filteredX;
		int filteredY;
		int filteredZ;

		//���������� �������� � �������������
		int smoothedX;
		int smoothedY;
		int smoothedZ;

		//���������
		ofVec3f acceleration;

		//����������������� ���������
		ofVec3f lerpAcceleration;

		bool btn1Pressed;
		bool btn2Pressed;
		bool btn3Pressed;
		bool btn4Pressed;

		//������ ���������
		int xLastCoord;
		int yLastCoord;
		int zLastCoord;
		int xCoord;
		int yCoord;
		int zCoord;
		int xVelocity;
		int yVelocity;
		int zVelocity;

		//���������
		float pDecay;
		float pFactor;
		float pLerpSpeed;
		int pLowThreshold;
		int pUpThreshold;


		//������ �������� ����������� ������
		vector<ofSerialDeviceInfo> deviceList;

		ofSerial serial;

		echoKalmanFiltering* kalmanX;
		echoKalmanFiltering* kalmanY;
		echoKalmanFiltering* kalmanZ;

		echoSmoothValue* smoothX;
		echoSmoothValue* smoothY;
		echoSmoothValue* smoothZ;

		bool tick;
};