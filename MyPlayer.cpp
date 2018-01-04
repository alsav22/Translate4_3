#include "MyPlayer.h"
#include "ui_Form.h"
#include <QMessageBox>
#include <qtimer.h>
#include <qapplication.h>

void MyPlayer::createMyPlayer(Ui::Form* ui)
{
	mpAudioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mpMediaObject = new Phonon::MediaObject(this);
    Phonon::createPath(mpMediaObject, mpAudioOutput);
	
	mpAudioOutput ->setVolume(0.5); // ����� ��������� �������� ��������
	
	uiForm = ui;
	uiForm ->volumeSlider ->setAudioOutput(mpAudioOutput);
	//valueSpinBox = uiForm ->spinBox ->value();
	
	QObject::connect(this, SIGNAL(clearMediaObject()), mpMediaObject, SLOT(clear()));
	QObject::connect(mpMediaObject, SIGNAL(finished()), this, SLOT(startPlay())); // �������
	QObject::connect(mpAudioOutput, SIGNAL(volumeChanged(qreal)), uiForm ->lineEditInput, SLOT(setFocus()));
	QObject::connect(ui -> lineEditInput, SIGNAL(textChanged(QString)), mpMediaObject, SLOT(clear()));
}

void MyPlayer::play(const QString& fileName)
{
    mFileName = fileName;
	valueSpinBox = uiForm ->spinBox ->value();
	if (!uiForm ->radioButtonRepeat ->isChecked())
		countLoopPlay = 1;
	else
	{
		//countLoopPlay = uiForm ->spinBox ->value();
	    countLoopPlay = valueSpinBox;
		//valueSpinBox = uiForm ->spinBox ->value();
	}
	
	
	mpMediaObject ->clear(); // ��������� ��������������� � ������� �������
	startPlay();
}

void MyPlayer::startPlay()
{
	static size_t n = 0;
	if (countLoopPlay)
	{
		if (n)
		{
			// ����� ����� ��������
			QEventLoop eventLoop;
			QTimer timer;
			timer.setSingleShot(true);
			timer.setInterval(2000);
	
			connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
			timer.start();
			eventLoop.exec();
		}
		mpMediaObject ->setCurrentSource(Phonon::MediaSource(mFileName));
		
		++n;
		--countLoopPlay;
		if (uiForm ->radioButtonRepeat ->isChecked())
			uiForm ->spinBox ->setValue(countLoopPlay);
		
		mpMediaObject ->play();
	}
	else
	{
		emit clearMediaObject(); // ��������� ��������������� � ������� �������
		n = 0;
		uiForm ->spinBox ->setValue(valueSpinBox);
	}
}
