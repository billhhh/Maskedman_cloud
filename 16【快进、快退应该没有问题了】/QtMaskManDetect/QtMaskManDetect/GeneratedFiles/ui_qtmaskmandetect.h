/********************************************************************************
** Form generated from reading UI file 'qtmaskmandetect.ui'
**
** Created: Tue Mar 24 16:38:22 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTMASKMANDETECT_H
#define UI_QTMASKMANDETECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtMaskManDetectClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtMaskManDetectClass)
    {
        if (QtMaskManDetectClass->objectName().isEmpty())
            QtMaskManDetectClass->setObjectName(QString::fromUtf8("QtMaskManDetectClass"));
        QtMaskManDetectClass->resize(600, 400);
        menuBar = new QMenuBar(QtMaskManDetectClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        QtMaskManDetectClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtMaskManDetectClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtMaskManDetectClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QtMaskManDetectClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QtMaskManDetectClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtMaskManDetectClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtMaskManDetectClass->setStatusBar(statusBar);

        retranslateUi(QtMaskManDetectClass);

        QMetaObject::connectSlotsByName(QtMaskManDetectClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtMaskManDetectClass)
    {
        QtMaskManDetectClass->setWindowTitle(QApplication::translate("QtMaskManDetectClass", "QtMaskManDetect", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QtMaskManDetectClass: public Ui_QtMaskManDetectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTMASKMANDETECT_H
