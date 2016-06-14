#include <QtGui>
#include "translate.h"
#include "Data.h"

#include <Windows.h>
#include <cstdlib>


#ifdef DEBUG
#include <vld.h>
#endif


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
	
	
	
	
	return app.exec();
}



