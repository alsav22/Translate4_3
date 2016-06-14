#ifndef MY_DIALOG
#define MY_DIALOG
#include <QDialog>
#include <QWidget>

class MyDialog : public QDialog
{
    Q_OBJECT

public:
    MyDialog(QDialog* parent = 0);

public slots:
    void accept();
    void reject();
};
#endif

