#include <QObject>
#include <QDir.h>
#include <QDebug>
#include <QRegExp>
#include <phonon>
#include "translate.h"
#include "MyPlayer.h"
#include "SoundFile.h"
//#include "Storage.h"
#include "GVariables.h"
#include "MyThread.h"
#include "Data.h"
#include "ui_Form.h"

#ifdef Q_OS_LINUX
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#endif

#ifdef Q_OS_WIN32
#include <Windows.h>
#include <string>
//#else
//#ifdef Q_OS_LINUX
//#include <iostream>
//#include <sys/types.h>
//#include <dirent.h>
//#endif
#endif

#ifdef DEBUG
#include <ctime>
#endif

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), uiForm(new Ui::Form),
                                      mpMessageBox(nullptr), mpCurrentSoundFile(nullptr),
									  mCurrentDir(""), mCurrentWord(""), mStrListFiles(""), mNumber(0)
{
	uiForm ->setupUi(this);
	setWindowTitle(QWidget::tr("������������-4"));
	
	mMyPlayer.createMyPlayer(uiForm);
	
	if (loadCache())
		uiForm ->cacheWord ->addItems(mCacheFiles.keys());

	//for (int i = 0; i < uiForm ->cacheWord ->count(); ++i)
	//{
	//	/*uiForm ->cacheWord ->item(i) ->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);*/
	//	uiForm ->cacheWord ->item(i) ->setIcon(QPixmap("images/cursor.png"));
	//}
	
	mpClipboard = QApplication::clipboard(); 
	fromClipboardToLineEdit(); // ����� �� ������ ������ -> � ������ ����� �����
	
	// ��� ��������� ������ ������, ����� �� ������ -> � ������ �����
	QObject::connect(mpClipboard, SIGNAL(changed(QClipboard::Mode)), this, SLOT(fromClipboardToLineEdit()));
}

void MyWidget::fromClipboardToLineEdit() // ����
{
	if (mpClipboard ->text().size() < 35)
	{
		uiForm ->lineEditInput ->setText(mpClipboard ->text());
		quint32 n = getIndexString(mCurrentListFileName, uiForm ->lineEditInput ->text(), "OneWord");
		if (n == -1)
			uiForm ->lineEditInput ->setFocus();
		else
		{
			qDebug() << "Index = " << n;
			quint32 m = uniqueFileName(mCurrentListFileName, uiForm ->lineEditInput ->text());
			if (m == 1)
				uiForm ->listWidgetFiles ->setCurrentRow(n);
			else
				uiForm ->listWidgetFiles ->setCurrentRow(mCurrentIndex);
		}
	}
}

// ��������� ������ ��� ������ �� ������ ���������� �� SoundFile
QStringList MyWidget::getStringListName(const QList <SoundFile*>& listPsoundFile)
{
	QStringList stringList;
	foreach (SoundFile* e, listPsoundFile)
		stringList.append(e ->getName());
		//stringList.insert(stringList.begin(), e ->getName()); // � �������� �������
	return stringList;
}

// �������� �������, ����� ����� �� ������, � ������
bool MyWidget::containsFileName(const QStringList& strListFiles, const QString& word)
{
	foreach (const QString e, strListFiles)
	{
		if (SoundFile::startsWithOneWord(e, word))
			return true;
	}
	return false;
}

quint32 MyWidget::uniqueFileName(const QStringList& strListFiles, const QString& word)
{
	quint32 count = 0;
	foreach (const QString e, strListFiles)
	{
		if (SoundFile::startsWithOneWord(e, word))
			++count;
	}
	return count;
}

// ���������� ������ ������ � ������, ������� �������� ����� (����, ��� ������ � ��������������, ��� ���������� �� �����)
qint32 MyWidget::getIndexString(const QStringList& strList, const QString& word, const QString mod)
{
	quint32 n = strList.size();
	for (quint32 i = 0; i < n; ++i)
	{
		if (mod == "WordGroup" && SoundFile::startsWithWordGroup(strList[i], word)) // ����� ������ � ��������������
			return i;
		if (mod == "OneWord"   && SoundFile::startsWithOneWord  (strList[i], word)) // ���� �����
			return i;
		if (mod == "Word"      && SoundFile::startsWithWord     (strList[i], word)) // ���������� �� �����
			return i;
	}
	return -1;
}

// ���������� true, ���� ���� �� ���� ������ � ������ - �������������� �� ������ (���� ������� ��� - ����� �����)
bool MyWidget::containsWordGroup(const QStringList& strListFiles, const QString& word)
{
	foreach(QString e, strListFiles)
	{
		if (SoundFile::startsWithWordGroup(e, word))
			return true;
	}
	return false;
}

#define FIND_3

