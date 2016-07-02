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
		//ui.checkBox_0 ->setChecked(true); // ���.
		
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
		
		//mvectorPointsToCheckBox.push_back(ui.checkBox_0); // ���.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_1); // ����.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_2); // ����.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_3); // �������.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_4); // ����.
		//mvectorPointsToCheckBox.push_back(ui.checkBox_5); // �����.
		
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
			if (mvectorPointsToCheckBox[i] ->checkState() == Qt::Checked) // ���� ������� ������
			{
					QString temp = mvectorPointsToDicts[i] ->getTr(word); // ��������� �������� �� ����� �������
					if (!temp.isEmpty())
					{
					translation.append(temp); // ������������ ��������� �� ������ ��������
					}
			}
		}
		outputTr(translation);
	}

	// ����� ��������
	void outputTr(QString& translation)
	{
		ui.textEdit ->clear();
		if (!translation.isEmpty())
			ui.textEdit ->setText(translation);
		else
			ui.textEdit ->setText(QWidget::tr("����� �� �������!"));
	}
	
	~DictProgram()
	{
		for (int i = 0; i < mvectorPointsToDicts.size(); ++i)
		{
			delete mvectorPointsToDicts[i];
		}
	}

private:
	QVector <QPair <QString, QString> > mvectorNamesDicts; // ��������� � ������� �������� (�����)
	QVector <Dictionary*> mvectorPointsToDicts; // ��������� � ����������� �� �������
	QVector <QCheckBox*> mvectorPointsToCheckBox; // ��������� � ����������� �� ���-�����
};
#endif