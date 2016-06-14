#ifndef MY_THREAD_H
#define MY_THREAD_H
#include <QThread>
#include <QStringList>
#include <QVector>
#include "Data.h"

template <typename T1, typename T2>
class MyThread : public QThread
{
    //Worker <QStringList> worker;
	const T1 d;
	T2 f;
	
public:
    explicit MyThread(const T1& dirs, T2 foo/*void (*foo)(const T&)*/) : d(dirs), f(foo)
	{}

	void run()
	{
		//Worker <QStringList>* pWorker = new Worker <QStringList>(d);
		Worker <T1, T2>* pWorker = new Worker <T1, T2>(d, f);
		pWorker ->run();
		delete pWorker;
	}
};

template <typename T1, typename T2>
inline void multiMythreading(QVector <T1>& v, QVector <MyThread <T1, T2>*>& vp, T2 foo) // многопоточная обработка элементов вектора через QThread
{
	foreach(T1 e, v)
	{
		
		MyThread <T1, T2>* thread = new MyThread <T1, T2>(e, foo);
		thread ->start();
		vp.push_back(thread);
		
		/*MyThread <T>* thread = new MyThread <T>(e, foo);
		thread ->start();
		vp.push_back(thread);*/
	}
}

template <typename T1, typename T2>
inline void multithreading(QVector <T1>& v, T2& foo) // многопоточная обработка элементов вектора через QThreadPool
{
	foreach(T1 e, v)
	{
		//Worker<T>* worker = new Worker<T>(e);
		Worker<T1, T2>* worker = new Worker<T1, T2>(e, foo);
		QThreadPool::globalInstance() ->start(worker);
	}
	//QThreadPool::globalInstance() ->waitForDone();
}

#endif