#ifdef FIND_3
// ���������� QHash <QChar, QStringList> (��� QMap)(����� ����� � ������ ������ � ���)
// ������ ������ �� �����, � ���������� ������ � ����� - ���� ������
// ����� ������ �� �����, ���� �������, �� ���������� ������� �������
bool MyWidget::findFiles(QString& word)
{
	mCurrentWord = word; // ������� �����
	
#ifdef DEBUG
qDebug() << mCurrentDir.dirName();
#endif
// ���� ��� ������� ����� ������ (������ �����), ��� ����� ���������� �� �����, �������� �� ������ ����� �����
// �����, ����� �������������� ��� �� ������ ������ (�� ��� �� �����), ��� ��� �� ����� ������ 
	if (mCurrentDir.dirName().isEmpty() ||  mCurrentDir.dirName()[0] != word[0])
	{
		mCurrentDir = QDir(GlobalVariables::getGlobalVariables().PATH_SOUND + '/' + word[0]); // �����, �� ������ ����� �����
		if (!mCurrentDir.exists()) // ���� ��� ����� �����
			return false;
#ifdef DEBUG
clock_t t1 = clock();
#endif
   // ������ ��� ������ �� ����� �� ������ ����� �����
    mStrListFiles = GlobalVariables::getGlobalVariables().ListsFilesFromDirs.value(word[0]);

#ifdef DEBUG
clock_t t2 = clock();
qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
		mNumber = mStrListFiles.size();
		if (mNumber == 0) // ���� � ����� ��� ������
		{
			mCurrentAbsFilePath.clear();
			return false;
		}
	} // if

    
    // ���������, �� ������ ��� ������, ������ ��� ������ � ������ ���������� ����� � ������, �� ��������� �����
	return extractFiles(mStrListFiles);
}
#endif
#ifndef FIND_3 
#ifdef FIND_2
// ������ ������ �� �����, � ���������� ������ � ����� - ���� ������
// ����� ������ �� �����, ���� �������, �� ���������� ������� �������
bool MyWidget::findFiles(QString& word)
{
	mCurrentWord = word; // ������� �����
	
#ifdef DEBUG
qDebug() << mCurrentDir.dirName();
#endif
// ���� ��� ������� ����� ������ (������ �����), ��� ����� ���������� �� �����, �������� �� ������ ����� �����
// �����, ����� �������������� ��� �� ������ ������ (�� ��� �� �����), ��� ��� �� ����� ������ 
	if (mCurrentDir.dirName().isEmpty() ||  mCurrentDir.dirName()[0] != word[0])
	{
		mCurrentDir = QDir(GlobalVariables::getGlobalVariables().PATH_SOUND + '/' + word[0]); // �����, �� ������ ����� �����
		if (!mCurrentDir.exists()) // ���� ��� ����� �����
			return false;
#ifdef DEBUG
clock_t t1 = clock();
#endif
  mStrListFiles = mCurrentDir.entryList(/*QDir::Files*/); // ������ ��� ������ �� ���� ����� (� QDir::Files ���������)
#ifdef DEBUG
clock_t t2 = clock();
qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
		mNumber = mStrListFiles.size();
		if (mNumber == 0) // ���� � ����� ��� ������
		{
			mCurrentAbsFilePath.clear();
			return false;
		}
	} // if
#ifdef DEBUG
clock_t t1 = clock();
#endif
	// ��������� �� ������ ��� ������, ������ ��� ������ � ������ ���������� ����� � ������, �� ��������� �����
	mCurrentListFileName.clear();
	mCurrentListAbsFilePath.clear();
	
	for (quint32 i = 0; i < mNumber; ++i) // ���� ������ ��������� ��� QDir::Files, �� ������ ��� ����� ���������� ('.' � '..') 
	{
		if ((uiForm ->checkBox ->isChecked()) || // ����� ��������������
			(!uiForm ->checkBox ->isChecked() && 
			  word.contains(' ')                 ) )
		{
			//if (SoundFile::startsWithWord(strListFiles[i], word)) // ��, ��� ���������� � word
			if (SoundFile::startsWithWordGroup(mStrListFiles[i], word) ||
				SoundFile::startsWithOneWord  (mStrListFiles[i], word)) 
			{
				mCurrentListFileName.append(mStrListFiles[i]);
				mCurrentListAbsFilePath.append(mCurrentDir.absoluteFilePath(mStrListFiles[i]));
			}

			/*if (SoundFile::startsWithWordGroup(mCurrentDir[i], word) ||
				SoundFile::startsWithOneWord  (mCurrentDir[i], word)) 
			{
				mCurrentListFileName.append(mCurrentDir[i]);
				mCurrentListAbsFilePath.append(mCurrentDir.absoluteFilePath(mCurrentDir[i]));
			}*/
		}
		else // ������ �����
		{
			/*if (SoundFile::startsWithOneWord(mCurrentDir[i], word))
            {
				mCurrentListFileName.append(mCurrentDir[i]);
				mCurrentListAbsFilePath.append(mCurrentDir.absoluteFilePath(mCurrentDir[i]));
			}*/
			if (SoundFile::startsWithOneWord(mStrListFiles[i], word))
			{
				mCurrentListFileName.append(mStrListFiles[i]);
				mCurrentListAbsFilePath.append(mCurrentDir.absoluteFilePath(mStrListFiles[i]));
			}
		}
	}
	if (mCurrentListAbsFilePath.isEmpty())
	{
		//mCurrentAbsFilePath.clear();
		return false;
	}
	#ifdef DEBUG
clock_t t2 = clock();
qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
	return true;
}

#else

