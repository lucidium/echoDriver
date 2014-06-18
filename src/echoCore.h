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

		//Процедура подключения к виртуальному порту
		void connect(int portID, int baudRate = 9600);
		//Процедура подключения к первому готовому виртуальному порту
		void connectToFirstDevice(int baudRate = 9600);
		//Процедура разрыва активного соединения
		void disconnect();

		//Процедура установки величины затухания
		void setDecay(float decay);
		//Процедура установки величины мультипликатора ускорения
		void setFactor(float factor);
		//Процедура установки скорости линейной интерполяции
		void setLerpSpeed(float speed);
		//Процедура установки нижней границы засчитывания движений
		void setLowThreshold(int newLowThreshold);
		//Процедура установки верхней границы засчитывания движений
		void setUpThreshold(int newUpThreshold);

		//Функция возвращает список виртуальных портов с устройствами
		vector<ofSerialDeviceInfo> getDeviceList();
		//Функция возвращает сырое значение параметра X
		int getRawX();
		//Функция возвращает сырое значение параметра Y
		int getRawY();
		//Функция возвращает сырое значение параметра Z
		int getRawZ();
		//Функция возвращает значение параметра X после фильтрации
		int getFilteredX();
		//Функция возвращает значение параметра Y после фильтрации
		int getFilteredY();
		//Функция возвращает значение параметра Z после фильтрации
		int getFilteredZ();
		//Функция возвращает сглаженное по времени значение параметра X
		int getSmoothedX();
		//Функция возвращает сглаженное по времени значение параметра Y
		int getSmoothedY();
		//Функция возвращает сглаженное по времени значение параметра Z
		int getSmoothedZ();
		//Функция возвращает трехмерный вектор ускорения датчика
		ofVec3f getAcceleration();
		//Функция возвращает трехмерный вектор интерполированного линейного ускорения датчика
		ofVec3f getInterpolatedAcceleration();
		//Функция возвращает статус активности виртуального порта
		bool isConnected();
		//Функция возвращает статус нажатой кнопки №1
		bool isBtn1Pressed();
		//Функция возвращает статус нажатой кнопки №2
		bool isBtn2Pressed();
		//Функция возвращает статус нажатой кнопки №3
		bool isBtn3Pressed();
		//Функция возвращает статус нажатой кнопки №3
		bool isBtn4Pressed();

		//Параметры из gui
		coreData *userData;

	private:
		//Инициализация переменных
		void initializeVariables();

		//Процедура парсинга строки с виртуального порта
		//void parseString(char *rawData);
		void parseString(string rawData);

		//Процедура фильтрации
		void kalmanFiltering(int x, int y, int z);

		//Процедура вычисления ускорения
		void accelerationCalculation(int x, int y, int z);

		//Процедура вычисления мгновенного ускорения датчика путем линейной интерполяции мгновенных значений
		void lerpAccelerationCalculation();

		//Триггер активности соединения
		bool isConnectionEstablished;

		//Количество байтов доступных на виртуальном порте
		int bytesTotalAmount;
		int bytesDifference;

		//Чтение данных с виртуального COM
		string receivedString;

		//Вспомогательные переменные при парсинге строки
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

		//Сырые значения с акселерометра
		int rawX;
		int rawY;
		int rawZ;

		//Отфильтрованные значения с акселерометра
		int filteredX;
		int filteredY;
		int filteredZ;

		//Сглаженные значения с акселерометра
		int smoothedX;
		int smoothedY;
		int smoothedZ;

		//Ускорение
		ofVec3f acceleration;

		//Интерполированное ускорение
		ofVec3f lerpAcceleration;

		bool btn1Pressed;
		bool btn2Pressed;
		bool btn3Pressed;
		bool btn4Pressed;

		//Расчет ускорения
		int xLastCoord;
		int yLastCoord;
		int zLastCoord;
		int xCoord;
		int yCoord;
		int zCoord;
		int xVelocity;
		int yVelocity;
		int zVelocity;

		//Параметры
		float pDecay;
		float pFactor;
		float pLerpSpeed;
		int pLowThreshold;
		int pUpThreshold;


		//Список активных виртуальных портов
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