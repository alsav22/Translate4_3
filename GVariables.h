#ifndef G_VARIABLES_H
#define G_VARIABLES_H
#include <QtGui>
#include <QtCore>

class GlobalVariables
{
public:
	const QChar   delim;       // разделитель между словом и остальной частью имени файла.
    const QChar   slash;       // слеш в путях
	const QString FILE_NAME;   // имя файла, в котором записан путь к папке со звуковыми файлами
	      QString PATH_SOUND;  // путь к папке со звуковыми файлами
	      //bool    openFileName;// проверка открытия файла FILE_NAME
	const QString MP3;         // расширения звуковых файлов (включая .)
    const QString WAV;
    const QString FILTER;
	
	QHash <QChar, QStringList> ListsFilesFromDirs; // контейнер для списков файлов из папок
	                                            // хранит буквы имён папок, и списки звуковых файлов в папках
	bool changeHash; // флаг изменения содержимого контейнера (при добавлении/удалении файлов)
	                // если изменился (true), то перезапись файла с контейнером
	QMutex mutex; // синхронизация доступа при заполнении контейнера для списков файлов из папок

    QStringList LIST_EXT; // допустимые расширения аудиофайлов

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