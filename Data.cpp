#include "GVariables.h"
#include "Data.h"
#include "MyThread.h"

QMutex mutex;

bool start()
{
#ifdef DEBUG
	clock_t t1 = clock();
#endif
	Data data;
    if (data.download())
	{
		//QSplashScreen splash(QPixmap("5.png"));
		//splash.show();
		
		if (data.loadHash())
			return true;
		
		if (data.processing() && data.saveHash())
		{
#ifdef DEBUG
clock_t t2 = clock();
qDebug() << "Data preparation " << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
			return true;
		}
#ifdef DEBUG
clock_t t2 = clock();
qDebug() << "Data preparation " << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
		data.errorProcessing();
        return false;
	}
	return false;
}

bool Data::download()
{
	QFile file(GlobalVariables::getGlobalVariables().FILE_NAME);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		GlobalVariables::getGlobalVariables().PATH_SOUND = stream.readLine();
		file.close();
	//qDebug() << PATH_SOUND;
		if (!GlobalVariables::getGlobalVariables().PATH_SOUND.isEmpty()      &&
			QDir(GlobalVariables::getGlobalVariables().PATH_SOUND).exists())
		{
			return true;
		}
		else
		{
			GlobalVariables::getGlobalVariables().PATH_SOUND = "";
			errorPathSound();
			return false;
		}
	}
	else
	{
	    errorOpeningFile();
		return false;
	}
}

bool Data::saveHash()
{
	QFile fileHash("Hash.dat");
	if (!fileHash.open(QIODevice::WriteOnly))
		return false;
	QDataStream streamSave(&fileHash);
	streamSave << GlobalVariables::getGlobalVariables().ListsFilesFromDirs;

	return true;
}

bool Data::loadHash()
{
	if (!QFile::exists("Hash.dat"))
		return false;

	QFile fileHash("Hash.dat");
	if (!fileHash.open(QIODevice::ReadOnly))
		return false;
	QDataStream streamLoad(&fileHash);
	streamLoad >> GlobalVariables::getGlobalVariables().ListsFilesFromDirs;

	return true;
}

//void input(QStringList& dirs)
//{
//	size_t n = GlobalVariables::getGlobalVariables().ListsFilesFromDirs.size();
//	qDebug() << "Size hesh = " << n;
//	dirs.sort();
//	for (size_t i = 0; i < n; ++i)
//	{
//		qDebug() << GlobalVariables::getGlobalVariables().ListsFilesFromDirs[dirs[i][0]].size();
//	}
//}


// ����� ��������� �� N ������ (������������� ������ ), � ��������� ��� ������ � ������������
template <typename T>
inline void partitioning(QVector <T>& v, T& container, size_t N)
{
	size_t size = container.size();
	size_t part = size / N;
	size_t tail = size % N;

	size_t m = 0;
	while (size)
	{
		T temp;
		for (size_t j = 0; j < part && size > 0; ++j)
		{
			temp << container[m];
			++m; --size;
		}
		if (size > 0 && tail > 0)
		{
			temp << container[m];
			++m; --size; --tail;
		}
		v.push_back(temp);
	}
}

// ��������� ����������� ������
bool Data::processing()
{
	// ������ ����� ��� ./ � ../
	QStringList dirs(QDir(GlobalVariables::getGlobalVariables().PATH_SOUND).entryList(QDir::Dirs | QDir::NoDotAndDotDot)); 
	if (dirs.isEmpty())
		return false;
	
	int N = QThread::idealThreadCount(); // 2
	
	// ���������� dirs ������� �� N ������,
	// ������ ����� �������������� � ��������� ������
	QVector <QStringList> vDirs; // ������ ��� ������ dirs
	vDirs.reserve(N);
	partitioning(vDirs, dirs, N);
#ifdef DEBUG
	foreach(QStringList s, vDirs)
		qDebug() << s;
#endif
	
//#define MYTHREAD
#ifndef MYTHREAD
	void (*pf)(const QStringList&) = f;
    
	multithreading(vDirs, pf);
	QThreadPool::globalInstance() ->waitForDone();
#else	
	QVector <MyThread <QStringList, void (*)(const QStringList&)>* > vp;
	vp.reserve(N);
	void (*pf)(const QStringList&) = f;
	
	multiMythreading(vDirs, vp, pf);
	
	typedef MyThread <QStringList, void (*)(const QStringList&)>* pMyThr;
	foreach(pMyThr p, vp) // ��� auto �������
	{
		p ->wait();
		//qDebug() << p ->isFinished();
		delete p;
	}


#endif
	
	return true;
}

void Data::errorOpeningFile()
{
	QMessageBox error;
		
	error.setIcon(QMessageBox::Critical);
	error.setWindowTitle(QWidget::tr("������!"));
	//error.setGeometry(300, 300, 600, 200); // ������ �� �������� (?)
	error.setText(QWidget::tr("������ ��� �������� Path_Sound.txt,\n"
			                    "� ������� �������� ���� � �����\n"
								"�� ��������� �������!"));
	error.exec();
}

void Data::errorPathSound()
{
	QMessageBox error;
		
	error.setIcon(QMessageBox::Critical);
	error.setWindowTitle(QWidget::tr("������!"));
	error.setText(QWidget::tr("�� ������� ����� �� ��������� �������!\n"
		                        "��������� ���� � ����� � ����� Path_Sound.txt"));
	error.exec();
}

void Data::errorProcessing()
{
	QMessageBox error;
		
	error.setIcon(QMessageBox::Critical);
	error.setWindowTitle(QWidget::tr("������!"));
	//error.setGeometry(300, 300, 600, 200); // ������ �� �������� (?)
	error.setText(QWidget::tr("������ ��� ��������� ����������� ������!"));
			                    
	error.exec();
}

