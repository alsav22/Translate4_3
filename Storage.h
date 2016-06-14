#ifndef STORAGE_H
#define STORAGE_H
#include <QObject>
#include <QWidget>
#include <QMultiMap>
#include <QStringList>
//#include "SoundFile.h"

class SoundFile;
class Note;
class QDir;

namespace Ui 
{
    class Form;
}

class Storage : public QObject
{
	Q_OBJECT

    Ui::Form* uiForm;

public:
	struct StorageStringLists
	{
		QStringList stringListAbsPath;
		QStringList stringListName;
		QStringList stringListExt;
		QStringList stringListNote;
	};
	
	QMultiMap <QString, SoundFile*> mMulMap;

public:
	void createStorage(const QDir& dirSound, Ui::Form* ui);
	
    SoundFile* addSoundFile(const QString& absPath, const Note& note);
	
	~Storage();
	
	const QMultiMap <QString, SoundFile*>& getmMulMap() const
	{ return mMulMap; }
};

#endif // STORAGE_H
