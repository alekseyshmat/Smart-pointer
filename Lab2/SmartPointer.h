#include <Windows.h>

using namespace std;

namespace SmartPointer
{
	struct ref_count_data {
		unsigned int use_count;
	};

	class reference_count
	{
	private:
		ref_count_data ref_counts;
		mutable CRITICAL_SECTION  critical_section;

	public:
		reference_count()
		{
			ref_counts.use_count = 1;
			InitializeCriticalSection(&critical_section);
		}

		void* get_shared_ref(void* pdata)
		{
			EnterCriticalSection(&critical_section);

			if (ref_counts.use_count) {
				ref_counts.use_count++;
				LeaveCriticalSection(&critical_section);
				return pdata;
			}
			else {
				LeaveCriticalSection(&critical_section);
				return NULL;
			}
		}

		ref_count_data release_shared_ref() {
			EnterCriticalSection(&critical_section);
			

			ref_counts.use_count--;
			LeaveCriticalSection(&critical_section);
			//DeleteCriticalSection(&critical_section);
			return ref_counts;
		}
	};

	template <typename T>
	class SafeSmartPointer
	{
	private:
		T * pdata;            
		reference_count* rc; 

	public:
		SafeSmartPointer() : pdata(NULL), rc(NULL) {} 

		SafeSmartPointer(T* pvalue) : pdata(pvalue), rc(NULL) {
			if (NULL != pdata) {
				rc = new reference_count();
			}
		}

		SafeSmartPointer(const SafeSmartPointer<T>& sp) : pdata(NULL), rc(sp.rc) {
			if (NULL != rc) {
				pdata = static_cast<T*>(rc->get_shared_ref(sp.pdata));

				if (NULL == pdata) {
					rc = NULL;
				}
			}
		}

		~SafeSmartPointer()
		{
			if (NULL != rc) {
				ref_count_data updated_counts = rc->release_shared_ref();

				if (0 == updated_counts.use_count) {
					delete pdata;
				}
			}
		}

		T& operator* () const {
			return *pdata;
		}

		T* operator-> () const {
			return pdata;
		}

	};
}