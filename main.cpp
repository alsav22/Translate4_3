#include <QtGui>
#include "translate.h"
#include "Data.h"

#include <Windows.h>
#include <cstdlib>

#include <qhash.h>

#ifdef DEBUG
#include <vld.h>
#endif

char* fileDict = "Mueller7accentGPL.1251.txt";
QHash <QString, QString> dict;
void input()
{
	QFile file(fileDict);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Error!";
		system("pause");
		return;
	}
	
	QTextStream in(&file);
	QString en;
	QString ru;
	QString temp;
	for (int i = 0; i < 279; ++i)
	{
		in.readLine();
	}
	for (int i = 0; i < 3; ++i)
	
	//while (true)
	{
		in >> en;
		if (!en.isEmpty() && en[0] != '_' && en [0] != '-' && en[0] != '\'')
			ru = in.readLine();
		if (!in.atEnd())
		{
			dict.insert(en, ru);
			qDebug() << en << "  " << dict.value(en);
		}
	}
	
	file.close();

	//qDebug() << dict.value("back");
	
}

int main(int argc, char **argv)
{	
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    QApplication app(argc, argv);
	
	QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));
	

	if (!start())
	{
		app.quit();
		return 0;
	}
	
	MyWidget myWidget;
	
	myWidget.setWindowIcon(QIcon("icon.png"));
	
	QObject::connect(&myWidget, SIGNAL(destroyed()), &app, SLOT(quit()));
	
	myWidget.move(700, 400);

    myWidget.show();

	input();

	return app.exec();
}



