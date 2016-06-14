#ifndef MY_SLIDER
#define MY_SLIDER
#include <phonon>

class MySlider: public Phonon::VolumeSlider
{

protected:

	void mousePressEvent(QMouseEvent* e)
	{
		qDebug() << "PressEvent MySlider  " << e ->x() << "  " << e ->y();
		
		
	}


};
#endif