#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QtGui>

class Dictionary 
{
public:
	Dictionary(const QString& dirName, const QString name);
	~Dictionary();
	
	QString getTr(const QString& word);
	QString getName();
	
private:
	QHash <QString, QPair <quint32, quint32> > mHash;
	QString dirDict;  // ����� �������
	QString mName;    // ��� ������� 
	QString fileIfo;  // ��� ����� ������� .ifo
	QString fileIdx;  // ��� ����� ������� .idx
	QString fileDict; // ��� ����� ������� .dict
	QString fileHash;
	QString fileParseIdx;
	QString fileParseIfo;
	QString ifoWordcount;   // ������ ���� �� .ifo
	QString ifoIdxfilesize; // ������ ���� �� .ifo
    quint32 wordcount;      // ���������� ���� � �������
	quint32 idxfilesize;    // ������ ����� .idx

	QFile* mpFileDict; // ������ ��� ������ � ������ ������� .dict

	inline void setNameFiles();
	inline bool parsingIdx();
	inline bool parsingIfo();
	inline bool createHash();
	inline bool loadHash();
	inline bool loadData();
};
	
#endif // DICTIONARY_H


