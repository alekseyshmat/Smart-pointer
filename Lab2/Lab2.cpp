#include "stdafx.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <process.h>
#include "SmartPointer.h"

using namespace SmartPointer;
using namespace std;

const int NUMBER_OF_ITERATIONS = 10;
const LPCWSTR PATH_TO_LOG_FILE = L"log.txt";
const LPCWSTR PATH_TO_FUNCTION_RESULT_FILE = L"test.txt";

SYSTEMTIME st;
DWORD dwBytesWritten;

HANDLE valuesFile, logFile, thread1, thread2, thread3, event1, event2, event3;

class Point {
	int x;
	int y;
	string startTime, endTime;

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

	string getStartTime() {
		return startTime;
	}

	string getEndTime() {
		return endTime;
	}

	void setX(int x) {
		this->x = x;
	}

	void setY(int y) {
		this->y = y;
	}

	void setStartTime(string startTime) {
		this->startTime = startTime;
	}

	void setEndTime(string endTime) {
		this->endTime = endTime;
	}
};

void QuadraticFunc(SafeSmartPointer<Point> &ptr) {
	for (int index = 0; index < NUMBER_OF_ITERATIONS; index++) {
		WaitForSingleObject(event1, INFINITE);
		cout << "Thread 1" << endl;
		int y = pow(index, 2);
		GetLocalTime(&st);
		string currentTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		string logFirstThreadMessage = currentTime + " [INFO] Data is calculate:  x =  " + to_string(index) +
			", y = " + to_string(y) + "\n";
		string startTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		WriteFile(logFile, logFirstThreadMessage.c_str(), logFirstThreadMessage.size(), &dwBytesWritten, NULL);
		ResetEvent(event1);
		SetEvent(event2);

		ptr->setX(index);
		ptr->setY(y);
		ptr->setStartTime(startTime);
	}
}

void WriteResultToFileFunc(SafeSmartPointer<Point> &ptr) {
	for (int index = 0; index < NUMBER_OF_ITERATIONS; index++) {
		WaitForSingleObject(event2, INFINITE);
		cout << "Thread 2" << endl;
		GetLocalTime(&st);
		string endTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		ResetEvent(event2);

		ptr->setEndTime(endTime);
		string valuesMessage = "x = " + to_string(ptr->getX()) + " y = " + to_string(ptr->getY()) + "\n";
		string logSecondThreadMessage = " [INFO] Data is written to file:  x =  " + to_string(ptr->getX()) +
			", y = " + to_string(ptr->getY()) + "\n";
		string currentTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds)) + logSecondThreadMessage;
		WriteFile(valuesFile, valuesMessage.c_str(), valuesMessage.size(), &dwBytesWritten, NULL);
		WriteFile(logFile, currentTime.c_str(), currentTime.size(), &dwBytesWritten, NULL);

		SetEvent(event3);
	}
}

void WriteToLogFileFunc(SafeSmartPointer<Point> &ptr) {
	for (int index = 0; index < NUMBER_OF_ITERATIONS; index++) {
		WaitForSingleObject(event3, INFINITE);
		cout << "Thread 3" << endl;
		GetLocalTime(&st);
		string currentTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		ResetEvent(event3);

		string log = currentTime + " [INFO] Start time counting x = " + to_string(ptr->getX()) +
			" y = " + to_string(ptr->getY()) + " : " + ptr->getStartTime() +
			"\n" + currentTime + " [INFO] End time writing " + ptr->getEndTime() + "\n";
		WriteFile(logFile, log.c_str(), log.size(), &dwBytesWritten, NULL);
		cout << "----------------------------------------" << endl;

		SetEvent(event1);
	}
}

int main() {
	Point* point = new Point(0, 0);
	SafeSmartPointer<Point> ptr(point);
	event1 = CreateEvent(NULL, TRUE, TRUE, TEXT("event1"));
	event2 = CreateEvent(NULL, TRUE, FALSE, TEXT("event2"));
	event3 = CreateEvent(NULL, TRUE, FALSE, TEXT("event3"));
	thread1 = (HANDLE)_beginthread((_beginthread_proc_type)QuadraticFunc, 0, (void*)&ptr);
	thread2 = (HANDLE)_beginthread((_beginthread_proc_type)WriteResultToFileFunc, 0, (void*)&ptr);
	thread3 = (HANDLE)_beginthread((_beginthread_proc_type)WriteToLogFileFunc, 0, (void*)&ptr);
	valuesFile = CreateFile(PATH_TO_FUNCTION_RESULT_FILE, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	logFile = CreateFile(PATH_TO_LOG_FILE, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	WaitForSingleObject(thread1, INFINITE);
	WaitForSingleObject(thread2, INFINITE);
	WaitForSingleObject(thread3, INFINITE);
	system("pause");
}
