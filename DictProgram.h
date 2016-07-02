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
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-ER-LingvoUniversal-2.4.2"), QString(QWidget::tr("����� �������"))));
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-lingvo-ER-Informal-2.4.2"), QString(QWidget::tr("����������� �������"))));
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-lingvo-ER-Computer-2.4.2"), QString(QWidget::tr("������������"))));
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-lingvo-ER-Polytechnical-2.4.2"), QString(QWidget::tr("���������������"))));
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-lingvo-ER-Biology-2.4.2"), QString(QWidget::tr("�������������"))));
		mvectorNamesDicts.push_back(qMakePair(QString("stardict-lingvo-ER-Medical-2.4.2"), QString(QWidget::tr("�����������"))));
		
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
				pCheckBox ->setChecked(true); // ���.
			mvectorPointsToCheckBox.push_back(pCheckBox);
		}
		
		//mvectorPointsToCheckBox.push_back(ui.checkBox_0); // ���.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_1); // ����.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_2); // ����.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_3); // �������.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_4); // ����.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_5); // �����.
	}

	void translate(const QString& word, Ui::Form* ui)
	{
		QString translation;
		for (int i = 0; i < mvectorPointsToDicts.size(); ++i)
		{
			if (mvectorPointsToCheckBox[i] ->checkState() == Qt::Checked) // ���� ������� ������
			{
				QString temp = mvectorPointsToDicts[i] ->getTr(word); // ��������� �������� �� ����� �������
				if (!temp.isEmpty())
				{
					translation.append(temp); // ������������ ��������� �� ������ ��������
				}
			}
		}
		outputTr(translation, ui);
	}

	// ����� ��������
	void outputTr(QString& translation, Ui::Form* ui)
	{
		ui ->labelOutput ->clear();
		if (!translation.isEmpty())
			ui ->labelOutput ->setText(translation);
		else
			ui ->labelOutput ->setText(QWidget::tr("����� �� �������!"));
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
	QVector <QPair <QString, QString> > mvectorNamesDicts; // ��������� � ������� �������� (�����)
	QVector <Dictionary*> mvectorPointsToDicts; // ��������� � ����������� �� �������
	QVector <QCheckBox*> mvectorPointsToCheckBox; // ��������� � ����������� �� ���-�����
};
#endif