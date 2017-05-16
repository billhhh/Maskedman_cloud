/********************************************************************************
** Form generated from reading UI file 'qttrajectoryanalysis.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTTRAJECTORYANALYSIS_H
#define UI_QTTRAJECTORYANALYSIS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtTrajectoryAnalysisClass
{
public:
    QWidget *widget_background;
    QProgressBar *progress_bar;
    QLineEdit *lEd_OutputFolder;
    QGraphicsView *imageView;
    QLabel *label_2;
    QTextBrowser *textBrowser;
    QPushButton *pBtn_OutputFolder;
    QLabel *label;
    QPushButton *pBtn_quit;
    QToolButton *pBtn_Run;
    QToolButton *pBtn_Stop;

    void setupUi(QDialog *QtTrajectoryAnalysisClass)
    {
        if (QtTrajectoryAnalysisClass->objectName().isEmpty())
            QtTrajectoryAnalysisClass->setObjectName(QString::fromUtf8("QtTrajectoryAnalysisClass"));
        QtTrajectoryAnalysisClass->resize(900, 600);
        QtTrajectoryAnalysisClass->setMinimumSize(QSize(900, 600));
        QtTrajectoryAnalysisClass->setMaximumSize(QSize(900, 600));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/QtTrajectoryAnalysis/Resources/maskManDetect.ico"), QSize(), QIcon::Normal, QIcon::Off);
        QtTrajectoryAnalysisClass->setWindowIcon(icon);
        widget_background = new QWidget(QtTrajectoryAnalysisClass);
        widget_background->setObjectName(QString::fromUtf8("widget_background"));
        widget_background->setGeometry(QRect(0, 0, 900, 600));
        widget_background->setStyleSheet(QString::fromUtf8("QWidget#widget_background{border-image: url(:/QtTrajectoryAnalysis/Resources/background.png)}"));
        progress_bar = new QProgressBar(widget_background);
        progress_bar->setObjectName(QString::fromUtf8("progress_bar"));
        progress_bar->setGeometry(QRect(70, 385, 360, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(10);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(9);
        progress_bar->setFont(font);
        progress_bar->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;"));
        progress_bar->setValue(0);
        progress_bar->setAlignment(Qt::AlignCenter);
        lEd_OutputFolder = new QLineEdit(widget_background);
        lEd_OutputFolder->setObjectName(QString::fromUtf8("lEd_OutputFolder"));
        lEd_OutputFolder->setGeometry(QRect(140, 415, 291, 21));
        lEd_OutputFolder->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(60, 60, 60);\n"
"background-color: #e4e9e9;"));
        imageView = new QGraphicsView(widget_background);
        imageView->setObjectName(QString::fromUtf8("imageView"));
        imageView->setGeometry(QRect(70, 115, 360, 270));
        imageView->setAutoFillBackground(true);
        label_2 = new QLabel(widget_background);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(440, -170, 111, 31));
        label_2->setStyleSheet(QString::fromUtf8("\n"
"font: 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        textBrowser = new QTextBrowser(widget_background);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(470, 115, 360, 321));
        textBrowser->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;\n"
"background-color: #eeeeee;"));
        textBrowser->setFrameShape(QFrame::WinPanel);
        textBrowser->setFrameShadow(QFrame::Raised);
        textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        pBtn_OutputFolder = new QPushButton(widget_background);
        pBtn_OutputFolder->setObjectName(QString::fromUtf8("pBtn_OutputFolder"));
        pBtn_OutputFolder->setGeometry(QRect(70, 415, 61, 21));
        pBtn_OutputFolder->setAutoFillBackground(false);
        pBtn_OutputFolder->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;\n"
"background-color: #f7a562"));
        pBtn_OutputFolder->setIconSize(QSize(32, 32));
        label = new QLabel(widget_background);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(630, 470, 201, 61));
        label->setLayoutDirection(Qt::RightToLeft);
        label->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;"));
        label->setAlignment(Qt::AlignCenter);
        pBtn_quit = new QPushButton(widget_background);
        pBtn_quit->setObjectName(QString::fromUtf8("pBtn_quit"));
        pBtn_quit->setEnabled(true);
        pBtn_quit->setGeometry(QRect(820, 515, 25, 25));
        pBtn_quit->setStyleSheet(QString::fromUtf8("border-style:hidden;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/QtTrajectoryAnalysis/Resources/quit.png"), QSize(), QIcon::Normal, QIcon::Off);
        pBtn_quit->setIcon(icon1);
        pBtn_quit->setIconSize(QSize(25, 25));
        pBtn_Run = new QToolButton(widget_background);
        pBtn_Run->setObjectName(QString::fromUtf8("pBtn_Run"));
        pBtn_Run->setGeometry(QRect(270, 450, 60, 80));
        pBtn_Run->setStyleSheet(QString::fromUtf8("border-style:hidden;\n"
"font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/QtTrajectoryAnalysis/Resources/startanalyze.png"), QSize(), QIcon::Normal, QIcon::Off);
        pBtn_Run->setIcon(icon2);
        pBtn_Run->setIconSize(QSize(60, 60));
        pBtn_Run->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        pBtn_Stop = new QToolButton(widget_background);
        pBtn_Stop->setObjectName(QString::fromUtf8("pBtn_Stop"));
        pBtn_Stop->setGeometry(QRect(370, 450, 60, 80));
        pBtn_Stop->setStyleSheet(QString::fromUtf8("border-style:hidden;\n"
"font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #4f4f4f;"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/QtTrajectoryAnalysis/Resources/stopanalyze.png"), QSize(), QIcon::Normal, QIcon::Off);
        pBtn_Stop->setIcon(icon3);
        pBtn_Stop->setIconSize(QSize(60, 60));
        pBtn_Stop->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        retranslateUi(QtTrajectoryAnalysisClass);

        QMetaObject::connectSlotsByName(QtTrajectoryAnalysisClass);
    } // setupUi

    void retranslateUi(QDialog *QtTrajectoryAnalysisClass)
    {
        QtTrajectoryAnalysisClass->setWindowTitle(QApplication::translate("QtTrajectoryAnalysisClass", "QtMaskManRec", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("QtTrajectoryAnalysisClass", "<html><head/><body><p><span style=\" font-weight:600;\">\344\272\213\344\273\266\344\277\241\346\201\257\357\274\232</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        pBtn_OutputFolder->setText(QApplication::translate("QtTrajectoryAnalysisClass", "\344\277\235\345\255\230\350\267\257\345\276\204", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QtTrajectoryAnalysisClass", "\350\222\231\351\235\242\344\272\272\346\243\200\346\265\213\346\234\215\345\212\241", 0, QApplication::UnicodeUTF8));
        pBtn_quit->setText(QString());
        pBtn_Run->setText(QApplication::translate("QtTrajectoryAnalysisClass", "\346\243\200\346\265\213\345\274\200\345\247\213", 0, QApplication::UnicodeUTF8));
        pBtn_Stop->setText(QApplication::translate("QtTrajectoryAnalysisClass", "\346\243\200\346\265\213\344\270\255\346\255\242", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QtTrajectoryAnalysisClass: public Ui_QtTrajectoryAnalysisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTTRAJECTORYANALYSIS_H
