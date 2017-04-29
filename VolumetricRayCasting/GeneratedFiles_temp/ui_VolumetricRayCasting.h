/********************************************************************************
** Form generated from reading UI file 'VolumetricRayCasting.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOLUMETRICRAYCASTING_H
#define UI_VOLUMETRICRAYCASTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VolumetricRayCastingClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VolumetricRayCastingClass)
    {
        if (VolumetricRayCastingClass->objectName().isEmpty())
            VolumetricRayCastingClass->setObjectName(QStringLiteral("VolumetricRayCastingClass"));
        VolumetricRayCastingClass->resize(600, 400);
        menuBar = new QMenuBar(VolumetricRayCastingClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        VolumetricRayCastingClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VolumetricRayCastingClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        VolumetricRayCastingClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(VolumetricRayCastingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        VolumetricRayCastingClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(VolumetricRayCastingClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        VolumetricRayCastingClass->setStatusBar(statusBar);

        retranslateUi(VolumetricRayCastingClass);

        QMetaObject::connectSlotsByName(VolumetricRayCastingClass);
    } // setupUi

    void retranslateUi(QMainWindow *VolumetricRayCastingClass)
    {
        VolumetricRayCastingClass->setWindowTitle(QApplication::translate("VolumetricRayCastingClass", "VolumetricRayCasting", 0));
    } // retranslateUi

};

namespace Ui {
    class VolumetricRayCastingClass: public Ui_VolumetricRayCastingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLUMETRICRAYCASTING_H
