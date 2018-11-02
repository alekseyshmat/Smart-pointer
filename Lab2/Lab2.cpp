#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <csignal>
#include <memory>
#include <string>
#include <mutex> 
#include "SmartPointer.h"


using namespace std;

CONST int COUNT_OF_NUMBERS = 10;
CONST string PATH_LOG = "log.txt";
CONST string PATH_FUNCTION_RESULT = "test.txt";

SYSTEMTIME st;
ofstream fout;
ofstream logout;


class Point{
		int x;
		int y;
		string startTime;
		string endTime;

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
		return startTime;
	}

	void setStartTime(string time) {
		startTime = time;
	}

	string getEndTime() {
		return endTime;
	}

	void setEndTime(string time) {
		endTime = time;
	}	
};


typedef my_experimental::shared_pointer<Point> shared;

//my_experimental::shared_pointer<Point> shared_p(new Point);
//shared = shared_p;
shared ptr(new Point);


void Thread1(LPVOID t) {
	int y;
	int x;
	HANDLE mutex = OpenMutex(SYNCHRONIZE, false, L"Mutex");
		for (int i = 0; i < COUNT_OF_NUMBERS; i++) {
			WaitForSingleObject(mutex, INFINITE);
			cout << "Thread 1" << endl;
			x = i;
			y = pow(i, 2);
			GetLocalTime(&st);
			string startTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
				+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));

			ptr->setX(x);
			ptr->setY(y);
			ptr->setStartTime(startTime);
			cout << x << " " << y << endl;	
			Sleep(150);//165
			ReleaseMutex(mutex);
		}
		CloseHandle(mutex);
}

void Thread2() {
	HANDLE mutex = OpenMutex(SYNCHRONIZE, false, L"Mutex");
	for (int i = 0; i < COUNT_OF_NUMBERS; i++) {
		Sleep(30);
		WaitForSingleObject(mutex, INFINITE);
		cout << "Thread 2" << endl;
		GetLocalTime(&st);
		fout << " x = " << ptr->getX() << " y = " << ptr->getY() << endl;
		cout << " x = " << ptr->getX() << " y = " << ptr->getY() << endl;
		string endTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":" 
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		ptr->setEndTime(endTime);
		ReleaseMutex(mutex);
		Sleep(50);
	}
	CloseHandle(mutex);
}

void Thread3() {
	HANDLE mutex = OpenMutex(SYNCHRONIZE, false, L"Mutex");
	for (int i = 0; i < COUNT_OF_NUMBERS; i++) {
		Sleep(70);  //65
		string currentTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		GetLocalTime(&st);
		WaitForSingleObject(mutex, INFINITE);
		cout << "Thread 3" << endl;
		logout << currentTime << " [INFO] Start time " << ptr->getStartTime() << endl;
		logout << currentTime << " [INFO] End time " << ptr->getEndTime() << endl;
		cout << "START TIME: " << ptr->getStartTime() << endl;
		cout << "END TIME: " << ptr->getEndTime() << endl;
		cout << "----------------------------------------" << endl;
		//ptr->
		ReleaseMutex(mutex);
		Sleep(30);
	}
	CloseHandle(mutex);
}

int main(){

	Point point;
	HANDLE hMutex, th1, th2, th3;
	if ((hMutex = CreateMutex(NULL, false, L"Mutex")) == NULL) GetLastError();
	fout.open(PATH_FUNCTION_RESULT, ios::trunc);
	logout.open(PATH_LOG, ios::trunc);
	if ((th1 = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)Thread1,
		NULL,
		0,
		NULL)) == NULL) return 0;
	if ((th2 = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)Thread2,
		NULL,
		0,
		NULL)) == NULL) return 0;
	if ((th3 = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)Thread3,
		NULL,
		0,
		NULL)) == NULL) return 0;


	WaitForSingleObject(th1, INFINITE);
	WaitForSingleObject(th2, INFINITE);
	WaitForSingleObject(th3, INFINITE);

	CloseHandle(th1);
	CloseHandle(th2);
	CloseHandle(th3);

	fout.close();
	logout.close();
	system("pause");
}
