#include "MyDialog.h"
#include <QDebug>

MyDialog::MyDialog(QDialog* parent) : QDialog(parent)
{
    setWindowModality(Qt::WindowModal);
    //setObjectName(QWidget::tr("Диалог"));
}


void MyDialog::accept()
{
    qDebug() << "MyDialog::accept()";
}

void MyDialog::reject()
{
    qDebug() << "MyDialog::reject()";
}

