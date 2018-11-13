#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include "SmartPointer.h"
#include <process.h>

using namespace SmartPointer;
using namespace std;

const int NUMBER_OF_ITERATIONS = 2;
const LPCWSTR PATH_TO_LOG_FILE = L"log.txt";
const LPCWSTR PATH_TO_FUNCTION_RESULT_FILE = L"test.txt";

SYSTEMTIME st;
DWORD dwBytesWritten;
HANDLE eventForFirstThread, eventForSecondThread, eventForThirdThread, valuesFile, logFile, thread1, thread2, thread3;

class Point {

public:
	int x;
	int y;
	char *h;
	string timeOfGettingY ;
	string timeOfWritingToLogFile;

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
		this->timeOfGettingY = time;
	}

	string getEndTime() {
		return timeOfWritingToLogFile;
	}

	void setEndTime(string time) {
		this->timeOfWritingToLogFile = time;
	}

	void setH(char *h) {
		this->h = h;
	}
	
	char getH() {
		return *h;
	}
};

void QuadraticFunc(SafeSmartPointer<Point> ptr) {
	for (int index = 0; index < NUMBER_OF_ITERATIONS; index++) {
		cout << "Thread 1" << endl;
		int x = index;
		int y = pow(x, 2);
		ptr->setX(x);
		ptr->setY(y);
		cout << x << " " << y << endl;
		GetLocalTime(&st);

		int h = (int(st.wHour));
		char h1 = (char)h;
		//ptr->setH(h1);

		string startTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));

		char *stTime = new char[startTime.size()+1];
		ptr->setH(stTime);

		cout << ptr->getH();
		string currentTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));

		string logFirstThreadMessage = currentTime + " [INFO] Data is calculate:  x =  " + to_string(ptr->getX()) +
			", y = " + to_string(ptr->getY()) + "\n";
		WriteFile(logFile, logFirstThreadMessage.c_str(), logFirstThreadMessage.size(), &dwBytesWritten, NULL);

		ptr->setStartTime(startTime);
		
		ResumeThread(thread2);

		if (index != NUMBER_OF_ITERATIONS) {
			SuspendThread(thread1);
		}
		
	}
	CloseHandle(thread1);
}

void WriteResultToFileFunc(SafeSmartPointer<Point> ptr) {
	for (int index = 0; index < NUMBER_OF_ITERATIONS; index++) {
		cout << "Thread 2" << endl;
		GetLocalTime(&st);
		string endTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		cout << " x = " << ptr->getX() << " y = " << ptr->getY() << endl;
		
		string currentTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));

		string valuesMessage = "x = " + to_string(ptr->getX()) + " y = " + to_string(ptr->getY()) + "\n";
		WriteFile(valuesFile, valuesMessage.c_str(), valuesMessage.size(), &dwBytesWritten, NULL);

		string logSecondThreadMessage = currentTime + " [INFO] Data is written to file:  x =  " + to_string(ptr->getX()) +
			", y = " + to_string(ptr->getY()) + "\n";
		WriteFile(logFile, logSecondThreadMessage.c_str(), logSecondThreadMessage.size(), &dwBytesWritten, NULL);
		
		ptr->setEndTime(endTime);
		
		ResumeThread(thread3);
		if (index != NUMBER_OF_ITERATIONS) {
			SuspendThread(thread2);
		}
	}
	CloseHandle(thread2);
}

void WriteToLogFileFunc(SafeSmartPointer<Point> ptr) {
	for (int index = 0; index < NUMBER_OF_ITERATIONS; index++) {
		cout << "Thread 3" << endl;
		cout << "START TIME: " << ptr->getStartTime() << endl;
		cout << "END TIME: " << ptr->getEndTime() << endl;
		GetLocalTime(&st);
		string currentTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		
		string log = currentTime + " [INFO] Start time " + ptr->getStartTime() +
			"\n" + currentTime + " [INFO] End time " + ptr->getEndTime() + "\n";
		WriteFile(logFile, log.c_str(), log.size(), &dwBytesWritten, NULL);
		cout << "----------------------------------------" << endl;
		
		ResumeThread(thread1);

		if (index != NUMBER_OF_ITERATIONS) {
			SuspendThread(thread3);
		}
	}
	CloseHandle(thread3);
}

int main() {
	SafeSmartPointer<Point> ptr(new Point());
	valuesFile = CreateFile(PATH_TO_FUNCTION_RESULT_FILE, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	logFile = CreateFile(PATH_TO_LOG_FILE, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	thread1 = (HANDLE)_beginthread((_beginthread_proc_type)QuadraticFunc, 0, (void*)&ptr);
	thread2 = (HANDLE)_beginthread((_beginthread_proc_type)WriteResultToFileFunc, 0, (void*)&ptr);
	thread3 = (HANDLE)_beginthread((_beginthread_proc_type)WriteToLogFileFunc, 0, (void*)&ptr);

	SuspendThread(thread2);
	SuspendThread(thread3);
	
	/* 
	thread th1(QuadraticFunc, ptr);
	thread th2(WriteResultToFileFunc, ptr);
	thread th3(WriteToLogFileFunc, ptr);
	th1.join();
	th2.join();
	th3.join();


	th1.~thread();
	th2.~thread();
	th3.~thread();
	*/
	Sleep(1500000);
	CloseHandle(valuesFile);
	CloseHandle(logFile);

	ptr.~SafeSmartPointer();
	system("pause");
}
