#include <QtGui>
#include "translate.h"
#include "Data.h"

#include <Windows.h>
#include <cstdlib>

#include <qhash.h>

#ifdef DEBUG
#include <vld.h>
#endif


void show(const char* key, QHash <QString, QString>* dict)
{
	QString str = dict ->value(key);
	str = str.trimmed();
	
	qDebug() << key << ": ";
	int ind_beg = 0;
	int ind_end = 0;
	while (true)
	{
		ind_end = str.indexOf(';', ind_beg);
		if (ind_end != -1)
		{
			qDebug() << "      " << str.mid(ind_beg, ind_end - ind_beg) << ";";
		}
		else
		{
			qDebug() << "      " << str.mid(ind_beg, str.size() - ind_beg);
			break;
		}
		ind_beg = ind_end + 1;
	}
}

void create()
{
	char* fileDict = "Mueller7accentGPL.1251.txt";
	QFile file(fileDict);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Error1!";
		system("pause");
		return;
	}
	
	QHash <QString, QString>* dict = new QHash <QString, QString>;
	
	QTextStream in(&file);
	QString en;
	QString ru;
	QString temp;
	/*for (int i = 0; i < 279; ++i)
	{
		in.readLine();
	}*/
	
	while (true)
	{
		in >> en;
		if (!en.isEmpty() && en[0] != '_' && en [0] != '-' && en[0] != '\'')
			ru = in.readLine();
		if (!in.atEnd())
		{
			dict ->insert(en, ru);
		}
		else
			break;
	}
	
	file.close();

	QFile file_out("Hash_Dict");
	if (!file_out.open(QIODevice::WriteOnly))
	{
		qDebug() << "Error2!";
		system("pause");
		return;
	}
	
	QDataStream out(&file_out);
	out << *dict;
	file_out.close();
	delete dict;
}

void input()
{
	QFile file("Hash_Dict");
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Error2!";
		system("pause");
		return;
	}
	
	QHash <QString, QString>* dict = new QHash <QString, QString>;
	QDataStream in(&file);
	in >> *dict;
	file.close();
	
	show("zero", dict);
	
	delete dict;
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
	QDir dir(QDir::current());
	
	if (!dir.exists("Hash_Dict"))
		create();
	
	input();

	return app.exec();
}



