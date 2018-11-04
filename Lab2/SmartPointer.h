#pragma once

#ifndef SMART_POINTER_HEADER
#define SMART_POINTER_HEADER

#include <cstddef>
#include <mutex>

using namespace std;



	struct ref_count_data{
		unsigned int use_count;
	};

	class reference_count
	{
		public:
		reference_count()
		{
			ref_counts.use_count = 1;
		}

		void * get_shared_ref(void * pdata)
		{
			lock_guard< mutex > lock(ref_count_mutex);

			if (ref_counts.use_count){
				ref_counts.use_count++;
				return pdata;
			}else{
				return NULL;
			}
		}

		ref_count_data release_shared_ref(){
			lock_guard<mutex> lock(ref_count_mutex);
			ref_counts.use_count--;
			return ref_counts;
		}

		unsigned int get_use_count() const
		{
			lock_guard< mutex > lock(ref_count_mutex);
			return ref_counts.use_count;
		}

	private:
		mutable mutex ref_count_mutex;
		ref_count_data ref_counts;
	};

	template < typename T >
	class shared_pointer
	{
	public:
		shared_pointer() : pdata(NULL), rc(NULL) {} //default constructor

		shared_pointer(T* pvalue) : pdata(pvalue), rc(NULL){
			if (NULL != pdata){
				rc = new reference_count();
			}
		}

		//Copy constructor
		shared_pointer(const shared_pointer<T>& sp) : pdata(NULL), rc(sp.rc){
			if (NULL != rc){
				pdata = static_cast<T*>(rc->get_shared_ref(sp.pdata));

				if (NULL == pdata){
					rc = NULL;
				}
			}
		}

		~shared_pointer()
		{
			if (NULL != rc){
				ref_count_data updated_counts = rc->release_shared_ref();

				if (0 == updated_counts.use_count){
					delete pdata;
				}
			}
		}

	
		T& operator* () const{
			return *pdata;
		}

		T* operator-> () const{
			return pdata;
		}

		template <class Y, class Z> friend shared_pointer<Y> dynamic_pointer_cast(const shared_pointer<Z>& sp);
	
	private:
		T * pdata;            // pointer
		reference_count* rc; // Reference count

		//Copy constructor for dynamic_pointer_cast and weak_pointer lock
		shared_pointer(T* pvalue_arg, reference_count * rc_arg) : pdata(NULL), rc(rc_arg){
			if (NULL == pvalue_arg){
				rc = NULL;
			}else{
				pdata = static_cast<T*>(rc->get_shared_ref(pvalue_arg));
				if (NULL == pdata){
					rc = NULL;
				}
			}
		}
	};
	template <class T, class U> shared_pointer<T> dynamic_pointer_cast(const shared_pointer<U>& sp){
		return shared_pointer<T>(dynamic_cast<T*>(sp.pdata), sp.rc);
	}


#endif