
#include "stdafx.h"
/*#include <iostream>
	
class Point {
	int x;
	double y;

public:
	Point(int x, double y) {
		this->x = x;
		this->y = y;
	}
	void show() {
		std::cout << "Coords x: " << x << "\t y: " << y << std::endl;
	}

};


DWORD WINAPI createThread3(LPVOID t)
{
	Point *ptr = (Point*)t;

	//unique_ptr<Point> unq(new Point);
	//unq.get_deleter();

	logout << "[INFO] Start time " << ptr->getStartTime() << endl;
	logout << "[INFO] End time " << ptr->getEndTime() << endl;


	cout << " THread 3 " << endl;
	cout << "Start time: " << ptr->getStartTime() << endl;
	cout << "End time: " << ptr->getEndTime() << endl;
	cout << "----------------------------------" << endl;

	return 0;
}

DWORD WINAPI createThread2(LPVOID t)
{
	vector<int*> *values = (vector<int*>*)t;
	int x = *(*values).at(0);
	int y = *(*values).at(1);
	

	Point *ptr = (Point*)t;

	GetLocalTime(&st);
	fout << " x = " << ptr->getX() << " y = " << ptr->getY() << endl;


	string endTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":" + to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));


	
	cout << "THREAD 2\n";
	cout << " x = " << ptr->getX() << " y = " << ptr->getY() << endl;
	ptr->setEndTime(endTime);

	cout << "START TIME: " << ptr->getStartTime() << endl;
	cout << "END TIME: " << ptr->getEndTime() << endl;
	cout << "----------------------------------" << endl;



	thread3 = CreateThread(NULL, 0, createThread3, (LPVOID)t, 0, NULL);
	WaitForSingleObject(thread3, INFINITE);
	return 0;
}


void function1() {
	int *y;
	int *x;
	int yT;
	int xT;

	vector<int*> name;
	vector<int*> *values;
	values = &name;

	for (int i = 0; i < 10; i++) {
		x = &i;

		yT = pow(*x, 2);
		y = &yT;
		GetLocalTime(&st);
		string startTime = to_string(int(st.wHour)) + ":" + to_string(int(st.wMinute)) + ":" + to_string(int(st.wSecond)) + ":" + to_string(int(st.wMilliseconds));

		(*values).push_back(x);
		(*values).push_back(y);

		Point point;
		
		shared_ptr<Point> pt(new Point());
		pt->setX(i);
		pt->setY(yT);
		pt->setStartTime(startTime);
		

		point.setX(i);
		point.setY(yT);
		point.setStartTime(startTime);

		cout << "Thread 1 " << endl;
		cout << " x = " << *x << " y = " << *y << endl;
		cout << "----------------------------------" << endl;
	
		Point *ptr = &point;
		//Point *ptr = &pt;


		thread2 = CreateThread(NULL, 0, createThread2, (LPVOID)ptr, 0, NULL);
		WaitForSingleObject(thread2, INFINITE);
	}
}
*/