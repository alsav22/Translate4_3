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
	setWindowTitle(QWidget::tr("Произношение-4"));
	
	mMyPlayer.createMyPlayer(uiForm);
	
	if (loadCache())
		uiForm ->cacheWord ->addItems(mCacheFiles.keys());
	
	mpClipboard = QApplication::clipboard(); 
	fromClipboardToLineEdit(); // текст из буфера обмена -> в строку ввода слова
	
	// при изменении буфера обмена, текст из буфера -> в строку ввода
	QObject::connect(mpClipboard, SIGNAL(changed(QClipboard::Mode)), this, SLOT(fromClipboardToLineEdit()));
}

void MyWidget::fromClipboardToLineEdit() // слот
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

// получение списка имён файлов из списка указателей на SoundFile
QStringList MyWidget::getStringListName(const QList <SoundFile*>& listPsoundFile)
{
	QStringList stringList;
	foreach (SoundFile* e, listPsoundFile)
		stringList.append(e ->getName());
		//stringList.insert(stringList.begin(), e ->getName()); // в обратном порядке
	return stringList;
}

// Проверка наличия, имени файла со словом, в списке
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

// Возвращает индекс строки в списке, которая содержит слово (одно, или первым в словосочетании, или начинается на слово)
qint32 MyWidget::getIndexString(const QStringList& strList, const QString& word, const QString mod)
{
	quint32 n = strList.size();
	for (quint32 i = 0; i < n; ++i)
	{
		if (mod == "WordGroup" && SoundFile::startsWithWordGroup(strList[i], word)) // слово первое в словосочетании
			return i;
		if (mod == "OneWord"   && SoundFile::startsWithOneWord  (strList[i], word)) // одно слово
			return i;
		if (mod == "Word"      && SoundFile::startsWithWord     (strList[i], word)) // начинается на слово
			return i;
	}
	return -1;
}

