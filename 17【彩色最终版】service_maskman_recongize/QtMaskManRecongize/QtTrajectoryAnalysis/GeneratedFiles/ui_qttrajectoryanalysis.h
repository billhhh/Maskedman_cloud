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
#include <QtGui/QFrame>
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
    QPushButton *pBtn_OutputFolder;
    QLabel *label;
    QToolButton *pBtn_Run;
    QPushButton *pBtn_minimize;
    QPushButton *pBtn_exit;
    QWidget *rightWidget;
    QFrame *line;
    QLabel *label_3;
    QTextBrowser *textBrowser;
    QToolButton *pBtn_forward;
    QToolButton *pBtn_backward;

    void setupUi(QDialog *QtTrajectoryAnalysisClass)
    {
        if (QtTrajectoryAnalysisClass->objectName().isEmpty())
            QtTrajectoryAnalysisClass->setObjectName(QString::fromUtf8("QtTrajectoryAnalysisClass"));
        QtTrajectoryAnalysisClass->resize(1080, 720);
        QtTrajectoryAnalysisClass->setMinimumSize(QSize(1080, 720));
        QtTrajectoryAnalysisClass->setMaximumSize(QSize(1080, 720));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/QtTrajectoryAnalysis/Resources/maskManDetect.ico"), QSize(), QIcon::Normal, QIcon::Off);
        QtTrajectoryAnalysisClass->setWindowIcon(icon);
        widget_background = new QWidget(QtTrajectoryAnalysisClass);
        widget_background->setObjectName(QString::fromUtf8("widget_background"));
        widget_background->setGeometry(QRect(0, 0, 1080, 720));
        widget_background->setStyleSheet(QString::fromUtf8("QWidget#widget_background{border-image: url(:/QtTrajectoryAnalysis/Resources/background.png)}"));
        progress_bar = new QProgressBar(widget_background);
        progress_bar->setObjectName(QString::fromUtf8("progress_bar"));
        progress_bar->setGeometry(QRect(30, 470, 480, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(10);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(9);
        progress_bar->setFont(font);
        progress_bar->setStyleSheet(QString::fromUtf8(" QProgressBar{\n"
"font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #ffffff;\n"
"border-radius:2px;\n"
"	background-color:#156079;\n"
"}\n"
"QProgressBar::chunk {\n"
"	background-color: rgb(52, 52, 52);\n"
"    border-bottom:1px solid gray;\n"
"}"));
        progress_bar->setValue(0);
        progress_bar->setAlignment(Qt::AlignCenter);
        lEd_OutputFolder = new QLineEdit(widget_background);
        lEd_OutputFolder->setObjectName(QString::fromUtf8("lEd_OutputFolder"));
        lEd_OutputFolder->setEnabled(true);
        lEd_OutputFolder->setGeometry(QRect(130, 510, 380, 40));
        lEd_OutputFolder->setStyleSheet(QString::fromUtf8("text-indent:5px;\n"
"font: 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: #403e3e;\n"
"background-color: #ffffff;"));
        imageView = new QGraphicsView(widget_background);
        imageView->setObjectName(QString::fromUtf8("imageView"));
        imageView->setGeometry(QRect(30, 110, 480, 360));
        imageView->setAutoFillBackground(true);
        imageView->setStyleSheet(QString::fromUtf8("border-image: url(:/QtTrajectoryAnalysis/Resources/bottomOfImage.png);"));
        label_2 = new QLabel(widget_background);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(440, -170, 111, 31));
        label_2->setStyleSheet(QString::fromUtf8("\n"
"font: 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        pBtn_OutputFolder = new QPushButton(widget_background);
        pBtn_OutputFolder->setObjectName(QString::fromUtf8("pBtn_OutputFolder"));
        pBtn_OutputFolder->setGeometry(QRect(30, 510, 102, 40));
        pBtn_OutputFolder->setAutoFillBackground(false);
        pBtn_OutputFolder->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/QtTrajectoryAnalysis/Resources/savePath.png);}\n"
"QPushButton:hover{border-image: url(:/QtTrajectoryAnalysis/Resources/savePath-hover.png);}\n"
"QPushButton:pressed{border-image: url(:/QtTrajectoryAnalysis/Resources/savePath-pressed.png);}\n"
""));
        pBtn_OutputFolder->setIconSize(QSize(32, 32));
        label = new QLabel(widget_background);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(943, 48, 109, 19));
        label->setLayoutDirection(Qt::RightToLeft);
        label->setStyleSheet(QString::fromUtf8("image: url(:/QtTrajectoryAnalysis/Resources/maskManDetectLogo.png);"));
        label->setAlignment(Qt::AlignCenter);
        pBtn_Run = new QToolButton(widget_background);
        pBtn_Run->setObjectName(QString::fromUtf8("pBtn_Run"));
        pBtn_Run->setGeometry(QRect(230, 610, 80, 80));
        pBtn_Run->setStyleSheet(QString::fromUtf8("QToolButton{border-image: url(:/QtTrajectoryAnalysis/Resources/play.png);}\n"
"QToolButton:hover{border-image: url(:/QtTrajectoryAnalysis/Resources/play-hover.png);}\n"
"QToolButton:pressed{border-image: url(:/QtTrajectoryAnalysis/Resources/play-pressed.png);}\n"
""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/QtTrajectoryAnalysis/Resources/startanalyze.png"), QSize(), QIcon::Normal, QIcon::Off);
        pBtn_Run->setIcon(icon1);
        pBtn_Run->setIconSize(QSize(60, 60));
        pBtn_Run->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        pBtn_minimize = new QPushButton(widget_background);
        pBtn_minimize->setObjectName(QString::fromUtf8("pBtn_minimize"));
        pBtn_minimize->setGeometry(QRect(1020, 10, 20, 20));
        pBtn_minimize->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/QtTrajectoryAnalysis/Resources/minimize.png);}\n"
