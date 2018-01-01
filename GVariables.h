#ifndef G_VARIABLES_H
#define G_VARIABLES_H
#include <QtGui>
#include <QtCore>

class GlobalVariables
{
public:
	const QChar   delim;       // ����������� ����� ������ � ��������� ������ ����� �����.
    const QChar   slash;       // ���� � �����
	const QString FILE_NAME;   // ��� �����, � ������� ������� ���� � ����� �� ��������� �������
	      QString PATH_SOUND;  // ���� � ����� �� ��������� �������
	      //bool    openFileName;// �������� �������� ����� FILE_NAME
	const QString MP3;         // ���������� �������� ������ (������� .)
    const QString WAV;
    const QString FILTER;
	
	QHash <QChar, QStringList> ListsFilesFromDirs; // ��������� ��� ������� ������ �� �����
	                                            // ������ ����� ��� �����, � ������ �������� ������ � ������
	bool changeHash; // ���� ��������� ����������� ���������� (��� ����������/�������� ������)
	                // ���� ��������� (true), �� ���������� ����� � �����������
	QMutex mutex; // ������������� ������� ��� ���������� ���������� ��� ������� ������ �� �����

    QStringList LIST_EXT; // ���������� ���������� �����������

	static GlobalVariables& getGlobalVariables() 
	{
		static GlobalVariables GlobalVar;
		return GlobalVar;
	}

private:
	GlobalVariables() :  delim ('_'), slash('/'), changeHash(false),
		                FILE_NAME("Path_Sound.txt"),
		                PATH_SOUND(""),
						/*openFileName(false),*/
						MP3(".mp3"), WAV(".wav"),
						FILTER("*" + MP3 + " " + "*" + WAV)
	{ 
		
			LIST_EXT.append("mp3");
			LIST_EXT.append("wav");
	}

	GlobalVariables(const GlobalVariables& );
	GlobalVariables& operator=(const GlobalVariables& );
};
#endif