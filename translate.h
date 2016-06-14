#ifndef TRANSLATE_H
#define TRANSLATE_H
#include <QObject>
#include <QtGui>
#include <QKeyEvent>
#include "MyPlayer.h"
//#include "Storage.h"
//#include "Data.h"
#include "ui_Form.h"

#include <Windows.h>

//namespace Ui 
//{
//    class Form;
//}

//class MyThread;
class SoundFile;

void checkOpenFileName();
void checkPathSound();

class MyWidget : public QWidget
{
	Q_OBJECT
public:       
        Ui::Form* uiForm;
		
private:		
	    QMessageBox* mpMessageBox;
		//MyThread* thr;
		//Storage      mStorage;
		MyPlayer     mMyPlayer;
        QDir         mDirSound;
		QDir         mCurrentDir;
		
		QList <SoundFile*> mCurrentListPsoundFile;
		SoundFile*         mpCurrentSoundFile;
		
		QStringList        mCurrentListAbsFilePath;
		QString            mCurrentAbsFilePath;
		QStringList        mCurrentListFileName;
		QString            mCurrentFileName;
		QString            mCurrentWord;
		qint32             mCurrentIndex; // индекс элемента в текущих списках
		
		QClipboard*        mpClipboard;

		QStringList mStrListFiles; // список файлов из папки
		//QStringList mStrListCacheWord; // список кеша слов, для которых были найдены звуковые файлы
		QHash <QString, QString> mCacheFiles; // контейнер для найденных звуковых файлов
		quint32 mNumber; // количество файлов в папке
		
public:	
		MyWidget(QWidget *parent = 0);
	    bool operator()(const QStringList& );
		
        inline int    checkWord(const QString& word);
		inline bool   findFiles(QString& word);
		inline bool   extractFiles(const QStringList& listFiles);
		inline bool   containsFileName(const QStringList& strListFiles, const QString& word);
		inline bool   containsWordGroup(const QStringList& strListFiles, const QString& word);
		inline void   showFilesFound();
		inline void   saveFile(QString& fileName, QString& fileNewName);
		inline qint32  getIndexString(const QStringList& strList, const QString& word, const QString mod);
		inline quint32 uniqueFileName(const QStringList& strListFiles, const QString& word); // количство файлов с таким словом в списке
		// индекс наименьшего элемента в списке
		template <typename T> 
		quint32 getIndSmallestElement(QList <T>& list) 
		{
			T e = *list.begin();
			quint32 ind = 0;
			quint32 n = list.size();
			for (quint32 i = 1; i < n; ++i)
			{
				if (e.size() > list[i].size()) 
				{
					e = list[i];
					ind = i;
				}
			}
			return ind;
		}
		// реверс списка
		template <typename T>
		void reverseList(QList <T>& list)	// реверс списка
		{
			QList <T> reverse;
			reverse.reserve(list.size());
			foreach(T e, list)
				reverse.prepend(e);
			reverse.swap(list);
		}
		
		const qint32 getCurrentIndex() const     { return mCurrentIndex; }
		      void   setCurrentIndex(qint32 ind) { mCurrentIndex = ind;  }
	    inline QStringList getStringListName(const QList <SoundFile*>& listPsoundFile);
	    inline void play(QString& AbsFilePath);
		inline void changeStateButton(QAbstractButton* pButton);
		inline void addCache();
		inline bool loadCache();
		inline bool saveCache();
	    
		~MyWidget()
		{
			delete uiForm;
		}

	public slots:
        void pressedEnter();
        void choiceItemFromList(QListWidgetItem* item);
		void choiceItemFromCacheWord(QListWidgetItem* item); // выбор слова из кеша
		void fromClipboardToLineEdit();

		void choiceFileInExplorer();
		void accept(QString& fileName, QString& fileNewName);
        void reject();
		void acceptMessBox();

		void setNewCurrentIndex(quint32 newInd);
		
    signals:
		void changedCurrentIndex(quint32 newInd);
		void error();
		
protected:
	void keyPressEvent(QKeyEvent*   pe);
	bool event(QEvent* pe); // переключение на английский ввод при активном окне
	void closeEvent(QCloseEvent* event)
	{
		if (saveCache())
		{
			qDebug() << "closeEvent is accept";
			event->accept();
		} 
		else 
		{
			qDebug() << "closeEvent is ignore";
			event->ignore();
		}
	}
};

//void extractFiles(const QStringList& listFiles, MyWidget*);

#endif // TRANSLATE_H