// ������� �� static
// ����� ������ �� �����, ���� �������, �� ���������� ������� �������
bool MyWidget::findFiles(QString& word)
{
	mCurrentWord = word; // ������� �����
	static QStringList strListFiles; // ������ ��� ������ �� ����� //??? �������� � ������ ��-�� static?
	static quint32 number = 0; // ���������� ������ � �����
#ifdef DEBUG
qDebug() << mCurrentDir.dirName();
#endif
// ���� ��� ������� ����� ������ (������ �����), ��� ����� ���������� �� �����, �������� �� ������ ����� �����
// �����, ����� �������������� ��� �� ������ ������ (�� ��� �� �����), ��� ��� �� ����� ������ 
	if (mCurrentDir.dirName().isEmpty() ||  mCurrentDir.dirName()[0] != word[0])
	{
		mCurrentDir = QDir(GlobalVariables::getGlobalVariables().PATH_SOUND + '/' + word[0]); // �����, �� ������ ����� �����
		if (!mCurrentDir.exists()) // ���� ��� ����� �����
			return false;
#ifdef DEBUG
clock_t t1 = clock();
#endif
  strListFiles = mCurrentDir.entryList(/*QDir::Files*/); // ������ ��� ������ �� ���� ����� (� QDir::Files ���������)
#ifdef DEBUG
clock_t t2 = clock();
qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
		number = strListFiles.size();
		if (number == 0) // ���� � ����� ��� ������
		{
			mCurrentAbsFilePath.clear();
			return false;
		}
	} // if
#ifdef DEBUG
clock_t t1 = clock();
#endif
	// ��������� �� ������ ��� ������, ������ ��� ������ � ������ ���������� ����� � ������, �� ��������� �����
	mCurrentListFileName.clear();
	mCurrentListAbsFilePath.clear();
	
	for (quint32 i = 0; i < number; ++i) // ���� ������ ��������� ��� QDir::Files, �� ������ ��� ����� ���������� ('.' � '..') 
	{
		if ((uiForm ->checkBox ->isChecked()) || // ����� ��������������
			(!uiForm ->checkBox ->isChecked() && 
			  word.contains(' ')                 ) )
		{
			//if (SoundFile::startsWithWord(strListFiles[i], word)) // ��, ��� ���������� � word
			if (SoundFile::startsWithWordGroup(strListFiles[i], word) ||
				SoundFile::startsWithOneWord  (strListFiles[i], word)) 
			{
				mCurrentListFileName.append(strListFiles[i]);
				mCurrentListAbsFilePath.append(mCurrentDir.absoluteFilePath(strListFiles[i]));
			}

			/*if (SoundFile::startsWithWordGroup(mCurrentDir[i], word) ||
				SoundFile::startsWithOneWord  (mCurrentDir[i], word)) 
			{
				mCurrentListFileName.append(mCurrentDir[i]);
				mCurrentListAbsFilePath.append(mCurrentDir.absoluteFilePath(mCurrentDir[i]));
			}*/
		}
		else // ������ �����
		{
			/*if (SoundFile::startsWithOneWord(mCurrentDir[i], word))
            {
				mCurrentListFileName.append(mCurrentDir[i]);
				mCurrentListAbsFilePath.append(mCurrentDir.absoluteFilePath(mCurrentDir[i]));
			}*/
			if (SoundFile::startsWithOneWord(strListFiles[i], word))
			{
				mCurrentListFileName.append(strListFiles[i]);
				mCurrentListAbsFilePath.append(mCurrentDir.absoluteFilePath(strListFiles[i]));
			}
		}
	}
	if (mCurrentListAbsFilePath.isEmpty())
	{
		//mCurrentAbsFilePath.clear();
		return false;
	}
	/*qDebug() << mCurrentListFileName;
	reverseList(mCurrentListFileName);
	qDebug() << mCurrentListFileName;*/
#ifdef DEBUG
clock_t t2 = clock();
qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
	return true;
}
#endif
#endif

bool MyWidget::operator()(const QStringList& listFiles)
{
	#ifdef DEBUG
clock_t t1 = clock();
#endif
	
 	/*this ->mCurrentListFileName.clear();
	this ->mCurrentListAbsFilePath.clear();*/
  	
	for (int i = 0; i < listFiles.size(); ++i) 
	{
		if ((this ->uiForm ->checkBox ->isChecked()) || // ����� ��������������
			(!this ->uiForm ->checkBox ->isChecked() && 
  			  this ->mCurrentWord.contains(' ')                 ) )
		{
			//if (SoundFile::startsWithWord(strListFiles[i], word)) // ��, ��� ���������� � word
			if (SoundFile::startsWithWordGroup(listFiles[i], this ->mCurrentWord) ||
				SoundFile::startsWithOneWord  (listFiles[i], this ->mCurrentWord)) 
			{
				GlobalVariables::getGlobalVariables().mutex.lock();
				this ->mCurrentListFileName.append(listFiles[i]);
				this ->mCurrentListAbsFilePath.append(this ->mCurrentDir.absoluteFilePath(listFiles[i]));
				GlobalVariables::getGlobalVariables().mutex.unlock();
			}
		}
		else // ������ �����
		{
     			if (SoundFile::startsWithOneWord(listFiles[i], this ->mCurrentWord))
			{
				GlobalVariables::getGlobalVariables().mutex.lock();
				this ->mCurrentListFileName.append(listFiles[i]);
				this ->mCurrentListAbsFilePath.append(this ->mCurrentDir.absoluteFilePath(listFiles[i]));
				GlobalVariables::getGlobalVariables().mutex.unlock();
			}
		}
    }
	if (this ->mCurrentListAbsFilePath.isEmpty())
	{
		//mCurrentAbsFilePath.clear();
		return false;
	}
#ifdef DEBUG
clock_t t2 = clock();
qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
	return true;
}

