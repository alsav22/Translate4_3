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
	QString dirDict; // папка словаря
	QString mName; // имя словаря 
	QString fileIfo; // имя файла словаря .ifo
	QString fileIdx; // имя файла словаря .idx
	QString fileDict; // имя файла словаря .dict
	QString fileHash;
	QString fileParseIdx;
	QString fileParseIfo;
	QString ifoWordcount;   // первое поле из .ifo
	QString ifoIdxfilesize; // второе поле из .ifo
    quint32 offset; // под размер смещения
	quint32 size;   // под размер записи
	quint32 wordcount; // количество слов в словаре
	quint32 idxfilesize; // размер файла .idx

	QFile* mpFileDict; // объект для работы с файлом словаря .dict
};
	
#endif // DICTIONARY_H


