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
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaitingProgressBar
{
public:
    QWidget *widget;
    QLabel *label_file;
    QLabel *label;
    QCommandLinkButton *cmdLinkBtn;
    QProgressBar *progressBar;
    QPushButton *pBtn_minimize;
    QPushButton *pBtn_exit;
    QLabel *label_progress;

    void setupUi(QWidget *WaitingProgressBar)
    {
        if (WaitingProgressBar->objectName().isEmpty())
            WaitingProgressBar->setObjectName(QString::fromUtf8("WaitingProgressBar"));
        WaitingProgressBar->setWindowModality(Qt::ApplicationModal);
        WaitingProgressBar->resize(540, 310);
        WaitingProgressBar->setMinimumSize(QSize(540, 310));
        WaitingProgressBar->setMaximumSize(QSize(540, 310));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/QtTrajectoryAnalysis/Resources/maskManDetect.ico"), QSize(), QIcon::Normal, QIcon::Off);
        WaitingProgressBar->setWindowIcon(icon);
        widget = new QWidget(WaitingProgressBar);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 540, 310));
        widget->setMinimumSize(QSize(540, 310));
        widget->setMaximumSize(QSize(540, 310));
        widget->setStyleSheet(QString::fromUtf8("QWidget#widget{border-image: url(:/QtTrajectoryAnalysis/Resources/bottom.png)}"));
        label_file = new QLabel(widget);
        label_file->setObjectName(QString::fromUtf8("label_file"));
        label_file->setGeometry(QRect(185, 60, 211, 41));
        label_file->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(178, 178, 178);"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 60, 171, 41));
        label->setStyleSheet(QString::fromUtf8("font: 75 8pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(178, 178, 178);"));
        cmdLinkBtn = new QCommandLinkButton(widget);
        cmdLinkBtn->setObjectName(QString::fromUtf8("cmdLinkBtn"));
        cmdLinkBtn->setGeometry(QRect(260, 230, 185, 41));
        cmdLinkBtn->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(170, 255, 255);"));
        progressBar = new QProgressBar(widget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(60, 150, 411, 23));
        progressBar->setStyleSheet(QString::fromUtf8("QProgressBar{\n"
"font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"border-radius:2px;\n"
"color:#fff;\n"
"	background-color:#156079;\n"
"}\n"
"QProgressBar::chunk {\n"
"	background-color: rgb(52, 52, 52);\n"
"    border-bottom:1px solid gray;\n"
"}"));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setTextVisible(true);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setTextDirection(QProgressBar::TopToBottom);
        pBtn_minimize = new QPushButton(WaitingProgressBar);
        pBtn_minimize->setObjectName(QString::fromUtf8("pBtn_minimize"));
        pBtn_minimize->setGeometry(QRect(480, 10, 20, 20));
        pBtn_minimize->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/QtTrajectoryAnalysis/Resources/minimize.png);}\n"
"QPushButton:hover{border-image: url(:/QtTrajectoryAnalysis/Resources/minimize-mouse.png);}\n"
"QPushButton:pressed{border-image: url(:/QtTrajectoryAnalysis/Resources/minimize-mouse.png);}\n"
""));
        pBtn_minimize->setIconSize(QSize(25, 25));
        pBtn_exit = new QPushButton(WaitingProgressBar);
        pBtn_exit->setObjectName(QString::fromUtf8("pBtn_exit"));
        pBtn_exit->setGeometry(QRect(510, 10, 20, 20));
        pBtn_exit->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/QtTrajectoryAnalysis/Resources/exit.png);}\n"
"QPushButton:hover{border-image: url(:/QtTrajectoryAnalysis/Resources/exit-mouse.png);}\n"
"QPushButton:pressed{border-image: url(:/QtTrajectoryAnalysis/Resources/exit-mouse.png);}\n"
""));
        pBtn_exit->setIconSize(QSize(25, 25));
        label_progress = new QLabel(WaitingProgressBar);
        label_progress->setObjectName(QString::fromUtf8("label_progress"));
        label_progress->setGeometry(QRect(360, 90, 161, 41));
        label_progress->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(178, 178, 178);"));
        label_progress->setAlignment(Qt::AlignCenter);

        retranslateUi(WaitingProgressBar);

        QMetaObject::connectSlotsByName(WaitingProgressBar);
    } // setupUi

    void retranslateUi(QWidget *WaitingProgressBar)
    {
        WaitingProgressBar->setWindowTitle(QApplication::translate("WaitingProgressBar", "WaitingProgressBar", 0, QApplication::UnicodeUTF8));
        label_file->setText(QApplication::translate("WaitingProgressBar", "<html><head/><body><p><span style=\" font-size:14pt;\">\346\226\207\344\273\266\345\220\215</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WaitingProgressBar", "<html><head/><body><p><span style=\" font-size:14pt;\">\345\275\223\345\211\215\345\244\204\347\220\206\346\226\207\344\273\266\357\274\232</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        cmdLinkBtn->setText(QApplication::translate("WaitingProgressBar", "\346\224\276\345\274\203\347\273\247\347\273\255\350\277\233\350\241\214\345\244\204\347\220\206", 0, QApplication::UnicodeUTF8));
        pBtn_minimize->setText(QString());
        pBtn_exit->setText(QString());
        label_progress->setText(QApplication::translate("WaitingProgressBar", "<html><head/><body><p><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WaitingProgressBar: public Ui_WaitingProgressBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITINGPROGRESSBAR_H
