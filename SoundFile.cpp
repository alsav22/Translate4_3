#include <QRegExp>
#include "SoundFile.h"
#include "GVariables.h"

//extern const QChar delim;
//extern const QChar slash;

SoundFile::SoundFile(const QString& absPath, const Note& note)
	: QFile(absPath), mNote(note)
{
	mName = extractName(absPath);
	mExt =  extractExt (absPath);
	mWord = extractWord(mName  );
}

SoundFile::~SoundFile() { }

QString SoundFile::extractName(const QString& absPath) // принимает путь к файлу, возвращает имя файла с расширением
{
	return absPath.mid(absPath.lastIndexOf(GlobalVariables::getGlobalVariables().slash) + 1);
}

QString SoundFile::extractExt(const QString& absPath) // принимает путь к файлу, возвращает расширение файла
{
	return absPath.mid(absPath.lastIndexOf('.') + 1);
}

// из словосочетания вернёт все слова, если слово одно - вернёт слово
QString SoundFile::extractWordGroup(QString fileName) // принимает имя файла с расширением,
	                                                  // возвращает слова (или слово) до первого delim ('_'),
												      // если delim нет, то до последней '.'
{
	int i = fileName.indexOf(GlobalVariables::getGlobalVariables().delim);
	if (i == -1)
		i = fileName.lastIndexOf('.');
	if (i != -1)
		fileName.truncate(i);
	return fileName;
}

// из слососочетания вернёт первое слово, если слово одно - вернёт слово
QString SoundFile::extractWord(QString fileName) // принимает имя файла с расширением,
	                                             // возвращает слово до первого пробела,
												 // если пробела нет, то до первого delim ('_'),
												 // если delim нет, то до последней '.'
{
	int i = fileName.indexOf(' ');
	if (i == -1)
		i = fileName.indexOf(GlobalVariables::getGlobalVariables().delim);
	if (i == -1)
		i = fileName.lastIndexOf('.');
	if (i != -1)
		fileName.truncate(i);
	return fileName;
}

//// принимает имя файла с расширением и слово,
//// возвращает true, если fileName начинается с word и дальше идёт пробел
//// т.е., если это слово первое в словосочетании 
//bool SoundFile::containsWordGroup(const QString& fileName, const QString& word) 
//{
//	int end = word.size();
//	if (fileName.startsWith(word, Qt::CaseInsensitive) && fileName[end] == ' ')
//	    return true;
//	return false;
//}

// принимает имя файла с расширением и слово,
// возвращает true, если fileName начинается с word и дальше идёт точка или delim ('_')
// т.е., если слово одно
bool SoundFile::startsWithOneWord(const QString& fileName, const QString& word) 
{
	/*int end = word.size();
	if (fileName.startsWith(word, Qt::CaseInsensitive) &&
	   (fileName[end] == '.'   ||
	    fileName[end] == delim   )  )
			return true;
	return false;*/

	
	QRegExp reg(QString("^" + word + "[" + GlobalVariables::getGlobalVariables().delim + "\\.]"), 
		                                                                    Qt::CaseInsensitive);
	if (reg.indexIn(fileName) != -1)
			return true;
	return false;
}

// принимает имя файла с расширением и слово,
// возвращает true, если fileName начинается с word и дальше идёт пробел, или '-',
// т.е., если слово первое в словосочетании (слова, в словосочетании, соединены пробелом или '-')
bool SoundFile::startsWithWordGroup(const QString& fileName, const QString& word) 
{
	/*int end = word.size();
	if (fileName.startsWith(word, Qt::CaseInsensitive) &&
		   (fileName[end] == ' ' ||
			fileName[end] == '-' ||
			fileName[end] == '.' ||
			fileName[end] == delim )  )
			return true;
	return false;*/
	
	QRegExp reg(QString("^" + word + "[- ]"), Qt::CaseInsensitive);
	if (reg.indexIn(fileName) != -1)
			return true;
	return false;
}

// принимает имя файла с расширением и слово,
// возвращает true, если fileName начинается с word 
bool SoundFile::startsWithWord(const QString& fileName, const QString& word) 
{
	/*if (fileName.startsWith(word, Qt::CaseInsensitive))
	   return true;
	return false;*/

	QRegExp reg(QString("^" + word), Qt::CaseInsensitive);
	if (reg.indexIn(fileName) != -1)
			return true;
	return false;
}

// пнинимает слово, возвращает true, если word словосочетание
bool SoundFile::WordGroup(const QString& word)
{
	/*if (word.contains(' ') || word.contains('-'))
		return true;
	return false;*/

	QRegExp reg("[- ]");
	if (reg.indexIn(word, Qt::CaseInsensitive) != -1)
		return true;
	return false;
}