bool MyWidget::extractFiles(const QStringList& listFiles/*, MyWidget* pWidg*/)
{
#ifdef DEBUG
clock_t t1 = clock();
#endif
	
	this ->mCurrentListFileName.clear();
	this ->mCurrentListAbsFilePath.clear();
	
	for (int i = 0; i < listFiles.size(); ++i) 
	{
		if ((this ->uiForm ->checkBox ->isChecked()) || // ����� ��������������
			(!this ->uiForm ->checkBox ->isChecked() && 
			  this ->mCurrentWord.contains(' ')                 ) )
		{
			//if (SoundFile::startsWithWord(strListFiles[i], word)) // ��, ��� ���������� � word
			if (SoundFile::startsWithWordGroup(listFiles[i], this ->mCurrentWord) ||
				SoundFile::startsWithOneWord  (listFiles[i], this ->mCurrentWord)) 
			{
				GlobalVariables::getGlobalVariables().mutex.lock();
				this ->mCurrentListFileName.append(listFiles[i]);
				this ->mCurrentListAbsFilePath.append(this ->mCurrentDir.absoluteFilePath(listFiles[i]));
				GlobalVariables::getGlobalVariables().mutex.unlock();
			}
		}
		else // ������ �����
		{
			if (SoundFile::startsWithOneWord(listFiles[i], this ->mCurrentWord))
			{
				GlobalVariables::getGlobalVariables().mutex.lock();
				this ->mCurrentListFileName.append(listFiles[i]);
				this ->mCurrentListAbsFilePath.append(this ->mCurrentDir.absoluteFilePath(listFiles[i]));
				GlobalVariables::getGlobalVariables().mutex.unlock();
			}
		}
	}
	if (this ->mCurrentListAbsFilePath.isEmpty())
	{
		//mCurrentAbsFilePath.clear();
		return false;
	}
#ifdef DEBUG
clock_t t2 = clock();
qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
	return true;
}

//template <typename T>
//void extractFiles(const QStringList& listFiles, MyWidget* pWidg)
//{
//#ifdef DEBUG
//clock_t t1 = clock();
//#endif
//	
//	pWidg ->mCurrentListFileName.clear();
//	pWidg ->mCurrentListAbsFilePath.clear();
//	
//	for (quint32 i = 0; i < pWidg ->mNumber; ++i) 
//	{
//		if ((pWidg ->uiForm ->checkBox ->isChecked()) || // ����� ��������������
//			(!pWidg ->uiForm ->checkBox ->isChecked() && 
//			  pWidg ->mCurrentWord.contains(' ')                 ) )
//		{
//			//if (SoundFile::startsWithWord(strListFiles[i], word)) // ��, ��� ���������� � word
//			if (SoundFile::startsWithWordGroup(listFiles[i], pWidg ->mCurrentWord) ||
//				SoundFile::startsWithOneWord  (listFiles[i], pWidg ->mCurrentWord)) 
//			{
//				pWidg ->mCurrentListFileName.append(listFiles[i]);
//				pWidg ->mCurrentListAbsFilePath.append(pWidg ->mCurrentDir.absoluteFilePath(listFiles[i]));
//			}
//		}
//		else // ������ �����
//		{
//			if (SoundFile::startsWithOneWord(listFiles[i], pWidg ->mCurrentWord))
//			{
//				pWidg ->mCurrentListFileName.append(listFiles[i]);
//				pWidg ->mCurrentListAbsFilePath.append(pWidg ->mCurrentDir.absoluteFilePath(listFiles[i]));
//			}
//		}
//	}
//	if (pWidg ->mCurrentListAbsFilePath.isEmpty())
//	{
//		//mCurrentAbsFilePath.clear();
//		//return false;
//	}
//#ifdef DEBUG
//clock_t t2 = clock();
//qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
//#endif
//}


// ����� ��� ��������� ������
void MyWidget::showFilesFound()
{
	uiForm ->listWidgetFiles ->clear();
	
	// �����, � ������ ��������, ������ ��� ��������� ������
	uiForm ->listWidgetFiles ->addItems(mCurrentListFileName);
	
	// ��������� � ������ ����� �������� �����
	//uiForm ->listWidgetFiles ->setCurrentRow(mCurrentIndex);
	
	/*uiForm ->listWidgetFiles ->setCurrentRow(mCurrentListAbsFilePath.indexOf(mCurrentAbsFilePath));*/
#ifdef DEBUG
#endif	
	// ����� � labelNote, ���������� (Note) �������� SoundFile, � ���� ������
	//uiForm ->labelNote ->setText(mpCurrentSoundFile ->getNote().stringForNote());
}

