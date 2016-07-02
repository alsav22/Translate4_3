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

	// �� QString � HTML-����� (������� ������ CSS)
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
	// �������������� ��������
	void formattingTr(QString& str, QString name)
	{
		str.replace(QRegExp("\n<tr>"), "<tr>"); // ������������ ��� �������� �� ��������� ������
		str.replace("<tr>", " <t>[");  // ��� <tr> �������� �� <t>, �����, ��� �������� � html,
		str.replace("</tr>", "]</t>"); // ���� ����� ����������; ������������ � [], 
		str.replace("\n", "<br />"); // 0x0A �������� �� ��� ����� ������
		str.remove(QRegExp("<rref>.+</rref>")); // ������ �� ������� ���������
		//str.replace("<c>", "<font color=\"red\">"); ��� ����� ������ ���� 
		// ��� ��������-������ �������� �� ��� html-�����������
		str.replace("<iref>", "<a>");
		str.replace("</iref>", "</a>");
		str.replace("web-site:", "<br />web-site:"); // "web-site:" � ����� ������
	
		// �������� �� ��������-������ html-�����������
		QRegExp reg("<a>(.+)</a>");
		QString href;
		int pos = 0;
		while ((pos = reg.indexIn(str, pos)) != -1)
		{
			href = reg.cap(1);
			str.replace(pos + 2, href.size() + 1, " href=\"" + href + "\">" + href);
			pos += reg.matchedLength();
		}
		// ���������� ������������ ����� �������
		QString ins = "&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"#ff00ff\"><b><i>" + name + "</i></b></font><br />";
		str.insert(0, ins);
		str.push_back("<br />");
		//qDebug() << str;
	
		HTMLfromString(str); // � html-����� (� CSS)
	}
	// ��������� ������� (�� ���������� ��������) ����� word � ������ translation
	void translate(const QString& word, QString& translation)
	{
		translation.clear();
		for (int i = 0; i < mvectorPointsToDicts.size(); ++i)
		{
			if (mvectorPointsToCheckBox[i] ->checkState() == Qt::Checked) // ���� ������� ������
			{
				QString temp = mvectorPointsToDicts[i] ->getTr(word); // ��������� �������� �� ����� �������
				if (!temp.isEmpty())
				{
					formattingTr(temp, mvectorPointsToDicts[i] ->getName());
					translation.append(temp); // ������������ ��������� �� ������ ��������
				}
			}
		}
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