// Возвращает true, если хотя бы одна строка в списке - словосочетание со словом (есть пробелы или - после слова)
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
// Глобальный QHash <QChar, QStringList> (или QMap)(буква папки и список файлов в ней)
// Список файлов из папки, и количество файлов в папке - поля класса
// Поиск файлов по слову, если найдены, то заполнение текущих списков
bool MyWidget::findFiles(QString& word)
{
	mCurrentWord = word; // текущее слово
	
#ifdef DEBUG
qDebug() << mCurrentDir.dirName();
#endif
// Если имя текущей папки пустое (первый поиск), или папка начинается на букву, отличную от первой буквы слова
// Иначе, будет использоавться тот же список файлов (из той же папки), что был до этого поиска 
	if (mCurrentDir.dirName().isEmpty() ||  mCurrentDir.dirName()[0] != word[0])
	{
		mCurrentDir = QDir(GlobalVariables::getGlobalVariables().PATH_SOUND + '/' + word[0]); // папка, на первую букву слова
		if (!mCurrentDir.exists()) // если нет такой папки
			return false;
#ifdef DEBUG
clock_t t1 = clock();
#endif
   // список имён файлов из папки на первую букву слова
    mStrListFiles = GlobalVariables::getGlobalVariables().ListsFilesFromDirs.value(word[0]);

#ifdef DEBUG
clock_t t2 = clock();
qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
		mNumber = mStrListFiles.size();
		if (mNumber == 0) // если в папке нет файлов
		{
			mCurrentAbsFilePath.clear();
			return false;
		}
	} // if

    
    // получение, из списка имён файлов, списка имён файлов и списка абсолютных путей к файлам, по введённому слову
	return extractFiles(mStrListFiles);
}
#endif
#ifndef FIND_3 
#ifdef FIND_2
// Список файлоа из папки, и количество файлов в папке - поля класса
// Поиск файлов по слову, если найдены, то заполнение текущих списков
bool MyWidget::findFiles(QString& word)
{
	mCurrentWord = word; // текущее слово
	
#ifdef DEBUG
qDebug() << mCurrentDir.dirName();
#endif
// Если имя текущей папки пустое (первый поиск), или папка начинается на букву, отличную от первой буквы слова
// Иначе, будет использоавться тот же список файлов (из той же папки), что был до этого поиска 
	if (mCurrentDir.dirName().isEmpty() ||  mCurrentDir.dirName()[0] != word[0])
	{
		mCurrentDir = QDir(GlobalVariables::getGlobalVariables().PATH_SOUND + '/' + word[0]); // папка, на первую букву слова
		if (!mCurrentDir.exists()) // если нет такой папки
			return false;
#ifdef DEBUG
clock_t t1 = clock();
#endif
  mStrListFiles = mCurrentDir.entryList(/*QDir::Files*/); // список имён файлов из этой папки (с QDir::Files медленнее)
#ifdef DEBUG
clock_t t2 = clock();
qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
		mNumber = mStrListFiles.size();
		if (mNumber == 0) // если в папке нет файлов
		{
			mCurrentAbsFilePath.clear();
			return false;
		}
	} // if
#ifdef DEBUG
clock_t t1 = clock();
#endif
	// получение из списка имён файлов, списка имён файлов и списка абсолютных путей к файлам, по введённому слову
	mCurrentListFileName.clear();
	mCurrentListAbsFilePath.clear();
	
	for (quint32 i = 0; i < mNumber; ++i) // если список составлен без QDir::Files, то первые два можно пропустить ('.' и '..') 
	{
		if ((uiForm ->checkBox ->isChecked()) || // найти словосочетания
			(!uiForm ->checkBox ->isChecked() && 
			  word.contains(' ')                 ) )
		{
			//if (SoundFile::startsWithWord(strListFiles[i], word)) // всё, что начинается с word
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
		else // только слова
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

// Вариант со static
// Поиск файлов по слову, если найдены, то заполнение текущих списков
bool MyWidget::findFiles(QString& word)
{
	mCurrentWord = word; // текущее слово
	static QStringList strListFiles; // список имён файлов из папки //??? медленно в начале из-зи static?
	static quint32 number = 0; // количество файлов в папке
#ifdef DEBUG
qDebug() << mCurrentDir.dirName();
#endif
// Если имя текущей папки пустое (первый поиск), или папка начинается на букву, отличную от первой буквы слова
// Иначе, будет использоавться тот же список файлов (из той же папки), что был до этого поиска 
	if (mCurrentDir.dirName().isEmpty() ||  mCurrentDir.dirName()[0] != word[0])
	{
		mCurrentDir = QDir(GlobalVariables::getGlobalVariables().PATH_SOUND + '/' + word[0]); // папка, на первую букву слова
		if (!mCurrentDir.exists()) // если нет такой папки
			return false;
#ifdef DEBUG
clock_t t1 = clock();
#endif
  strListFiles = mCurrentDir.entryList(/*QDir::Files*/); // список имён файлов из этой папки (с QDir::Files медленнее)
#ifdef DEBUG
clock_t t2 = clock();
qDebug() << (t2 - t1 + .0) / CLOCKS_PER_SEC << endl;
#endif
		number = strListFiles.size();
		if (number == 0) // если в папке нет файлов
		{
			mCurrentAbsFilePath.clear();
			return false;
		}
	} // if
#ifdef DEBUG
clock_t t1 = clock();
#endif
	// получение из списка имён файлов, списка имён файлов и списка абсолютных путей к файлам, по введённому слову
	mCurrentListFileName.clear();
	mCurrentListAbsFilePath.clear();
	
	for (quint32 i = 0; i < number; ++i) // если список составлен без QDir::Files, то первые два можно пропустить ('.' и '..') 
	{
		if ((uiForm ->checkBox ->isChecked()) || // найти словосочетания
			(!uiForm ->checkBox ->isChecked() && 
			  word.contains(' ')                 ) )
		{
			//if (SoundFile::startsWithWord(strListFiles[i], word)) // всё, что начинается с word
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
		else // только слова
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
		if ((this ->uiForm ->checkBox ->isChecked()) || // найти словосочетания
			(!this ->uiForm ->checkBox ->isChecked() && 
  			  this ->mCurrentWord.contains(' ')                 ) )
		{
			//if (SoundFile::startsWithWord(strListFiles[i], word)) // всё, что начинается с word
			if (SoundFile::startsWithWordGroup(listFiles[i], this ->mCurrentWord) ||
				SoundFile::startsWithOneWord  (listFiles[i], this ->mCurrentWord)) 
			{
				GlobalVariables::getGlobalVariables().mutex.lock();
				this ->mCurrentListFileName.append(listFiles[i]);
				this ->mCurrentListAbsFilePath.append(this ->mCurrentDir.absoluteFilePath(listFiles[i]));
				GlobalVariables::getGlobalVariables().mutex.unlock();
			}
		}
		else // только слова
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
		if ((this ->uiForm ->checkBox ->isChecked()) || // найти словосочетания
			(!this ->uiForm ->checkBox ->isChecked() && 
			  this ->mCurrentWord.contains(' ')                 ) )
		{
			//if (SoundFile::startsWithWord(strListFiles[i], word)) // всё, что начинается с word
			if (SoundFile::startsWithWordGroup(listFiles[i], this ->mCurrentWord) ||
				SoundFile::startsWithOneWord  (listFiles[i], this ->mCurrentWord)) 
			{
				GlobalVariables::getGlobalVariables().mutex.lock();
				this ->mCurrentListFileName.append(listFiles[i]);
				this ->mCurrentListAbsFilePath.append(this ->mCurrentDir.absoluteFilePath(listFiles[i]));
				GlobalVariables::getGlobalVariables().mutex.unlock();
			}
		}
		else // только слова
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
//		if ((pWidg ->uiForm ->checkBox ->isChecked()) || // найти словосочетания
//			(!pWidg ->uiForm ->checkBox ->isChecked() && 
//			  pWidg ->mCurrentWord.contains(' ')                 ) )
//		{
//			//if (SoundFile::startsWithWord(strListFiles[i], word)) // всё, что начинается с word
//			if (SoundFile::startsWithWordGroup(listFiles[i], pWidg ->mCurrentWord) ||
//				SoundFile::startsWithOneWord  (listFiles[i], pWidg ->mCurrentWord)) 
//			{
//				pWidg ->mCurrentListFileName.append(listFiles[i]);
//				pWidg ->mCurrentListAbsFilePath.append(pWidg ->mCurrentDir.absoluteFilePath(listFiles[i]));
//			}
//		}
//		else // только слова
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


// Вывод имён найденных файлов
void MyWidget::showFilesFound()
{
	uiForm ->listWidgetFiles ->clear();
	
	// вывод, в список виджетов, списка имён найденных файлов
	uiForm ->listWidgetFiles ->addItems(mCurrentListFileName);
	
	// выделение в списке имени текущего файла
	//uiForm ->listWidgetFiles ->setCurrentRow(mCurrentIndex);
	
	/*uiForm ->listWidgetFiles ->setCurrentRow(mCurrentListAbsFilePath.indexOf(mCurrentAbsFilePath));*/
#ifdef DEBUG
#endif	
	// вывод в labelNote, примечаний (Note) текущего SoundFile, в виде строки
	//uiForm ->labelNote ->setText(mpCurrentSoundFile ->getNote().stringForNote());
}

void MyWidget::choiceItemFromList(QListWidgetItem* item)
{
#ifdef DEBUG
qDebug() << QWidget::tr("Выбранный файл");
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
		//uiForm ->labelNote ->setText(mpCurrentSoundFile ->getNote().stringForNote()); // вывод примечания (Note), текущего SoundFile, строкой
#ifdef DEBUG
qDebug() << QWidget::tr("Изменился!");
#endif	
	}
#ifdef DEBUG
	else
		qDebug() << QWidget::tr("Не изменился!");
#endif
	
	uiForm ->labelOutput ->setText(mCurrentWord);
	uiForm ->lineEditInput ->setText(mCurrentWord);
	play(mCurrentAbsFilePath); // воспроизведение текущего файла
}

void MyWidget::addCache()
{
	// добавление в кеш слова и звукового файла (пути)
	if (mCacheFiles.find(mCurrentWord) == mCacheFiles.end())
	{
		mCacheFiles.insert(mCurrentWord, mCurrentAbsFilePath);
		
		//mStrListCacheWord.append(mCurrentWord);
		//mStrListCacheWord.sort();
		// вывод кеша
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

void MyWidget::choiceItemFromCacheWord(QListWidgetItem* item) // выбор слова из кеша
{
	//uiForm ->lineEditInput ->setText(item ->text());
	//mCurrentAbsFilePath = mCacheFiles.value(item ->text());
	QString AbsFilePath = mCacheFiles.value(item ->text());
	//mpClipboard ->setText(item ->text());
	play(AbsFilePath); // воспроизведение файла 
	
    //pressedEnter();
}

// Воспроизведение файла
void MyWidget::play(QString& AbsFilePath)
{
	qDebug() << "mCurrentAbsFilePath = " << AbsFilePath;
	if (!GlobalVariables::getGlobalVariables().LIST_EXT.contains(SoundFile::extractExt(AbsFilePath)))
	{
		uiForm ->labelOutput ->setText(QWidget::tr("Файл не подходит\nдля воспроизведения!"));
		return;
	}
	mMyPlayer.play(AbsFilePath); // воспроизведение файла
}

// Установка нового текущего индекса в списках
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
		
// Проверка слова из строки ввода, для выбора дальнейших действий
int MyWidget::checkWord(const QString& word)
{
	 // пустое слово
	if (word.isEmpty())
		return 1;
	// неподустимый символ в слове (тире в начале, тире в конце, не английская буква и не пробел,
	// два тире подряд или не подряд
	QRegExp reg("^-|[^a-z- ]|-$|-.*-", Qt::CaseInsensitive);
	//QRegExp reg2("[a-z]+|-",Qt::CaseInsensitive);
	//for (int i = 0; i < word.size(); ++i)
	//{
		if (reg.indexIn(word) != -1) 
			return 2;
	//}
	// если введено то же самое слово, что не было найдено перед этим (лист найденных файлов - пуст)
	if (mCurrentAbsFilePath == "" && mCurrentWord == word) 
		return 3;
	
	// если указано вывести словосочетания, 
	// то запустить поиск файлов 
	if (uiForm ->checkBox ->isChecked())
		return 6;
	
	// если не указано вывести словосочетания, 
	// и слово (или словосочетание как слово) есть в текущем списке
	if (!uiForm ->checkBox ->isChecked() && containsFileName(mCurrentListFileName, word))
	{
		if (mCurrentListFileName.size() > 1) // в списке несколко файлов
		{
			uiForm ->listWidgetFiles ->clear();
			qDebug() << "if c p";
			QStringList newList;
			bool flag = false;
				
			for (int i = 0; i < mCurrentListFileName.size(); ++i) // новый список файлов с этим словом 
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
			newList.swap(mCurrentListFileName); // делаем новый список текущим
			mCurrentListAbsFilePath.clear();
				
			for (int i = 0; i < mCurrentListFileName.size(); ++i) // новый список путей
				mCurrentListAbsFilePath.append(mCurrentDir.absoluteFilePath(mCurrentListFileName[i]));
		
			uiForm ->listWidgetFiles ->addItems(mCurrentListFileName);
			return 5;
		}
		return 4; // в списке один файл
	}
	
	return 6; // поиск файлов
}

// Слот. В строке ввода нажата Enter
void MyWidget::pressedEnter()
{
#ifdef DEBUG 
qDebug() << QWidget::tr("Нажата Enter");
#endif
    QString word = (uiForm ->lineEditInput ->text()).trimmed().toLower();
	uiForm ->lineEditInput ->setText(word);
	int ind = 0;
	switch (checkWord(word))
	{
	case 1 : 
		uiForm ->labelOutput ->setText(QWidget::tr("Введите слово!"));
		break;
	case 2 :
		uiForm ->labelOutput ->setText(QWidget::tr("Недопустимые символы\nв слове!"));
		break;
	
	case 3 :
#ifdef DEBUG 
qDebug() <<  QWidget::tr("Нажата Enter. Не найденное слово не изменилось!");
#endif
        uiForm ->labelOutput ->setText(QWidget::tr("Слово не найдено!"));
		break;
	case 4 : // файл со словом есть в текущем списке, и текущий файл равен введённому слову
#ifdef DEBUG 
qDebug() <<  QWidget::tr("Нажата Enter. Найденный файл не изменился!");
#endif
		//setNewCurrentIndex(getIndexString(mCurrentListFileName, word, "OneWord"));
        
		play(mCurrentAbsFilePath); // воспроизведение текущего файла
		break;
	case 5 : // файл со словом есть в текущем списке, но текущий файл не равен введённому слову
#ifdef DEBUG 
qDebug() <<  QWidget::tr("Нажата Enter. Найденный файл изменился!");
#endif
		ind = getIndexString(mCurrentListFileName, word, "OneWord"); // поиск в списке файла из одного введённого слова
		if (ind != -1) // если такой файл есть в списке
		{
			setNewCurrentIndex(ind); // установка нового текущего индекса
		
			play(mCurrentAbsFilePath); // воспроизведение текущего файла
			break;
		}
		// если такого файла в списке нет (т.е., есть только словосочетания с введённым словом), то поиск файлов
	default :
		// слово - в буффер обмена
		mpClipboard ->setText(word);
#ifdef DEBUG	
qDebug() <<  QWidget::tr("Поиск файлов!");
#endif
		
	    // поиск звуковых файлов по слову
		if (findFiles(word)) // если файлы существуют
		{
			showFilesFound(); // вывод в список имён найденных файлов
			// новый текущий индекс (в списке - это индекс самого короткого имени файла (основное слово))
			setNewCurrentIndex(getIndSmallestElement(mCurrentListFileName)); 
			
			uiForm ->labelOutput ->setText(mCurrentWord);
			
			//mpClipboard ->setText(mCurrentWord);
			
			play(mCurrentAbsFilePath); // воспроизведение текущего файла
			
			addCache(); // добавление слова и файла в кеш
		}
		else // если файлов, с таким словом, нет
		{
			if (word.contains(' '))
				uiForm ->labelOutput ->setText(QWidget::tr("Словосочетание\nне найдено!"));
			else
				uiForm ->labelOutput ->setText(QWidget::tr("Слово не найдено!"));
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

// Переключение на английский ввод при активном окне
bool MyWidget::event(QEvent* pe) 
{
	if (pe ->type() == QEvent::WindowActivate)
	{
#ifdef Q_OS_WIN32
		
			qDebug() << "define Q_OS_WIN32";
			PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 1, 0x04090409);
			// LoadKeyboardLayout(L"00000409", KLF_ACTIVATE); // или так
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

// Обработка нажатия клавиш управления
void MyWidget::keyPressEvent(QKeyEvent* pe)
{
	
	int codeKey = pe ->key();
	
	//qDebug() << pe ->nativeVirtualKey();
	//qDebug() << codeKey;
	//qDebug() << (pe ->modifiers() & Qt::ShiftModifier);
	
	Qt::KeyboardModifiers keyModifiers = pe ->modifiers();
	
	// Установка и снятие radioButtonRepeat ("Повторять")
	if (codeKey == Qt::Key_Right && (keyModifiers & Qt::ShiftModifier) && (keyModifiers & Qt::AltModifier)) // если Shift + Alt + ->
	{
		changeStateButton(uiForm ->radioButtonRepeat);
		
		QWidget::keyPressEvent(pe);
	    return;
	}
	
	// Установка и снятие checkBox ("Вывести словосочетания")
	if (codeKey == Qt::Key_Right && (keyModifiers & Qt::AltModifier)) // если Alt + ->
	{
		changeStateButton(uiForm ->checkBox);
		
		uiForm ->lineEditInput ->setFocus();
		QWidget::keyPressEvent(pe);
	    return;
	}
	//....................................................

	if (uiForm ->listWidgetFiles ->count() != 0)
	{
		switch (codeKey)
		{
			case Qt::Key_Down : // если стрелка вниз и
				
				if (uiForm ->lineEditInput ->hasFocus()) // строка ввода в фокусе
				{
					uiForm ->listWidgetFiles ->setFocus(); // переход фокуса в лист виджетов
					uiForm ->listWidgetFiles ->setCurrentRow(0);
				}
				// последняя строка, в листе виджетов, в фокусе 
				else if (uiForm ->listWidgetFiles ->item(uiForm ->listWidgetFiles ->count() - 1) ->isSelected())
							uiForm ->lineEditInput ->setFocus(); // переход фокуса в строку ввода
				break;
			case Qt::Key_Up : // если стрелка вверх и
				
				if (uiForm ->listWidgetFiles ->hasFocus() && uiForm ->listWidgetFiles ->item(0) ->isSelected()) // первый элемент, в листе виджетов, в фокусе
					uiForm ->lineEditInput ->setFocus(); // переход фокуса в строку ввода
				else if(uiForm ->lineEditInput ->hasFocus()) // строка ввода в фокусе
					 {
						uiForm ->listWidgetFiles ->setFocus(); // переход фокуса в лист виджетов
						uiForm ->listWidgetFiles ->setCurrentRow(uiForm ->listWidgetFiles ->count() - 1);
					 }
				break;
		}
		
		QWidget::keyPressEvent(pe);
	}
	else
	{
		uiForm ->lineEditInput ->setFocus();
		QWidget::keyPressEvent(pe);
	}
}

// Добавление файлов
////////////////////////////////////////////////////////////////////////////

void MyWidget::reject()
{

}

void MyWidget::accept(QString& fileName, QString& fileNewName) // замена файла
{
	if (mDirSound.remove(mDirSound.absoluteFilePath(fileNewName)))
	{
		saveFile(fileName, fileNewName);
		uiForm ->labelOutput ->setText(QWidget::tr("Файл успешно заменён!"));
	}
	else
        uiForm ->labelOutput ->setText(QWidget::tr("Ошибка при удалении\nстарого файла!"));
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
        
		uiForm ->labelOutput ->setText(QWidget::tr("Файл успешно добавлен!"));
	}
    else
	{
        uiForm ->labelOutput ->setText(QWidget::tr("Ошибка\nпри копировании файла!"));
		uiForm ->lineEditInput ->setFocus();
	}
}

void MyWidget::choiceFileInExplorer()
{
    QString fileName = QFileDialog::getOpenFileName(this, QWidget::tr("Выбор файла"), 
                                           QDir::homePath(), GlobalVariables::getGlobalVariables().FILTER);
#ifdef DEBUG
qDebug() << fileName;
#endif
    if (fileName.isEmpty())
    {
        uiForm ->labelOutput ->setText(QWidget::tr("Файл не выбран!"));
		uiForm ->lineEditInput ->setFocus();
		return;
    }
    
	QString fileNewName = SoundFile::extractName(fileName); // имя выбранного файла с расширением
    QString word = SoundFile::extractWord(fileNewName); // слово в имени файла
	// поиск звуковых файлов по слову
	if (findFiles(word)) // если файлы существуют
	{
		mCurrentWord = word; // текущее слово
		uiForm ->labelOutput ->setText(word);
		showFilesFound(); // вывод в список имён найденных файлов 
		
	    mpMessageBox = new QMessageBox(this);
	    QObject::connect(mpMessageBox, SIGNAL(accepted()), this, SLOT(acceptMessBox()));
		
		mpMessageBox ->setIcon(QMessageBox::Question);
		mpMessageBox ->setWindowTitle(QWidget::tr("Добавление файла"));
		mpMessageBox ->setText(QWidget::tr("Файл уже существует!\nЗаменить или добавить?"));
		
		QPushButton* changeButton = mpMessageBox ->addButton(QWidget::tr("Заменить"), QMessageBox::AcceptRole);
		QPushButton* appendButton = mpMessageBox ->addButton(QWidget::tr("Добавить"), QMessageBox::RejectRole);
		QPushButton* cancelButton = mpMessageBox ->addButton(QWidget::tr("Отмена"),   QMessageBox::RejectRole);
		
		mpMessageBox ->exec();
		if (mpMessageBox ->clickedButton() == changeButton)
		{
				//accept(fileName, fileNewName); // замена файла
				 uiForm ->labelOutput ->setText(QWidget::tr("Файл заменён!"));
				 uiForm ->lineEditInput ->setFocus();
		}
		else if (mpMessageBox ->clickedButton() == appendButton)
		{
			     uiForm ->labelOutput ->setText(QWidget::tr("Файл добавлен!"));
				 uiForm ->lineEditInput ->setFocus();
		}
		else if (mpMessageBox ->clickedButton() == cancelButton)
		{
				 uiForm ->labelOutput ->setText(QWidget::tr("Отмена!"));
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