void MyWidget::choiceItemFromList(QListWidgetItem* item)
{
#ifdef DEBUG
qDebug() << QWidget::tr("��������� ����");
#endif
	qDebug() << mCurrentIndex;
    if (mCurrentIndex != uiForm ->listWidgetFiles ->row(item))
	{
	qDebug() << uiForm ->listWidgetFiles ->row(item);
		setNewCurrentIndex(uiForm ->listWidgetFiles ->row(item));
	qDebug() << mCurrentIndex;
		uiForm ->lineEditInput ->setText(SoundFile::extractWordGroup(mCurrentListFileName[mCurrentIndex]));
		mCurrentWord = uiForm ->lineEditInput ->text();
		mpClipboard ->setText(mCurrentWord);
		//uiForm ->labelNote ->setText(mpCurrentSoundFile ->getNote().stringForNote()); // ����� ���������� (Note), �������� SoundFile, �������
#ifdef DEBUG
qDebug() << QWidget::tr("���������!");
#endif	
	}
#ifdef DEBUG
	else
		qDebug() << QWidget::tr("�� ���������!");
#endif
	
	uiForm ->labelOutput ->setText(mCurrentWord);
	uiForm ->lineEditInput ->setText(mCurrentWord);
	play(mCurrentAbsFilePath); // ��������������� �������� �����
}

void MyWidget::addCache()
{
	// ���������� � ��� ����� � ��������� ����� (����)
	if (mCacheFiles.find(mCurrentWord) == mCacheFiles.end())
	{
		mCacheFiles.insert(mCurrentWord, mCurrentAbsFilePath);
		
		//mStrListCacheWord.append(mCurrentWord);
		//mStrListCacheWord.sort();
		// ����� ����
		//uiForm ->cacheWord ->clear();
	    //uiForm ->cacheWord ->addItems(mStrListCacheWord);
		uiForm ->cacheWord ->addItem(mCurrentWord);
	}
}

bool MyWidget::loadCache()
{
	QFile file("Cache.dat");
	if (file.open(QIODevice::ReadOnly))
	{
		QDataStream in(&file);
		in >> mCacheFiles;
		file.close();
		return true;
	}
	return false;
}

bool MyWidget::saveCache()
{
	QFile file("Cache.dat");
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream out(&file);
		out << mCacheFiles;
		file.close();
		return true;
	}
	return false;
}

void MyWidget::choiceItemFromCacheWord(QListWidgetItem* item) // ����� ����� �� ����
{
	//uiForm ->lineEditInput ->setText(item ->text());
	//mCurrentAbsFilePath = mCacheFiles.value(item ->text());
	QString AbsFilePath = mCacheFiles.value(item ->text());
	//mpClipboard ->setText(item ->text());
	play(AbsFilePath); // ��������������� ����� 
	
    //pressedEnter();
}

// ��������������� �����
void MyWidget::play(QString& AbsFilePath)
{
	qDebug() << "mCurrentAbsFilePath = " << AbsFilePath;
	if (!GlobalVariables::getGlobalVariables().LIST_EXT.contains(SoundFile::extractExt(AbsFilePath)))
	{
		uiForm ->labelOutput ->setText(QWidget::tr("���� �� ��������\n��� ���������������!"));
		return;
	}
	mMyPlayer.play(AbsFilePath); // ��������������� �����
}

// ��������� ������ �������� ������� � �������
void MyWidget::setNewCurrentIndex(quint32 newInd)
{
	mCurrentIndex = newInd;
	if (!mCurrentListFileName.isEmpty())
	{
		mCurrentFileName = mCurrentListFileName[mCurrentIndex];
		mCurrentAbsFilePath = mCurrentListAbsFilePath[mCurrentIndex];
		uiForm ->listWidgetFiles ->setCurrentRow(mCurrentIndex);
		mCurrentWord = SoundFile::extractWordGroup(mCurrentFileName);
		uiForm ->labelOutput ->setText(mCurrentWord);
		uiForm ->lineEditInput ->setText(mCurrentWord);
	}
}
		
