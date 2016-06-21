#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <QHash.h>
#include <QString>

class Dictionary : QHash <QString, QString>
{
public:
	Dictionary(const QString fileName, const QString hashName);

private:
	bool create();
	bool load();
	bool save();
	void show(const char* key);

	const QString mfile;
	const QString mhash;
};
#endif