#ifndef MY_PLAYER
#define MY_PLAYER
#include <QString>
#include <QWidget>
#include <phonon>

namespace Ui 
{
    class Form;
}

class MyPlayer : public QWidget
{
	Q_OBJECT
	
	Phonon::AudioOutput*  mpAudioOutput;
    Phonon::MediaObject*  mpMediaObject;
    Ui::Form* uiForm;
	QString mFileName;
	int countLoopPlay;

public: 
     
    void createMyPlayer(Ui::Form*);

	Phonon::MediaObject* getmpMediaObject()
	{ return mpMediaObject; }

	const QString& getFileName() const
	{ return mFileName; }
    
	public slots:
        void play(const QString& fileName);
		
	private slots:	
		void startPlay();

	signals:
		void clearMediaObject();
};

#endif