// �������� ����� �� ������ �����, ��� ������ ���������� ��������
int MyWidget::checkWord(const QString& word)
{
	 // ������ �����
	if (word.isEmpty())
		return 1;
	// ������������ ������ � ����� (���� � ������, ���� � �����, �� ���������� ����� � �� ������,
	// ��� ���� ������ ��� �� ������
	QRegExp reg("^-|[^a-z- ]|-$|-.*-", Qt::CaseInsensitive);
	//QRegExp reg2("[a-z]+|-",Qt::CaseInsensitive);
	//for (int i = 0; i < word.size(); ++i)
	//{
		if (reg.indexIn(word) != -1) 
			return 2;
	//}
	// ���� ������� �� �� ����� �����, ��� �� ���� ������� ����� ���� (���� ��������� ������ - ����)
	if (mCurrentAbsFilePath == "" && mCurrentWord == word) 
		return 3;
	
	// ���� ������� ������� ��������������, 
	// �� ��������� ����� ������ 
	if (uiForm ->checkBox ->isChecked())
		return 6;
	
	// ���� �� ������� ������� ��������������, 
	// � ����� (��� �������������� ��� �����) ���� � ������� ������
	if (!uiForm ->checkBox ->isChecked() && containsFileName(mCurrentListFileName, word))
	{
		if (mCurrentListFileName.size() > 1) // � ������ �������� ������
		{
			uiForm ->listWidgetFiles ->clear();
			qDebug() << "if c p";
			QStringList newList;
			bool flag = false;
				
			for (int i = 0; i < mCurrentListFileName.size(); ++i) // ����� ������ ������ � ���� ������ 
			{
				if (SoundFile::startsWithOneWord(mCurrentListFileName[i], word))
				{
					newList.append(mCurrentListFileName[i]);
					flag = true;
				}
			}
			if (!flag)
				return 6;
				
			mCurrentListFileName.clear();
			newList.swap(mCurrentListFileName); // ������ ����� ������ �������
			mCurrentListAbsFilePath.clear();
				
			for (int i = 0; i < mCurrentListFileName.size(); ++i) // ����� ������ �����
				mCurrentListAbsFilePath.append(mCurrentDir.absoluteFilePath(mCurrentListFileName[i]));
		
			uiForm ->listWidgetFiles ->addItems(mCurrentListFileName);
			return 5;
		}
		return 4; // � ������ ���� ����
	}
	
	return 6; // ����� ������
}

// ����. � ������ ����� ������ Enter
void MyWidget::pressedEnter()
{
#ifdef DEBUG 
qDebug() << QWidget::tr("������ Enter");
#endif
    QString word = (uiForm ->lineEditInput ->text()).trimmed().toLower();
	uiForm ->lineEditInput ->setText(word);
	int ind = 0;
	switch (checkWord(word))
	{
	case 1 : 
		uiForm ->labelOutput ->setText(QWidget::tr("������� �����!"));
		break;
	case 2 :
		uiForm ->labelOutput ->setText(QWidget::tr("������������ �������\n� �����!"));
		break;
	
	case 3 :
#ifdef DEBUG 
qDebug() <<  QWidget::tr("������ Enter. �� ��������� ����� �� ����������!");
#endif
        uiForm ->labelOutput ->setText(QWidget::tr("����� �� �������!"));
		break;
	case 4 : // ���� �� ������ ���� � ������� ������, � ������� ���� ����� ��������� �����
#ifdef DEBUG 
qDebug() <<  QWidget::tr("������ Enter. ��������� ���� �� ���������!");
#endif
		//setNewCurrentIndex(getIndexString(mCurrentListFileName, word, "OneWord"));
        
		play(mCurrentAbsFilePath); // ��������������� �������� �����
		break;
	case 5 : // ���� �� ������ ���� � ������� ������, �� ������� ���� �� ����� ��������� �����
#ifdef DEBUG 
qDebug() <<  QWidget::tr("������ Enter. ��������� ���� ���������!");
#endif
		ind = getIndexString(mCurrentListFileName, word, "OneWord"); // ����� � ������ ����� �� ������ ��������� �����
		if (ind != -1) // ���� ����� ���� ���� � ������
		{
			setNewCurrentIndex(ind); // ��������� ������ �������� �������
		
			play(mCurrentAbsFilePath); // ��������������� �������� �����
			break;
		}
		// ���� ������ ����� � ������ ��� (�.�., ���� ������ �������������� � �������� ������), �� ����� ������
	default :
		// ����� - � ������ ������
		mpClipboard ->setText(word);
#ifdef DEBUG	
qDebug() <<  QWidget::tr("����� ������!");
#endif
		
	    // ����� �������� ������ �� �����
		if (findFiles(word)) // ���� ����� ����������
		{
			showFilesFound(); // ����� � ������ ��� ��������� ������
			// ����� ������� ������ (� ������ - ��� ������ ������ ��������� ����� ����� (�������� �����))
			setNewCurrentIndex(getIndSmallestElement(mCurrentListFileName)); 
			
			uiForm ->labelOutput ->setText(mCurrentWord);
			
			//mpClipboard ->setText(mCurrentWord);
			
			play(mCurrentAbsFilePath); // ��������������� �������� �����
			
			addCache(); // ���������� ����� � ����� � ���
		}
		else // ���� ������, � ����� ������, ���
		{
			if (word.contains(' '))
				uiForm ->labelOutput ->setText(QWidget::tr("��������������\n�� �������!"));
			else
				uiForm ->labelOutput ->setText(QWidget::tr("����� �� �������!"));
			uiForm ->listWidgetFiles ->clear();
		}
	} // switch (checkWord(word))
}

void MyWidget::changeStateButton(QAbstractButton* pButton)
{
	if (pButton ->isChecked())
		pButton ->setChecked(false);
	else
		pButton ->setChecked(true);
}

