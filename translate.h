#ifndef TRANSLATE_H
#define TRANSLATE_H
#include <QObject>
#include <QtGui>
#include <QKeyEvent>
#include "MyPlayer.h"
#include "DictProgram.h"
#include "GVariables.h"
//#include "Storage.h"
#include "Data.h"
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
		QString            mCurrentTranslate;
		qint32             mCurrentIndex; // ������ �������� � ������� �������
		
		QClipboard*        mpClipboard;

		QStringList mStrListFiles; // ������ ������ �� �����
		//QStringList mStrListCacheWord; // ������ ���� ����, ��� ������� ���� ������� �������� �����
		QHash <QString, QString> mCacheFiles; // ��������� ��� ��������� �������� ������
		                                      // (������ ����� � ���� � �������� ������, ����: G:/Sound/StarDict_En-En/BrE/a/about.wav) 
		quint32 mNumber; // ���������� ������ � �����

		DictProgram dictProgram; // ������ ��� ���������
		QStringList mListLastWords; // ������ ��������� ��������� ����
		
public:	
		MyWidget(QWidget *parent = 0);
	    bool operator()(const QStringList& );
		
        inline int     checkWord(const QString& word);
		inline bool    findFiles(QString& word);
		inline bool    extractFiles(const QStringList& listFiles);
		inline bool    containsFileName(const QStringList& strListFiles, const QString& word);
		inline bool    containsWordGroup(const QStringList& strListFiles, const QString& word);
		inline void    showFilesFound();
		inline void    saveFile(const QString& fileName, const QString& fileNewName);
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
		
		inline bool containsInCache(const QString& word); // ���� ����� ���� � ����
		inline QListWidgetItem* getItemFromCache(const QString& word); 
		
		inline void pressKeyNoModifier(const int codeKey);
		inline void pressKeyYesModifier(const Qt::KeyboardModifiers&, const int codeKey);

		inline void setCurrentDataForItemCache(QListWidgetItem* item);

		//inline void outputTr(QString& translation); // ����� ��������
		inline void findTr(const QString& word); // ����� � ����� ��������
	    
		~MyWidget()
		{
			delete uiForm;
		}

	public slots:
        void pressedEnter();
        void choiceItemFromList(QListWidgetItem* item);
		void choiceItemFromCacheWord(QListWidgetItem* item); // ����� ����� �� ����
		void previewToCache(const QString&);
		void fromClipboardToLineEdit();

		void choiceFileInExplorer();
		void replaceFile(QString& fileName, QString& fileNewName);
        void appendFile(const QString& fileName, const QString& fileNewName);
		void acceptMessBox();

		void setNewCurrentIndex(quint32 newInd);

		void addCache();
		bool loadCache();
		bool saveCache();
		
    signals:
		void changedCurrentIndex(QClipboard::Mode);
		void error();
		
protected:
	void keyPressEvent(QKeyEvent* pe);
	bool event(QEvent* pe); // ������������ �� ���������� ���� ��� �������� ����
	
	void closeEvent(QCloseEvent* pcle)
	{
		QSplashScreen splash(QPixmap("6.png"));
	    splash.show();
		
		if (GlobalVariables::getGlobalVariables().changeHash)
		{
			if(!Data::saveHash()) // ���������� ����� ����
			{
				QMessageBox error;
				error.setIcon(QMessageBox::Critical);
				error.setWindowTitle(QWidget::tr("������!"));
				error.setText(QWidget::tr("������ ��� ���������� ����� Hesh.dat!"));
				error.exec();
			}
		}
		if (saveCache())
		{
			qDebug() << "closeEvent is accept";
			pcle ->accept();
		} 
		else 
		{
			qDebug() << "closeEvent is ignore";
			QMessageBox error;
			error.setIcon(QMessageBox::Critical);
			error.setWindowTitle(QWidget::tr("������!"));
			error.setText(QWidget::tr("������ ��� ���������� ����� Cache.dat!"));
			error.exec();
			
			pcle ->ignore();
		}
		qDebug() << "exit!!!";
		splash.close();
	}

protected:
	void mousePressEvent(QMouseEvent* pme)
	{
		if (pme ->button() == Qt::LeftButton)
		{
			qDebug() << "Qt::LeftButton";
			QWidget* p = this ->focusWidget();
			if (p)
				qDebug() << p ->objectName();
		}
	}
};

//void extractFiles(const QStringList& listFiles, MyWidget*);

#endif // TRANSLATE_H
