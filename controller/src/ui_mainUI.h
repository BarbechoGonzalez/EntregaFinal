/********************************************************************************
** Form generated from reading UI file 'mainUI.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUI_H
#define UI_MAINUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_guiDlg
{
public:
    QPushButton *Start;
    QPushButton *Stop;
    QGraphicsView *Grafico;
    QLCDNumber *LCDhor;
    QLCDNumber *LCDmin;
    QLCDNumber *LCDseg;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QTextEdit *InfoText;
    QPushButton *Reset;
    QLabel *label_4;
    QLabel *label_5;

    void setupUi(QWidget *guiDlg)
    {
        if (guiDlg->objectName().isEmpty())
            guiDlg->setObjectName(QString::fromUtf8("guiDlg"));
        guiDlg->resize(838, 559);
        Start = new QPushButton(guiDlg);
        Start->setObjectName(QString::fromUtf8("Start"));
        Start->setGeometry(QRect(20, 500, 81, 51));
        Stop = new QPushButton(guiDlg);
        Stop->setObjectName(QString::fromUtf8("Stop"));
        Stop->setGeometry(QRect(110, 500, 81, 51));
        Grafico = new QGraphicsView(guiDlg);
        Grafico->setObjectName(QString::fromUtf8("Grafico"));
        Grafico->setGeometry(QRect(290, 10, 541, 541));
        LCDhor = new QLCDNumber(guiDlg);
        LCDhor->setObjectName(QString::fromUtf8("LCDhor"));
        LCDhor->setEnabled(true);
        LCDhor->setGeometry(QRect(20, 20, 61, 51));
#ifndef QT_NO_ACCESSIBILITY
        LCDhor->setAccessibleName(QString::fromUtf8(""));
#endif // QT_NO_ACCESSIBILITY
        LCDhor->setDigitCount(2);
        LCDmin = new QLCDNumber(guiDlg);
        LCDmin->setObjectName(QString::fromUtf8("LCDmin"));
        LCDmin->setGeometry(QRect(100, 20, 61, 51));
        LCDmin->setDigitCount(2);
        LCDseg = new QLCDNumber(guiDlg);
        LCDseg->setObjectName(QString::fromUtf8("LCDseg"));
        LCDseg->setGeometry(QRect(180, 30, 51, 41));
        LCDseg->setDigitCount(2);
        label = new QLabel(guiDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 0, 66, 17));
        label_2 = new QLabel(guiDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(100, 0, 66, 17));
        label_3 = new QLabel(guiDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(170, 0, 81, 17));
        InfoText = new QTextEdit(guiDlg);
        InfoText->setObjectName(QString::fromUtf8("InfoText"));
        InfoText->setGeometry(QRect(10, 80, 271, 120));
        Reset = new QPushButton(guiDlg);
        Reset->setObjectName(QString::fromUtf8("Reset"));
        Reset->setGeometry(QRect(200, 500, 81, 51));
        label_4 = new QLabel(guiDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(50, 120, 66, 17));
        label_5 = new QLabel(guiDlg);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(30, 250, 221, 151));
        label_5->setPixmap(QPixmap(QString::fromUtf8("../../../../../Im\303\241genes/Captura.PNG")));

        retranslateUi(guiDlg);

        QMetaObject::connectSlotsByName(guiDlg);
    } // setupUi

    void retranslateUi(QWidget *guiDlg)
    {
        guiDlg->setWindowTitle(QApplication::translate("guiDlg", "controller", 0, QApplication::UnicodeUTF8));
        Start->setText(QApplication::translate("guiDlg", "Start", 0, QApplication::UnicodeUTF8));
        Stop->setText(QApplication::translate("guiDlg", "Stop", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        LCDmin->setWhatsThis(QApplication::translate("guiDlg", "<html><head/><body><p><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label->setText(QApplication::translate("guiDlg", "  HORAS", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("guiDlg", "MINUTOS", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("guiDlg", "SEGUNDOS", 0, QApplication::UnicodeUTF8));
        Reset->setText(QApplication::translate("guiDlg", "Reset", 0, QApplication::UnicodeUTF8));
        label_4->setText(QString());
        label_5->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class guiDlg: public Ui_guiDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
