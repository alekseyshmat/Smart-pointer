#include <Windows.h>

namespace SmartPointer
{
	template <typename T>
	class SafeSmartPointer
	{
	private:
		T * data;
		int numberOfReferences;
		HANDLE mutex;
		const LPCWSTR mutexName = L"Mutex_Name";

		T* getPointer(T* data)
		{
			WaitForSingleObject(mutex, INFINITE);

			if (numberOfReferences == 0) {
				numberOfReferences++;
				Sleep(10);
				ReleaseMutex(mutex);
				return data;
			}
			else {
				ReleaseMutex(mutex);
				return NULL;
			}
		}

	public:
		SafeSmartPointer() : data(NULL), numberOfReferences(0), mutex(NULL) {}

		SafeSmartPointer(T* value) {
			numberOfReferences = 0;
			mutex = NULL;
			if (value != NULL) {
				mutex = CreateMutex(NULL, FALSE, mutexName);
				data = getPointer(value);
			}
		}

		T& operator* () {
			WaitForSingleObject(mutex, INFINITE);

			if (numberOfReferences == 1) {
				Sleep(10);
				ReleaseMutex(mutex);
				return *data;
			}
			ReleaseMutex(mutex);
			return NULL;
		}

		T* operator-> () {
			WaitForSingleObject(mutex, INFINITE);

			if (numberOfReferences == 1) {
				Sleep(10);
				ReleaseMutex(mutex);
				return data;
			}
			ReleaseMutex(mutex);
			return NULL;
		}

		~SafeSmartPointer() {
			delete data;
			ReleaseMutex(mutex);
			CloseHandle(mutex);
		}

	};
}