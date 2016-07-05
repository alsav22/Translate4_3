#include "dictionary.h"
#include <QDebug>
#include <qregexp.h>
#include <QtGui>
#include <string>
#include <sstream>
#include <fstream>

Dictionary::Dictionary(const QString& dirName, const QString name) : dirDict(dirName), mName(name), ifoWordcount("wordcount"), 
	                                                                 ifoIdxfilesize("idxfilesize"), wordcount(0), idxfilesize(0), 
																	 mpFileDict(nullptr)
{
	setNameFiles();
	
	if (!loadData())
	{
		qDebug() << "Error loadData()!";
	}
	//getTagForDict(this);
}

QString Dictionary::getName()
{
	return mName;
}

void Dictionary::setNameFiles()
{
	QDir dir(dirDict);
	QStringList list = dir.entryList(QDir::Files | QDir::NoDotAndDotDot); // список файлов из папки со словарём
    
	// переменным, с именами файлов словаря, устанавливаются имена файлов, с нужным расширением, из папки словаря 
	if (!list.empty())
	{
		foreach(QString s, list)
		{
			if (s.endsWith(".dict"))
				fileDict = dirDict + "/" + s;
	        else if (s.endsWith(".idx"))
				fileIdx  = dirDict + "/" + s;
			else if (s.endsWith(".ifo"))
				fileIfo  = dirDict + "/" + s;
		}
	}
	
	fileHash     = dirDict + "/" + "Hash.txt";
	fileParseIfo = dirDict + "/" + "parseIfo.txt";
	fileParseIdx = dirDict + "/" + "parseIdx.txt";
}

bool Dictionary::loadData()
{
	if (!QFile::exists(fileParseIfo))
    if (!parsingIfo())
	{
		qDebug() << "Error parsingIfo()!";
		return false;
	}
	if (!QFile::exists(fileParseIdx))
    if (!parsingIdx())
	{
		qDebug() << "Error parsingIdx()!";
		return false;
	}
	if (!QFile::exists(fileHash))
    if (!createHash())
	{
		qDebug() << "Error createHash()!";
		return false;
	}
	if (!loadHash())
	{
		qDebug() << "Error loadHash()!";
		return false;
	}
	mpFileDict = new QFile(fileDict);
	if (!mpFileDict ->open(QIODevice::ReadOnly))
	{
		qDebug() << "Error opening " << fileDict << " !";
		return false;
	}
	return true;
}

bool Dictionary::parsingIfo()
{
	QFile fileIn(fileIfo);
	QFile fileOut(fileParseIfo);
	if (fileIn.open(QIODevice::ReadOnly) && fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream in(&fileIn);
		QTextStream out(&fileOut); 
		QString str;
		QString number;
		while (true)
		{
			str = in.readLine();
			if (in.atEnd())
				break;
			if (str.startsWith(ifoWordcount))
			{
				number = str.mid(ifoWordcount.size() + 1);
				wordcount = number.toUInt();
				out << wordcount << endl;
			}
			if (str.startsWith(ifoIdxfilesize))
			{
				number = str.mid(ifoIdxfilesize.size() + 1);
				idxfilesize = number.toUInt();
				out << idxfilesize << endl;
			}
		}
		if (wordcount != 0 && idxfilesize != 0)
			return true;
	}
	
	qDebug() << "Error parsingIfo()!";
	return false;
}

// из .idx файла формируется файл формата: строка с именем слова для перевода, 
// следующая строка - два числа через пробел: смещение для первода в файле .dict и
// размер перевода в файле .dict
bool Dictionary::parsingIdx()
{
	QFile fileIn(fileIdx);
	QFile fileOut(fileParseIdx);
	if (fileIn.open(QIODevice::ReadOnly) && fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QDataStream in(&fileIn);
		QTextStream out(&fileOut); 
		QString str;
		QByteArray arr;
		
		while (!in.atEnd())
		{
			char ch ;
			if (in.readRawData(&ch, 1) == -1)
			{
				qDebug() << QWidget::tr("Ошибка чтения из .idx!");
				return false;
			}
			else
			{
				if (ch)
					arr.append(ch);
				else
				{
					quint32 offset;
					quint32 size;
					in >> offset >> size;
					str = QString::fromUtf8(arr.data());
					out << str << '\n' << offset << " " << size << '\n'; 
					arr.clear();
				}
			}
		}
	}
	else
	{
		qDebug() << "Error parsingIdx()!";
		return false;
	}
	return true;
}

// Создание и запись в файл хеша из данных в .idx: ключ - слово для перевода, значение - пара из смещения и размера первода в .dict
bool Dictionary::createHash()
{
	QFile fileIn(fileParseIdx);
	QFile fileOut(fileHash);
	if (fileIn.open(QIODevice::ReadOnly | QIODevice::Text) && fileOut.open(QIODevice::WriteOnly))
	{
		QTextStream in(&fileIn); 
		QString str;
		quint32 offset = 0;
		quint32 size   = 0;
		while (true)
		{
			str = in.readLine();
			if (in.atEnd())
				break;
			//qDebug() << str;
			in >> offset;
			in >> size;
			in.read(1); // пропуск символа(ов) новой строки перед readLine()

			mHash.insert(str, qMakePair(offset, size));
		}
		
		QDataStream out(&fileOut);
		out << mHash;
	    return true;
	}
	qDebug() << "Error createHash()!";
	return false;
}
// загрузка хеша с данными из .idx
bool Dictionary::loadHash()
{
	QFile fileIn(fileHash);
	if (fileIn.open(QIODevice::ReadOnly))
	{
		QDataStream in(&fileIn);
		in >> mHash;
		return true;
	}
	qDebug() << "Error loadHash()!";
	return false;
}

// получение первода из .dict
QString Dictionary::getTr(const QString& word)
{
	QString translation;
	if (!word.isEmpty())
	{
		if (mHash.contains(word))
		{
			QPair <quint32, quint32> pair(mHash.value(word)); // получение смещения и размера из хеша
			quint32 offset = pair.first;
			quint32 size   = pair.second;
			
			char* buffer = new char[size + 1]; // буфер под перевод
			mpFileDict ->seek(0);
			mpFileDict ->seek(offset); // переход к переводу
			mpFileDict ->read(buffer, size); // чтение перевода
			buffer[size] = '\0';
			
			translation = QString::fromUtf8(buffer); // первод в QString из UTF-8
			delete buffer;
		}
	}
	return translation;
}

Dictionary::~Dictionary()
{
	if (mpFileDict)
		mpFileDict ->close();
	delete mpFileDict;
}
