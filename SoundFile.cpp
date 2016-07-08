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

QString SoundFile::extractName(const QString& absPath) // ��������� ���� � �����, ���������� ��� ����� � �����������
{
	return absPath.mid(absPath.lastIndexOf(GlobalVariables::getGlobalVariables().slash) + 1);
}

QString SoundFile::extractExt(const QString& absPath) // ��������� ���� � �����, ���������� ���������� �����
{
	return absPath.mid(absPath.lastIndexOf('.') + 1);
}

// �� �������������� ����� ��� �����, ���� ����� ���� - ����� �����
QString SoundFile::extractWordGroup(QString fileName) // ��������� ��� ����� � �����������,
	                                                  // ���������� ����� (��� �����) �� ������� delim ('_'),
												      // ���� delim ���, �� �� ��������� '.'
{
	int i = fileName.indexOf(GlobalVariables::getGlobalVariables().delim);
	if (i == -1)
		i = fileName.lastIndexOf('.');
	if (i != -1)
		fileName.truncate(i);
	return fileName;
}

// �� �������������� ����� ������ �����, ���� ����� ���� - ����� �����
QString SoundFile::extractWord(QString fileName) // ��������� ��� ����� � �����������,
	                                             // ���������� ����� �� ������� �������,
												 // ���� ������� ���, �� �� ������� delim ('_'),
												 // ���� delim ���, �� �� ��������� '.'
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

//// ��������� ��� ����� � ����������� � �����,
//// ���������� true, ���� fileName ���������� � word � ������ ��� ������
//// �.�., ���� ��� ����� ������ � �������������� 
//bool SoundFile::containsWordGroup(const QString& fileName, const QString& word) 
//{
//	int end = word.size();
//	if (fileName.startsWith(word, Qt::CaseInsensitive) && fileName[end] == ' ')
//	    return true;
//	return false;
//}

// ��������� ��� ����� � ����������� � �����,
// ���������� true, ���� fileName ���������� � word � ������ ��� ����� ��� delim ('_')
// �.�., ���� ����� ����
bool SoundFile::startsWithOneWord(const QString& fileName, const QString& word) 
{
	/*int end = word.size();
	if (fileName.startsWith(word, Qt::CaseInsensitive) &&
	   (fileName[end] == '.'   ||
	    fileName[end] == GlobalVariables::getGlobalVariables().delim))
			return true;
	return false;*/
	
	QRegExp reg(QString("^"  + QRegExp::escape(word) + "[" + GlobalVariables::getGlobalVariables().delim + "\\.]"));
	
	if (reg.indexIn(fileName) != -1)
			return true;
	return false;
}

// ��������� ��� ����� � ����������� � �����,
// ���������� true, ���� fileName ���������� � word � ������ ��� ������, ��� '-',
// �.�., ���� ����� ������ � �������������� (�����, � ��������������, ��������� �������� ��� '-')
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
	
	QRegExp reg(QString("^" +  QRegExp::escape(word) + "[- ]")/*, Qt::CaseInsensitive*/);
	if (reg.indexIn(fileName) != -1)
			return true;
	return false;
}

// ��������� ��� ����� � ����������� � �����,
// ���������� true, ���� fileName ���������� � word 
bool SoundFile::startsWithWord(const QString& fileName, const QString& word) 
{
	/*if (fileName.startsWith(word, Qt::CaseInsensitive))
	   return true;
	return false;*/
	
	QRegExp reg(QString("^" + QRegExp::escape(word))/*, Qt::CaseInsensitive*/);
	
	if (reg.indexIn(fileName) != -1)
			return true;
	return false;
}

// ��������� �����, ���������� true, ���� word ��������������
bool SoundFile::WordGroup(const QString& word)
{
	/*if (word.contains(' ') || word.contains('-'))
		return true;
	return false;*/

	QRegExp reg("[- ]");
	if (reg.indexIn(word/*, Qt::CaseInsensitive*/) != -1)
		return true;
	return false;
}