// ������������ �� ���������� ���� ��� �������� ����
bool MyWidget::event(QEvent* pe) 
{
	if (pe ->type() == QEvent::WindowActivate)
	{
#ifdef Q_OS_WIN32
		
			qDebug() << "define Q_OS_WIN32";
			PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 1, 0x04090409);
			// LoadKeyboardLayout(L"00000409", KLF_ACTIVATE); // ��� ���
#endif

#ifdef Q_OS_LINUX
#ifdef DEBUG
qDebug() << "define Q_OS_LINUX";
#endif
			
			int xkbGroup = 0;
            int event_rtrn, error_rtrn, reason_rtrn;
            Display* display = XkbOpenDisplay(NULL, &event_rtrn, &error_rtrn,
                                              NULL, NULL, &reason_rtrn       );
            if(display == NULL)
            {
                #ifdef DEBUG
				qDebug() << "Cannot open display!";
                #endif
                return QWidget::event(pe);
            }
            XkbLockGroup(display, XkbUseCoreKbd, xkbGroup);
            XCloseDisplay(display);
#endif
	
	}
	return QWidget::event(pe);
	
}

void MyWidget::pressKeyNoModifier(const int codeKey)
{
	if (uiForm ->cacheWord ->count() != 0) // ���� ��� �� ����
	{
		// ������� �����
		if (codeKey == Qt::Key_Right) 
		{
			uiForm ->cacheWord ->setFocus(); // ������� ������ � ���
			uiForm ->cacheWord ->currentItem() ->setSelected(true);
			return;
		}
		// ������� �����, � ������ ������� � ���� � ������
		if (codeKey == Qt::Key_Up && uiForm ->cacheWord ->hasFocus() && uiForm ->cacheWord ->item(0) ->isSelected())
		{
			uiForm ->lineEditInput ->setFocus(); // ������� ������ � ������ �����
			return;
		}
		// ������� ����, ����� ����� � ������ �����, � ������ ������ ����
		if (codeKey == Qt::Key_Down && uiForm ->lineEditInput ->hasFocus() && uiForm ->listWidgetFiles ->count() == 0)
		{
			uiForm ->cacheWord ->setFocus(); // ������� ������ � ���
			uiForm ->cacheWord ->currentItem() ->setSelected(true);
			return;
		}
	
		// �������� �� ����
		if (codeKey == Qt::Key_Delete && uiForm ->cacheWord ->hasFocus())
		{
			mCacheFiles.remove(uiForm ->cacheWord ->currentItem() ->text());
			uiForm ->cacheWord ->takeItem(uiForm ->cacheWord ->currentRow());
			// ����� ��� ����, �� ����� � ������ �����
			if (uiForm ->cacheWord ->count() == 0)
				uiForm ->lineEditInput ->setFocus(); 
			return;
		}
	}
	
	if (uiForm ->listWidgetFiles ->count() != 0) // ���� ���� ������ �� ����
	{
		switch (codeKey)
		{
			case Qt::Key_Left : // ������� �����
				uiForm ->listWidgetFiles ->setFocus(); // ������� ������ � ���� ������
				uiForm ->listWidgetFiles ->currentItem() ->setSelected(true);
			break;
		
			case Qt::Key_Down : // ������� ���� �
			if (uiForm ->lineEditInput ->hasFocus()) // ������ ����� � ������
			{
				uiForm ->listWidgetFiles ->setFocus(); // ������� ������ � ���� ������
				uiForm ->listWidgetFiles ->setCurrentRow(0);
			}
			// ��������� ������, � ����� ������, � ������ 
			else if (uiForm ->listWidgetFiles ->item(uiForm ->listWidgetFiles ->count() - 1) ->isSelected())
					uiForm ->lineEditInput ->setFocus(); // ������� ������ � ������ �����
			break;
			
			case Qt::Key_Up : // ������� ����� �
				if (uiForm ->listWidgetFiles ->hasFocus() && uiForm ->listWidgetFiles ->item(0) ->isSelected()) // ������ �������, � ����� ������, � ������
					uiForm ->lineEditInput ->setFocus(); // ������� ������ � ������ �����
				else if (uiForm ->lineEditInput ->hasFocus()) // ������ ����� � ������
				{
					uiForm ->listWidgetFiles ->setFocus(); // ������� ������ � ���� ������
					uiForm ->listWidgetFiles ->setCurrentRow(uiForm ->listWidgetFiles ->count() - 1);
				}
			break;
		}

		//if (uiForm ->cacheWord ->count() != 0) // ���� � ���� ������ � ��� �� ������
		//{
		//	switch (codeKey)
		//	{
		//		case Qt::Key_Left : // ������� �����
		//			uiForm ->listWidgetFiles ->setFocus(); // ������� ������ � ���� ������
		//			uiForm ->listWidgetFiles ->currentItem() ->setSelected(true);
		//		break;
		//		case Qt::Key_Right : // ������� ������
		//			uiForm ->cacheWord ->setFocus(); // ������� ������ � ���
		//			uiForm ->cacheWord ->currentItem() ->setSelected(true);
		//		break;
		//	}
		//}
	}
}

