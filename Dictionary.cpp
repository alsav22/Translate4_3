#include <QtGui>
#include "Dictionary.h"

Dictionary::Dictionary(const QString fileName, const QString hashName) : mfile(fileName), mhash(hashName)
{
	QDir dir(QDir::current());
	
	if (!dir.exists(mhash))
		create();
	
	load();
}

bool Dictionary::create()
{
	QFile file(mfile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Error1!";
		system("pause");
		return false;
	}
	
	QTextStream in(&file);
	QString en;
	QString ru;
	QString temp;
		
	while (true)
	{
		in >> en;
		if (!en.isEmpty() && en[0] != '_' && en [0] != '-' && en[0] != '\'')
			ru = in.readLine();
		if (!in.atEnd())
		{
			insert(en, ru);
		}
		else
			break;
	}
	
	file.close();

	QFile file_out(mhash);
	if (!file_out.open(QIODevice::WriteOnly))
	{
		qDebug() << "Error2!";
		system("pause");
		return false;
	}
	
	QDataStream out(&file_out);
	out << *this;
	file_out.close();
	
	return true;
}


//bool Dictionary::create()
//{
//	QFile file(mfile);
//	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//	{
//		qDebug() << "Error1!";
//		system("pause");
//		return false;
//	}
//	
//	QTextStream in(&file);
//	QString en;
//	QString ru;
//	QString temp;
//		
//	while (true)
//	{
//		in >> en;
//		if (!en.isEmpty() && en[0] != '_' && en [0] != '-' && en[0] != '\'')
//			ru = in.readLine();
//		if (!in.atEnd())
//		{
//			insert(en, ru);
//		}
//		else
//			break;
//	}
//	
//	file.close();
//
//	QFile file_out(mhash);
//	if (!file_out.open(QIODevice::WriteOnly))
//	{
//		qDebug() << "Error2!";
//		system("pause");
//		return false;
//	}
//	
//	QDataStream out(&file_out);
//	out << *this;
//	file_out.close();
//	
//	return true;
//}

bool Dictionary::load()
{
	QFile file(mhash);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Error2!";
		system("pause");
		return false;
	}
	
	QDataStream in(&file);
	in >> *this;
	file.close();
	
	show("zero");
	return true;
}

bool Dictionary::save()
{
	return true;
}

QString Dictionary::show(const QString& key)
{
	QString str = value(key);
	str = str.trimmed();
	QString output;
	output.append(str + ": " + "\r\n");
	//qDebug() << key << ": ";
	int ind_beg = 0;
	int ind_end = 0;
	while (true)
	{
		ind_end = str.indexOf(';', ind_beg);
		if (ind_end != -1)
		{
			//qDebug() << "      " << str.mid(ind_beg, ind_end - ind_beg) << ";";
			output.append(str.mid(ind_beg, ind_end - ind_beg) + ";\r\n");
		}
		else
		{
			//qDebug() << "      " << str.mid(ind_beg, str.size() - ind_beg);
			output.append(str.mid(ind_beg, str.size() - ind_beg) + "\r\n");
			break;
		}
		ind_beg = ind_end + 1;
	}
	return output;
}

