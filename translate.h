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
		qint32             mCurrentIndex; // ������ �������� � ������� �������
		
		QClipboard*        mpClipboard;

		QStringList mStrListFiles; // ������ ������ �� �����
		//QStringList mStrListCacheWord; // ������ ���� ����, ��� ������� ���� ������� �������� �����
		QHash <QString, QString> mCacheFiles; // ��������� ��� ��������� �������� ������
		quint32 mNumber; // ���������� ������ � �����
		
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
		inline quint32 uniqueFileName(const QStringList& strListFiles, const QString& word); // ��������� ������ � ����� ������ � ������
		// ������ ����������� �������� � ������
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
		// ������ ������
		template <typename T>
		void reverseList(QList <T>& list)	// ������ ������
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
		
		/*inline void addCache();
		inline bool loadCache();
		inline bool saveCache();*/

		inline void pressKeyNoModifier(const int codeKey);
		inline void pressKeyYesModifier(const Qt::KeyboardModifiers&, const int codeKey);
	    
		~MyWidget()
		{
			delete uiForm;
		}

	public slots:
        void pressedEnter();
        void choiceItemFromList(QListWidgetItem* item);
		void choiceItemFromCacheWord(QListWidgetItem* item); // ����� ����� �� ����
		void fromClipboardToLineEdit();

		void choiceFileInExplorer();
		void accept(QString& fileName, QString& fileNewName);
        void reject();
		void acceptMessBox();

		void setNewCurrentIndex(quint32 newInd);

		void addCache();
		bool loadCache();
		bool saveCache();
		
    signals:
		void changedCurrentIndex(quint32 newInd);
		void error();
		
protected:
	void keyPressEvent(QKeyEvent* pe);
	bool event(QEvent* pe); // ������������ �� ���������� ���� ��� �������� ����
	void closeEvent(QCloseEvent* pcle)
	{
		if (saveCache())
		{
			qDebug() << "closeEvent is accept";
			pcle ->accept();
		} 
		else 
		{
			qDebug() << "closeEvent is ignore";
			pcle ->ignore();
		}
	}

//protected:
//	void mousePressEvent(QMouseEvent* pme)
//	{
//		if (pme ->button() == Qt::RightButton)
//			qDebug() << "Qt::RightButton";
//	}
};

//void extractFiles(const QStringList& listFiles, MyWidget*);

#endif // TRANSLATE_H
