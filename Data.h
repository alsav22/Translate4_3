#ifndef DATA_H
#define DATA_H
#include <QtGui>
#include <QtCore>
//#include <QChar>
//#include <QString>
//#include <QFile>
//#include <QDir>
//#include <QTextStream>
//#include <QMessagebox>
//#include <QDebug>
#include "GVariables.h"

#ifdef DEBUG
#include <ctime>
#endif

bool start(); // загрузка и обработка данных перед созданием виджета
template <typename T>
inline void partitioning(QVector <T>& v, T& container, size_t N); // делит контейнер на N частей (прибизительно равных ), и заполняет ими вектор с контейнерами

class Data /*:  public QObject*/
{
	//Q_OBJECT

public:
	Data()
	{ GlobalVariables::getGlobalVariables(); } // создание и инициализация глобальных переменных
	
	bool download();   // загрузка данных из файла
	bool processing(); // обработка загруженных данных
	
	void errorOpeningFile();
    void errorPathSound();
	void errorProcessing();
};

// класс для работы в потоках
template <typename T1, typename T2>
class Worker : public QRunnable
{
	T1 data;
	//void (*f)(const T&);
	T2& f;

public:
	explicit Worker(T1& d, T2& foo) : data(d), f(foo)
	{}
	
	virtual void run()
	{
		f(data);
	}
};

template <typename T>
void f(const T& data)
{

	QChar ch;
	QDir dir;
	QStringList listFiles;
	
	foreach (const QString nameDir, data)
	{
		dir = QDir(GlobalVariables::getGlobalVariables().PATH_SOUND + '/' + nameDir);
		listFiles = dir.entryList(QDir::Files);
		
		ch = nameDir[0]; // первая буква в имени папки
		//QMutexLocker locker(&GlobalVariables::getGlobalVariables().mutex);
		GlobalVariables::getGlobalVariables().mutex.lock();
		//qDebug() << this ->objectName();
		GlobalVariables::getGlobalVariables().ListsFilesFromDirs[ch] = listFiles; // список файлов из папки - в хеш.
		GlobalVariables::getGlobalVariables().mutex.unlock();
	}
}

#endif