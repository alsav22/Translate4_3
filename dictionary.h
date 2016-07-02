#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QtGui>

class Dictionary 
{
	//friend class DictProgram;

public:
	Dictionary(const QString& dirName, const QString name);
	~Dictionary();
	
	inline void setNameFiles();
	inline bool parsingIdx();
	inline bool parsingIfo();
	inline bool createHash();
	inline bool loadHash();
	inline bool loadData();
	inline void HTMLfromString(QString& str);
	inline void formattingTr(QString& str);
	QString getTr(const QString& word);
	QString getName();
	
protected:
	friend void getTagForDict(Dictionary* p);

private:
	QHash <QString, QPair <quint32, quint32> > mHash;
	QString dirDict; // ����� �������
	QString mName; // ��� ������� 
	QString fileIfo; // ��� ����� ������� .ifo
	QString fileIdx; // ��� ����� ������� .idx
	QString fileDict; // ��� ����� ������� .dict
	QString fileHash;
	QString fileParseIdx;
	QString fileParseIfo;
	QString ifoWordcount;   // ������ ���� �� .ifo
	QString ifoIdxfilesize; // ������ ���� �� .ifo
    quint32 offset; // ��� ������ ��������
	quint32 size;   // ��� ������ ������
	quint32 wordcount; // ���������� ���� � �������
	quint32 idxfilesize; // ������ ����� .idx

	QFile* mpFileDict; // ������ ��� ������ � ������ ������� .dict
};
	
#endif // DICTIONARY_H


