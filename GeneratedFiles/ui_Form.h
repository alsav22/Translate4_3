/********************************************************************************
** Form generated from reading UI file 'Form.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>
#include <phonon/volumeslider.h>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QLineEdit *lineEditInput;
    QLabel *labelInput;
    QLabel *labelOutput;
    Phonon::VolumeSlider *volumeSlider;
    QCommandLinkButton *commandLinkButton;
    QRadioButton *radioButtonRepeat;
    QSpinBox *spinBox;
    QListWidget *listWidgetFiles;
    QLabel *label_3;
    QCheckBox *checkBox;
    QListWidget *cacheWord;
    QLabel *label;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(939, 489);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        Form->setFont(font);
        Form->setCursor(QCursor(Qt::ArrowCursor));
        Form->setFocusPolicy(Qt::NoFocus);
        lineEditInput = new QLineEdit(Form);
        lineEditInput->setObjectName(QString::fromUtf8("lineEditInput"));
        lineEditInput->setGeometry(QRect(30, 70, 421, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(12);
        lineEditInput->setFont(font1);
        labelInput = new QLabel(Form);
        labelInput->setObjectName(QString::fromUtf8("labelInput"));
        labelInput->setGeometry(QRect(40, 30, 251, 20));
        labelInput->setFont(font);
        labelOutput = new QLabel(Form);
        labelOutput->setObjectName(QString::fromUtf8("labelOutput"));
        labelOutput->setGeometry(QRect(480, 30, 421, 81));
        labelOutput->setFont(font1);
        labelOutput->setLayoutDirection(Qt::LeftToRight);
        labelOutput->setFrameShape(QFrame::Box);
        labelOutput->setLineWidth(1);
        labelOutput->setAlignment(Qt::AlignCenter);
        volumeSlider = new Phonon::VolumeSlider(Form);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
        volumeSlider->setGeometry(QRect(30, 170, 211, 30));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(9);
        volumeSlider->setFont(font2);
        volumeSlider->setCursor(QCursor(Qt::PointingHandCursor));
        volumeSlider->setMaximumVolume(1);
        commandLinkButton = new QCommandLinkButton(Form);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));
        commandLinkButton->setGeometry(QRect(480, 165, 191, 42));
        commandLinkButton->setFont(font);
        commandLinkButton->setCursor(QCursor(Qt::PointingHandCursor));
        commandLinkButton->setIconSize(QSize(40, 40));
        radioButtonRepeat = new QRadioButton(Form);
        radioButtonRepeat->setObjectName(QString::fromUtf8("radioButtonRepeat"));
        radioButtonRepeat->setGeometry(QRect(40, 130, 291, 23));
        radioButtonRepeat->setFont(font);
        radioButtonRepeat->setCursor(QCursor(Qt::PointingHandCursor));
        spinBox = new QSpinBox(Form);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(385, 125, 66, 31));
        spinBox->setFont(font);
        spinBox->setCursor(QCursor(Qt::PointingHandCursor));
        spinBox->setValue(3);
        listWidgetFiles = new QListWidget(Form);
        listWidgetFiles->setObjectName(QString::fromUtf8("listWidgetFiles"));
        listWidgetFiles->setGeometry(QRect(30, 260, 421, 151));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial"));
        font3.setPointSize(11);
        listWidgetFiles->setFont(font3);
        listWidgetFiles->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
        label_3 = new QLabel(Form);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 220, 171, 19));
        checkBox = new QCheckBox(Form);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(40, 440, 351, 23));
        cacheWord = new QListWidget(Form);
        cacheWord->setObjectName(QString::fromUtf8("cacheWord"));
        cacheWord->setGeometry(QRect(480, 260, 421, 151));
        cacheWord->setFont(font3);
        cacheWord->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
        cacheWord->setSortingEnabled(true);
        label = new QLabel(Form);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(485, 220, 66, 19));

        retranslateUi(Form);
        QObject::connect(lineEditInput, SIGNAL(textChanged(QString)), labelOutput, SLOT(clear()));
        QObject::connect(radioButtonRepeat, SIGNAL(clicked()), lineEditInput, SLOT(setFocus()));
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), lineEditInput, SLOT(setFocus()));
        QObject::connect(lineEditInput, SIGNAL(returnPressed()), Form, SLOT(pressedEnter()));
        QObject::connect(commandLinkButton, SIGNAL(clicked()), Form, SLOT(choiceFileInExplorer()));
        QObject::connect(listWidgetFiles, SIGNAL(itemActivated(QListWidgetItem*)), Form, SLOT(choiceItemFromList(QListWidgetItem*)));
        QObject::connect(listWidgetFiles, SIGNAL(itemClicked(QListWidgetItem*)), Form, SLOT(choiceItemFromList(QListWidgetItem*)));
        QObject::connect(checkBox, SIGNAL(stateChanged(int)), lineEditInput, SLOT(setFocus()));
        QObject::connect(cacheWord, SIGNAL(itemClicked(QListWidgetItem*)), Form, SLOT(choiceItemFromCacheWord(QListWidgetItem*)));
        QObject::connect(cacheWord, SIGNAL(itemActivated(QListWidgetItem*)), Form, SLOT(choiceItemFromCacheWord(QListWidgetItem*)));

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "\320\237\321\200\320\276\320\270\320\267\320\275\320\276\321\210\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        labelInput->setText(QApplication::translate("Form", "\320\220\320\275\320\263\320\273\320\270\320\271\321\201\320\272\320\276\320\265 \321\201\320\273\320\276\320\262\320\276  (Enter) :", 0, QApplication::UnicodeUTF8));
        labelOutput->setText(QString());
        commandLinkButton->setText(QApplication::translate("Form", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \321\204\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
        radioButtonRepeat->setText(QApplication::translate("Form", " \320\237\320\276\320\262\321\202\320\276\321\200\321\217\321\202\321\214 (Shift + Alt + \342\206\222)", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Form", "\320\227\320\262\321\203\320\272\320\276\320\262\321\213\320\265 \321\204\320\260\320\271\320\273\321\213:", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("Form", "\320\222\321\213\320\262\320\265\321\201\321\202\320\270 \321\201\320\273\320\276\320\262\320\276\321\201\320\276\321\207\320\265\321\202\320\260\320\275\320\270\321\217 (Alt + \342\206\222)", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form", "\320\232\320\265\321\210:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