void  MyWidget::pressKeyYesModifier(const Qt::KeyboardModifiers& keyModifiers, const int codeKey)
{
	// ��������� � ������ radioButtonRepeat ("���������") (���� Alt + <-)
	// (���� Shift + Alt + ->)
	// if (codeKey == Qt::Key_Right && (keyModifiers & Qt::ShiftModifier) && (keyModifiers & Qt::AltModifier)) 
	if (codeKey == Qt::Key_Left && (keyModifiers & Qt::AltModifier)) 
	{
		changeStateButton(uiForm ->radioButtonRepeat);
		return;
	}
	
	// ��������� � ������ checkBox ("������� ��������������") (���� Alt + ->)
	if (codeKey == Qt::Key_Right && (keyModifiers & Qt::AltModifier)) 
	{
		changeStateButton(uiForm ->checkBox);
		
		uiForm ->lineEditInput ->setFocus();
		return;
	}
}

// ��������� ������� ������ ����������
void MyWidget::keyPressEvent(QKeyEvent* pe)
{
	int codeKey = pe ->key();
	Qt::KeyboardModifiers keyModifiers = pe ->modifiers();
	
	//qDebug() << pe ->nativeVirtualKey();
	//qDebug() << codeKey;
	//qDebug() << (pe ->modifiers() & Qt::ShiftModifier);
	
	if (keyModifiers == Qt::NoModifier)
		pressKeyNoModifier(codeKey);
	else
		pressKeyYesModifier(keyModifiers, codeKey);
	
	QWidget::keyPressEvent(pe);
}

// ���������� ������
////////////////////////////////////////////////////////////////////////////

void MyWidget::reject()
{

}

void MyWidget::accept(QString& fileName, QString& fileNewName) // ������ �����
{
	if (mDirSound.remove(mDirSound.absoluteFilePath(fileNewName)))
	{
		saveFile(fileName, fileNewName);
		uiForm ->labelOutput ->setText(QWidget::tr("���� ������� ������!"));
	}
	else
        uiForm ->labelOutput ->setText(QWidget::tr("������ ��� ��������\n������� �����!"));
}

void MyWidget::acceptMessBox()
{
	#ifdef DEBUG
	qDebug() << "acceptMessBox()";
	#endif
}

void MyWidget::saveFile(QString& fileName, QString& fileNewName)
{
	if (QFile::copy(fileName, mDirSound.filePath(fileNewName)))
    {
        fileNewName.truncate(fileNewName.lastIndexOf('.'));
		uiForm ->lineEditInput ->setText(fileNewName);
        uiForm ->lineEditInput ->setFocus();
        
		uiForm ->labelOutput ->setText(QWidget::tr("���� ������� ��������!"));
	}
    else
	{
        uiForm ->labelOutput ->setText(QWidget::tr("������\n��� ����������� �����!"));
		uiForm ->lineEditInput ->setFocus();
	}
}

void MyWidget::choiceFileInExplorer()
{
    QString fileName = QFileDialog::getOpenFileName(this, QWidget::tr("����� �����"), 
                                           QDir::homePath(), GlobalVariables::getGlobalVariables().FILTER);
#ifdef DEBUG
qDebug() << fileName;
#endif
    if (fileName.isEmpty())
    {
        uiForm ->labelOutput ->setText(QWidget::tr("���� �� ������!"));
		uiForm ->lineEditInput ->setFocus();
		return;
    }
    
	QString fileNewName = SoundFile::extractName(fileName); // ��� ���������� ����� � �����������
    QString word = SoundFile::extractWord(fileNewName); // ����� � ����� �����
	// ����� �������� ������ �� �����
	if (findFiles(word)) // ���� ����� ����������
	{
		mCurrentWord = word; // ������� �����
		uiForm ->labelOutput ->setText(word);
		showFilesFound(); // ����� � ������ ��� ��������� ������ 
		
	    mpMessageBox = new QMessageBox(this);
	    QObject::connect(mpMessageBox, SIGNAL(accepted()), this, SLOT(acceptMessBox()));
		
		mpMessageBox ->setIcon(QMessageBox::Question);
		mpMessageBox ->setWindowTitle(QWidget::tr("���������� �����"));
		mpMessageBox ->setText(QWidget::tr("���� ��� ����������!\n�������� ��� ��������?"));
		
		QPushButton* changeButton = mpMessageBox ->addButton(QWidget::tr("��������"), QMessageBox::AcceptRole);
		QPushButton* appendButton = mpMessageBox ->addButton(QWidget::tr("��������"), QMessageBox::RejectRole);
		QPushButton* cancelButton = mpMessageBox ->addButton(QWidget::tr("������"),   QMessageBox::RejectRole);
		
		mpMessageBox ->exec();
		if (mpMessageBox ->clickedButton() == changeButton)
		{
				//accept(fileName, fileNewName); // ������ �����
				 uiForm ->labelOutput ->setText(QWidget::tr("���� ������!"));
				 uiForm ->lineEditInput ->setFocus();
		}
		else if (mpMessageBox ->clickedButton() == appendButton)
		{
			     uiForm ->labelOutput ->setText(QWidget::tr("���� ��������!"));
				 uiForm ->lineEditInput ->setFocus();
		}
		else if (mpMessageBox ->clickedButton() == cancelButton)
		{
				 uiForm ->labelOutput ->setText(QWidget::tr("������!"));
				 uiForm ->lineEditInput ->setFocus();
		}
				
		delete mpMessageBox;
		mpMessageBox = NULL;
		return;
    }
	else
		saveFile(fileName, fileNewName);
}
//////////////////////////////////////////////////////////////////////////////


