#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <Windows.h>


#include <string>
#include "SmartPointer.h"

using namespace SmartPointer;
using namespace std;

const int NUMBER_OF_ITERATIONS = 10;
const LPCWSTR PATH_TO_LOG_FILE = L"log.txt";
const LPCWSTR PATH_TO_FUNCTION_RESULT_FILE = L"test.txt";

SYSTEMTIME st;
ofstream fout;
ofstream logout;
HANDLE eventForFirstThread, eventForSecondThread, eventForThirdThread, valuesFile, logFile;

class Point {
	int x;
	int y;
	string timeOfGettingY;
	string timeOfWritingToLogFile;

public:
	Point() {}

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}


	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	void setX(int x) {
		this->x = x;
	}

	void setY(int y) {
		this->y = y;
	}

	string getStartTime() {
		return timeOfGettingY;
	}

	void setStartTime(string time) {
		timeOfGettingY = time;
	}

	string getEndTime() {
		return timeOfWritingToLogFile;
	}

	void setEndTime(string time) {
		timeOfWritingToLogFile = time;
	}
};


void QuadraticFunc(SafeSmartPointer<Point> ptr) {
	for (int index = 0; index < NUMBER_OF_ITERATIONS; index++) {
		WaitForSingleObject(eventForFirstThread, INFINITE);
		cout << "Thread 1" << endl;
		int x = index;
		int y = pow(x, 2);
		ptr->setX(x);
		ptr->setY(y);
		cout << x << " " << y << endl;
		GetLocalTime(&st);
		string startTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		ptr->setStartTime(startTime);
		ResetEvent(eventForFirstThread);
		SetEvent(eventForSecondThread);
	}
}

void WriteResultToFileFunc(SafeSmartPointer<Point> ptr) {
	for (int index = 0; index < NUMBER_OF_ITERATIONS; index++) {
		WaitForSingleObject(eventForSecondThread, INFINITE);
		cout << "Thread 2" << endl;
		GetLocalTime(&st);
		string endTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		cout << " x = " << ptr->getX() << " y = " << ptr->getY() << endl;
		DWORD dwBytesWritten;
	
		string values = "x = " + to_string(ptr->getX()) + "y = " + to_string(ptr->getY()) + "\n";

		WriteFile(valuesFile, values.c_str(), values.size(), &dwBytesWritten, NULL);

		//fout << " x = " << ptr->getX() << " y = " << ptr->getY() << endl;
		ptr->setEndTime(endTime);
		ResetEvent(eventForSecondThread);
		SetEvent(eventForThirdThread);
	}
}

void WriteToLogFileFunc(SafeSmartPointer<Point> ptr) {
	for (int index = 0; index < NUMBER_OF_ITERATIONS; index++) {
		WaitForSingleObject(eventForThirdThread, INFINITE);
		cout << "Thread 3" << endl;
		cout << "START TIME: " << ptr->getStartTime() << endl;
		cout << "END TIME: " << ptr->getEndTime() << endl;
		GetLocalTime(&st);
		string currentTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		DWORD dwBytesWritten;
		string log = currentTime + " [INFO] Start time " + ptr->getStartTime() +
			"\n" + currentTime + " [INFO] End time " + ptr->getEndTime() + "\n";
		WriteFile(logFile, log.c_str(), log.size(), &dwBytesWritten, NULL);
		cout << "----------------------------------------" << endl;
		ResetEvent(eventForThirdThread);
		SetEvent(eventForFirstThread);
	}
}

int main() {
	SafeSmartPointer<Point> ptr(new Point());

	//fout.open(PATH_TO_FUNCTION_RESULT_FILE, ios::trunc);
//	logout.open(PATH_TO_LOG_FILE, ios::trunc);
	valuesFile = CreateFile(PATH_TO_FUNCTION_RESULT_FILE, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	logFile = CreateFile(PATH_TO_LOG_FILE, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	eventForSecondThread = CreateEvent(NULL, TRUE, FALSE, TEXT("event1"));
	eventForThirdThread = CreateEvent(NULL, TRUE, FALSE, TEXT("event2"));
	eventForFirstThread = CreateEvent(NULL, TRUE, FALSE, TEXT("event3"));
	SetEvent(eventForFirstThread);
	thread th1(QuadraticFunc, ptr);
	thread th2(WriteResultToFileFunc, ptr);
	thread th3(WriteToLogFileFunc, ptr);
	th1.join();
	th2.join();
	th3.join();
	CloseHandle(eventForSecondThread);
	CloseHandle(eventForThirdThread);
	CloseHandle(eventForFirstThread);
	th1.~thread();
	th2.~thread();
	th3.~thread();
	ptr.~SafeSmartPointer();
	CloseHandle(valuesFile);
	CloseHandle(logFile);
	//fout.close();
	//logout.close();
	system("pause");
}
