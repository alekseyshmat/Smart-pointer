#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <csignal>
#include <memory>
#include <string>
#include <mutex> 
#include <process.h>  
#include "SmartPointer.h"


using namespace std;

CONST int COUNT_OF_NUMBERS = 6;
CONST string PATH_LOG = "log.txt";
CONST string PATH_FUNCTION_RESULT = "test.txt";

SYSTEMTIME st;
ofstream fout;
ofstream logout;
HANDLE hMutex, th1, th2, th3, event1, event2, event3, closeEvent;

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


//typedef my_experimental::shared_pointer<Point> shared;


//shared = shared_p;

//shared ptr;

void Thread1(shared_pointer<Point> ptr) {
	
	int y;
	int x;
	
		for (int i = 0; i < COUNT_OF_NUMBERS; i++) {
			WaitForSingleObject(event1, INFINITE);
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
			//Sleep(50);
			SetEvent(event2);
		}
		
}

void Thread2(shared_pointer<Point> ptr) {	
		for (int i = 0; i < COUNT_OF_NUMBERS; i++) {
			Sleep(30);
			WaitForSingleObject(event2, INFINITE);
			cout << "Thread 2" << endl;
			GetLocalTime(&st);
			fout << " x = " << ptr->getX() << " y = " << ptr->getY() << endl;
			cout << " x = " << ptr->getX() << " y = " << ptr->getY() << endl;
			string endTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
				+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
			ptr->setEndTime(endTime);
			//Sleep(100);
			SetEvent(event3);
		}
		
}

void Thread3(shared_pointer<Point> ptr) {
	for (int i = 0; i < COUNT_OF_NUMBERS; i++) {
		Sleep(200);
		WaitForSingleObject(event3, INFINITE);
		string currentTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":"
			+ to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));
		GetLocalTime(&st);
		cout << "Thread 3" << endl;
		logout << currentTime << " [INFO] Start time " << ptr->getStartTime() << endl;
		logout << currentTime << " [INFO] End time " << ptr->getEndTime() << endl;
		cout << "START TIME: " << ptr->getStartTime() << endl;
		cout << "END TIME: " << ptr->getEndTime() << endl;
		cout << "----------------------------------------" << endl;
		//ptr.~shared_pointer();
		SetEvent(event1);
	}
}

int main(){

	//shared_pointer<Point> ptr(new Point);
	
	fout.open(PATH_FUNCTION_RESULT, ios::trunc);
	logout.open(PATH_LOG, ios::trunc);
	shared_pointer<Point> ptr(new Point);
	event1 = CreateEvent(NULL, FALSE, TRUE, TEXT("event1"));
	event2 = CreateEvent(NULL, FALSE, TRUE, TEXT("event2"));
	event3 = CreateEvent(NULL, FALSE, TRUE, TEXT("event3"));
	thread thr1(Thread1, ref(ptr));
	thread thr2(Thread2, ref(ptr));
	thread thr3(Thread3, ref(ptr));
	thr1.join();
	thr2.join();
	thr3.join();

	



	/* 
	thread thr1(Thread1, ref(ptr));
	thread thr2(Thread2, ref(ptr));
	
	thr1.join();
	thr2.join();
	thr3.join();

	thr1.~thread();
	thr2.~thread();
	thr3.~thread();
	*/
/*
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
		&ptr,
		0,
		NULL)) == NULL) return 0;
	if ((th3 = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)Thread3,
		&ptr,
		0,
		NULL)) == NULL) return 0;
	
	
	WaitForSingleObject(th1, INFINITE);
	WaitForSingleObject(th2, INFINITE);
	WaitForSingleObject(th3, INFINITE);

	CloseHandle(th1);
	CloseHandle(th2);
	CloseHandle(th3);
	*/
	//CloseHandle(hMutex);
	fout.close();
	logout.close();
	Sleep(3000);
	system("pause");
}