"QPushButton:hover{border-image: url(:/QtTrajectoryAnalysis/Resources/minimize-mouse.png);}\n"
"QPushButton:pressed{border-image: url(:/QtTrajectoryAnalysis/Resources/minimize-mouse.png);}\n"
""));
        pBtn_minimize->setIconSize(QSize(25, 25));
        pBtn_exit = new QPushButton(widget_background);
        pBtn_exit->setObjectName(QString::fromUtf8("pBtn_exit"));
        pBtn_exit->setGeometry(QRect(1050, 10, 20, 20));
        pBtn_exit->setStyleSheet(QString::fromUtf8("QPushButton{border-image: url(:/QtTrajectoryAnalysis/Resources/exit.png);}\n"
"QPushButton:hover{border-image: url(:/QtTrajectoryAnalysis/Resources/exit-mouse.png);}\n"
"QPushButton:pressed{border-image: url(:/QtTrajectoryAnalysis/Resources/exit-mouse.png);}\n"
""));
        pBtn_exit->setIconSize(QSize(25, 25));
        rightWidget = new QWidget(widget_background);
        rightWidget->setObjectName(QString::fromUtf8("rightWidget"));
        rightWidget->setGeometry(QRect(620, 110, 431, 580));
        line = new QFrame(rightWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(0, 30, 580, 8));
        line->setStyleSheet(QString::fromUtf8("background-color: #0d4051"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_3 = new QLabel(rightWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 0, 580, 39));
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(178, 178, 178);\n"
"background-color: #0d4051;\n"
"font: 75 13pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; "));
        textBrowser = new QTextBrowser(rightWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(0, 40, 431, 540));
        textBrowser->setStyleSheet(QString::fromUtf8("\n"
"QScrollBar:vertical\n"
"{\n"
"    width:25px;\n"
"    background:rgba(0,0,0,0%);\n"
"    margin:0px,0px,0px,0px;\n"
"    padding-top:22px;   \n"
"    padding-bottom:22px;\n"
"}\n"
"QScrollBar::handle:vertical\n"
"{\n"
"    width:25px;\n"
"	background-color: #03242e;\n"
"    border-radius:5px;   \n"
"    min-height:20;\n"
"}\n"
"QScrollBar::sub-line:vertical  \n"
"{\n"
"    height:22px;width:25px;\n"
"	border-image: url(:/QtTrajectoryAnalysis/Resources/uparrow.png);\n"
"    subcontrol-position:top;\n"
"}\n"
"QScrollBar::add-line:vertical{\n"
"    height:22px;width:25px;\n"
"    border-image: url(:/QtTrajectoryAnalysis/Resources/downarrow.png);\n"
"    subcontrol-position:bottom;\n"
"}\n"
"\n"
"QTextBrowser{font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\"; \n"
"color: rgb(255, 255, 255);\n"
"background-color: #0d4051;\n"
"}"));
        textBrowser->setFrameShape(QFrame::NoFrame);
        textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        label_3->raise();
        textBrowser->raise();
        line->raise();
        pBtn_forward = new QToolButton(widget_background);
        pBtn_forward->setObjectName(QString::fromUtf8("pBtn_forward"));
        pBtn_forward->setGeometry(QRect(330, 638, 20, 23));
        pBtn_forward->setStyleSheet(QString::fromUtf8("QToolButton{border-image: url(:/QtTrajectoryAnalysis/Resources/forward.png);}\n"
"QToolButton:hover{border-image: url(:/QtTrajectoryAnalysis/Resources/forward-hover.png);}\n"
"QToolButton:pressed{border-image: url(:/QtTrajectoryAnalysis/Resources/forward-pressed.png);}\n"
""));
        pBtn_backward = new QToolButton(widget_background);
        pBtn_backward->setObjectName(QString::fromUtf8("pBtn_backward"));
        pBtn_backward->setGeometry(QRect(190, 638, 20, 23));
        pBtn_backward->setStyleSheet(QString::fromUtf8("QToolButton{border-image: url(:/QtTrajectoryAnalysis/Resources/back.png);}\n"
"QToolButton:hover{border-image: url(:/QtTrajectoryAnalysis/Resources/back-hover.png);}\n"
"QToolButton:pressed{border-image: url(:/QtTrajectoryAnalysis/Resources/backward-pressed.png);}\n"
""));

        retranslateUi(QtTrajectoryAnalysisClass);

        QMetaObject::connectSlotsByName(QtTrajectoryAnalysisClass);
    } // setupUi

    void retranslateUi(QDialog *QtTrajectoryAnalysisClass)
    {
        QtTrajectoryAnalysisClass->setWindowTitle(QApplication::translate("QtTrajectoryAnalysisClass", "QtMaskManRec", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("QtTrajectoryAnalysisClass", "<html><head/><body><p><span style=\" font-weight:600;\">\344\272\213\344\273\266\344\277\241\346\201\257\357\274\232</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        pBtn_OutputFolder->setText(QString());
        label->setText(QString());
        pBtn_Run->setText(QString());
        pBtn_minimize->setText(QString());
        pBtn_exit->setText(QString());
        label_3->setText(QApplication::translate("QtTrajectoryAnalysisClass", "    \350\222\231\351\235\242\344\272\272\350\257\206\345\210\253\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        pBtn_forward->setText(QString());
        pBtn_backward->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QtTrajectoryAnalysisClass: public Ui_QtTrajectoryAnalysisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTTRAJECTORYANALYSIS_H
