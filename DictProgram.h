#ifndef DICTPROGRAM_H
#define DICTPROGRAM_H

#include <QtGui>
#include "dictionary.h"
#include "ui_Form.h"

class DictProgram //: public QWidget
{
	//Q_OBJECT

public:
	DictProgram(/*QWidget *parent = 0, Qt::WFlags flags = 0*/) //: QWidget(parent, flags)
	{
		initialization();
	}

	void initialization()
	{
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-ER-LingvoUniversal-2.4.2"), QString(QWidget::tr("Общей лексики"))));
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-lingvo-ER-Informal-2.4.2"), QString(QWidget::tr("Разговорной лексики"))));
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-lingvo-ER-Computer-2.4.2"), QString(QWidget::tr("Компьютерный"))));
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-lingvo-ER-Polytechnical-2.4.2"), QString(QWidget::tr("Политехнический"))));
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-lingvo-ER-Biology-2.4.2"), QString(QWidget::tr("Биологический"))));
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-lingvo-ER-Medical-2.4.2"), QString(QWidget::tr("Медицинский"))));
		
		initVectorCheckBox();
		
		for (int i = 0; i < mvectorNamesDicts.size(); ++i)
		{
			Dictionary* pdict = new Dictionary(mvectorNamesDicts[i].first, mvectorNamesDicts[i].second);
			mvectorPointsToDicts.push_back(pdict);
		}
	}

	void initVectorCheckBox()
	{
		for (int i = 0; i < mvectorNamesDicts.size(); ++i)
		{
			QCheckBox* pCheckBox = new QCheckBox();
			if (i == 0)
				pCheckBox ->setChecked(true); // общ.
			mvectorPointsToCheckBox.push_back(pCheckBox);
		}
		
		//mvectorPointsToCheckBox.push_back(ui.checkBox_0); // общ.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_1); // разг.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_2); // комп.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_3); // политех.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_4); // биол.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_5); // медиц.
	}

	// из QString в HTML-текст (задание стилей CSS)
	void HTMLfromString(QString& str)
	{
		QString begin("<html><head>");
		QString style("<style type=text/css>"
					  "k {font-weight: bold}"
					  "kref {color: #008080; font-size: 5; font-weight: bold}"
					  "t {font-size: 5; font-family: \"Lucida Sans Unicode\"}"
					  "c {color: blue}");
		QString end("</style></head><body>" + str + "</body></html>");
		str = begin + style + end;
	}
	// форматирование перевода
	void formattingTr(QString& str, QString name)
	{
		str.replace(QRegExp("\n<tr>"), "<tr>"); // транскрипция без переноса на следующую строку
		str.replace("<tr>", " <t>[");  // тег <tr> меняется на <t>, иначе, при переводе в html,
		str.replace("</tr>", "]</t>"); // этот текст вырезается; транскрипцию в [], 
		str.replace("\n", "<br />"); // 0x0A меняется на тег новой строки
		str.remove(QRegExp("<rref>.+</rref>")); // ссылки на ресурсы удаляются
		//str.replace("<c>", "<font color=\"red\">"); так можно задать цвет 
		// тег интернет-ссылки меняется на тег html-гиперссылки
		str.replace("<iref>", "<a>");
		str.replace("</iref>", "</a>");
		str.replace("web-site:", "<br />web-site:"); // "web-site:" с новой строки
	
		// создание из интернет-ссылки html-гиперссылки
		QRegExp reg("<a>(.+)</a>");
		QString href;
		int pos = 0;
		while ((pos = reg.indexIn(str, pos)) != -1)
		{
			href = reg.cap(1);
			str.replace(pos + 2, href.size() + 1, " href=\"" + href + "\">" + href);
			pos += reg.matchedLength();
		}
		// добавление сокращённого имени словаря
		QString ins = "&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"#ff00ff\"><b><i>" + name + "</i></b></font><br />";
		str.insert(0, ins);
		str.push_back("<br />");
		//qDebug() << str;
	
		HTMLfromString(str); // в html-текст (с CSS)
	}
	// получение первода (из нескольких словарей) слова word в строку translation
	void translate(const QString& word, QString& translation)
	{
		translation.clear();
		for (int i = 0; i < mvectorPointsToDicts.size(); ++i)
		{
			if (mvectorPointsToCheckBox[i] ->checkState() == Qt::Checked) // если словарь выбран
			{
				QString temp = mvectorPointsToDicts[i] ->getTr(word); // получение перевода от этого словаря
				if (!temp.isEmpty())
				{
					formattingTr(temp, mvectorPointsToDicts[i] ->getName());
					translation.append(temp); // суммирование переводов от разных словарей
				}
			}
		}
	}

	// вывод перевода
	void outputTr(QString& translation, Ui::Form* ui)
	{
		ui ->labelOutput ->clear();
		if (!translation.isEmpty())
			ui ->labelOutput ->setText(translation);
		else
			ui ->labelOutput ->setText(QWidget::tr("Слово не найдено!"));
	}
	
	~DictProgram()
	{
		for (int i = 0; i < mvectorPointsToDicts.size(); ++i)
		{
			delete mvectorPointsToDicts[i];
		}
		for (int i = 0; i < mvectorPointsToCheckBox.size(); ++i)
		{
			delete mvectorPointsToCheckBox[i];
		}
	}

private:
	QVector <QPair <QString, QString> > mvectorNamesDicts; // контейнер с именами словарей (папок)
	QVector <Dictionary*> mvectorPointsToDicts; // контейнер с указателями на словари
	QVector <QCheckBox*> mvectorPointsToCheckBox; // контейнер с указателями на чек-боксы
};
#endif