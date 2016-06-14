#include "MyPlayer.h"
#include "ui_Form.h"
#include <QMessageBox>
#include <qapplication.h>

void MyPlayer::createMyPlayer(Ui::Form* ui)
{
	mpAudioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mpMediaObject = new Phonon::MediaObject(this);
    Phonon::createPath(mpMediaObject, mpAudioOutput);
	
	mpAudioOutput ->setVolume(0.5); // задаёт начальное значение ползунка
	
	uiForm = ui;
	uiForm ->volumeSlider ->setAudioOutput(mpAudioOutput);
	
	QObject::connect(this, SIGNAL(clearMediaObject()), mpMediaObject, SLOT(clear()));
	QObject::connect(mpMediaObject, SIGNAL(finished()), this, SLOT(startPlay()));
	QObject::connect(mpAudioOutput, SIGNAL(volumeChanged(qreal)), uiForm ->lineEditInput, SLOT(setFocus()));
	QObject::connect(ui -> lineEditInput, SIGNAL(textChanged(QString)), mpMediaObject, SLOT(clear()));
}

void MyPlayer::play(const QString& fileName)
{
    mFileName = fileName;
	if (!uiForm ->radioButtonRepeat ->isChecked())
		countLoopPlay = 1;
	else
		countLoopPlay = uiForm ->spinBox ->value();
	
	mpMediaObject ->clear(); // остановка воспроизведения и очистка очереди
	startPlay();
}

void MyPlayer::startPlay()
{
	if (countLoopPlay)
	{
		mpMediaObject ->setCurrentSource(Phonon::MediaSource(mFileName));
		
		mpMediaObject ->play();
		
		--countLoopPlay;
	}
	else
		emit clearMediaObject(); // остановка воспроизведения и очистка очереди
}
