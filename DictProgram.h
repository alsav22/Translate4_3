#ifndef DICTPROGRAM_H
#define DICTPROGRAM_H

#include <QtGui>
#include "dictionary.h"

class DictProgram //: public QWidget
{
	//Q_OBJECT

public:
	DictProgram(/*QWidget *parent = 0, Qt::WFlags flags = 0*/) //: QWidget(parent, flags)
	{
		//ui.setupUi(this);
		//ui.checkBox_0 ->setChecked(true); // общ.
		
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
		
		//mvectorPointsToCheckBox.push_back(ui.checkBox_0); // общ.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_1); // разг.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_2); // комп.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_3); // политех.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_4); // биол.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_5); // медиц.
		
		for (int i = 0; i < mvectorNamesDicts.size(); ++i)
		{
			Dictionary* pdict = new Dictionary(mvectorNamesDicts[i].first, mvectorNamesDicts[i].second);
			mvectorPointsToDicts.push_back(pdict);
		}
	}

	void translate()
	{
		QString word((ui.lineEdit ->text()).trimmed());
		QString translation;
		for (int i = 0; i < mvectorPointsToDicts.size(); ++i)
		{
			if (mvectorPointsToCheckBox[i] ->checkState() == Qt::Checked) // если словарь выбран
			{
					QString temp = mvectorPointsToDicts[i] ->getTr(word); // получение перевода от этого словаря
					if (!temp.isEmpty())
					{
					translation.append(temp); // суммирование переводов от разных словарей
					}
			}
		}
		outputTr(translation);
	}

	// вывод перевода
	void outputTr(QString& translation)
	{
		ui.textEdit ->clear();
		if (!translation.isEmpty())
			ui.textEdit ->setText(translation);
		else
			ui.textEdit ->setText(QWidget::tr("Слово не найдено!"));
	}
	
	~DictProgram()
	{
		for (int i = 0; i < mvectorPointsToDicts.size(); ++i)
		{
			delete mvectorPointsToDicts[i];
		}
	}

private:
	QVector <QPair <QString, QString> > mvectorNamesDicts; // контейнер с именами словарей (папок)
	QVector <Dictionary*> mvectorPointsToDicts; // контейнер с указателями на словари
	QVector <QCheckBox*> mvectorPointsToCheckBox; // контейнер с указателями на чек-боксы
};
#endif