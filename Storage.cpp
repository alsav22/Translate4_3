#include <QDir>
#include <QStringList>
#include <QString>
#include <QMultiMap>
#include "Storage.h"
#include "SoundFile.h"
#include "ui_Form.h"


void Storage::createStorage(const QDir& dirSound, Ui::Form* ui) // заполнение MultiMap словами и, соответствующими им, SoundFile
{
	uiForm = ui;
	QStringList strLstFiles = dirSound.entryList(QDir::Files);
	if (strLstFiles.isEmpty())
	{
		ui ->labelOutput ->setText(QWidget::tr("Звуковые файлы\nне найдены!"));
		return;
	}
	
	for (int i = 0; i < strLstFiles.size(); ++i)
	{
		Note note;
		addSoundFile(dirSound.absoluteFilePath(strLstFiles[i]), note);
	}
}

SoundFile* Storage::addSoundFile(const QString& absPath, const Note& note)
{
	SoundFile* pSoundFile = new SoundFile(absPath, note);
	mMulMap.insert(pSoundFile ->getWord(), pSoundFile);
	return pSoundFile;
}

Storage::~Storage()
{
	QMultiMap <QString, SoundFile*>::iterator i = mMulMap.begin();
	for ( ; i != mMulMap.end(); ++i)
		delete i.value();
}
