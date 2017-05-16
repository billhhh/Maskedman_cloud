/********************************************************************************
** Form generated from reading UI file 'waitingprogressbar.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAITINGPROGRESSBAR_H
#define UI_WAITINGPROGRESSBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaitingProgressBar
{
public:
    QWidget *widget;
    QLabel *label_file;
    QLabel *label;
    QCommandLinkButton *cmdLinkBtn;
    QLabel *label_progress;
    QProgressBar *progressBar;

    void setupUi(QWidget *WaitingProgressBar)
    {
        if (WaitingProgressBar->objectName().isEmpty())
            WaitingProgressBar->setObjectName(QString::fromUtf8("WaitingProgressBar"));
        WaitingProgressBar->setWindowModality(Qt::ApplicationModal);
        WaitingProgressBar->resize(500, 200);
        WaitingProgressBar->setMinimumSize(QSize(500, 200));
        WaitingProgressBar->setMaximumSize(QSize(500, 200));
        widget = new QWidget(WaitingProgressBar);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 500, 200));
        widget->setMinimumSize(QSize(500, 200));
        widget->setMaximumSize(QSize(500, 200));
        widget->setStyleSheet(QString::fromUtf8("QWidget#widget{\n"
"border-image: url(:/QtTrajectoryAnalysis/Resources/processor_bg.png)}"));
        label_file = new QLabel(widget);
        label_file->setObjectName(QString::fromUtf8("label_file"));
        label_file->setGeometry(QRect(170, 30, 141, 31));
        label_file->setStyleSheet(QString::fromUtf8("font: 75 8pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 30, 141, 31));
        label->setStyleSheet(QString::fromUtf8("font: 75 8pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;"));
        cmdLinkBtn = new QCommandLinkButton(widget);
        cmdLinkBtn->setObjectName(QString::fromUtf8("cmdLinkBtn"));
        cmdLinkBtn->setGeometry(QRect(240, 130, 171, 41));
        cmdLinkBtn->setStyleSheet(QString::fromUtf8("font: 75 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;"));
        label_progress = new QLabel(widget);
        label_progress->setObjectName(QString::fromUtf8("label_progress"));
        label_progress->setGeometry(QRect(370, 30, 81, 31));
        label_progress->setStyleSheet(QString::fromUtf8("font: 75 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;"));
        label_progress->setAlignment(Qt::AlignCenter);
        progressBar = new QProgressBar(widget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(59, 80, 381, 20));
        progressBar->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;"));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setTextVisible(true);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setTextDirection(QProgressBar::TopToBottom);

        retranslateUi(WaitingProgressBar);

        QMetaObject::connectSlotsByName(WaitingProgressBar);
    } // setupUi

    void retranslateUi(QWidget *WaitingProgressBar)
    {
        WaitingProgressBar->setWindowTitle(QApplication::translate("WaitingProgressBar", "WaitingProgressBar", 0, QApplication::UnicodeUTF8));
        label_file->setText(QApplication::translate("WaitingProgressBar", "<html><head/><body><p><span style=\" font-size:14pt;\">\346\226\207\344\273\266\345\220\215</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WaitingProgressBar", "<html><head/><body><p><span style=\" font-size:14pt;\">\345\275\223\345\211\215\345\244\204\347\220\206\346\226\207\344\273\266\357\274\232</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        cmdLinkBtn->setText(QApplication::translate("WaitingProgressBar", "\346\224\276\345\274\203\347\273\247\347\273\255\350\277\233\350\241\214\345\244\204\347\220\206", 0, QApplication::UnicodeUTF8));
        label_progress->setText(QApplication::translate("WaitingProgressBar", "<html><head/><body><p>i/n<br/></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WaitingProgressBar: public Ui_WaitingProgressBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITINGPROGRESSBAR_H
