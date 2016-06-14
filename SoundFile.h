#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <QWidget>
#include <QFile>
#include <QString>
#include <QDate>


#include <QDebug>

class Note /*: public QWidget*/
{
	//friend class SoundFile;
	
	QString mCountry;
	QString mSex;
	QString mSource;
	QString mDate;

public:
	Note(QString country = QWidget::tr("Великобритания"), QString sex = QWidget::tr("Мужской"), QString source = QWidget::tr("Forvo")) : mCountry(country), mSex(sex),
		                                                                mSource(source), mDate(QDate::currentDate().toString("dd.MM.yyyy"))
	{}
	const QString stringForNote() const
	{
		return QString(QWidget::tr("Страна:    "  ) + mCountry + '\n' +
			           QWidget::tr("Пол:         ") + mSex     + '\n' +
					   QWidget::tr("Источник: "   ) + mSource  + '\n' +
					   QWidget::tr("Дата:       " ) + mDate            );
	}
};

class SoundFile : public QFile
{
	Q_OBJECT

	QString mName;
	QString mExt;
	QString mWord;
	Note    mNote;
	
public:
	SoundFile(const QString& absPath, const Note& note);
	~SoundFile();

	const QString& getExt    () const { return mExt;     }
	const QString& getName   () const { return mName;    }
	const QString& getWord   () const { return mWord;    }

	const Note& getNote      () const { return mNote;    }
	
	void setExt    (const QString ext    ) { mExt     = ext;     };
	void setName   (const QString name   ) { mName    = name;    };
	void setWord   (const QString word   ) { mWord    = word;    };

	void setNote   (const Note& note      ) { mNote    = note;    };

	void show()
	{
		/*qDebug() << fileName() << endl << mName << endl << mExt << endl << mWord << endl
			     << mNote.mCountry << endl << mNote.mSex << endl 
				 << mNote.mSource << endl << mNote.mDate << endl;*/
	}

	static QString extractName(const QString& absPath);
	static QString extractExt (const QString& absPath);
	static QString extractWord(      QString  name   );
	static QString extractWordGroup(QString fileName );
	static bool startsWithOneWord(const QString& fileName, const QString& word);
	static bool WordGroup(const QString& word);
	static bool startsWithWordGroup(const QString& fileName, const QString& word);
	static bool startsWithWord(const QString& fileName, const QString& word);
};

#endif // SOUNDFILE_H
