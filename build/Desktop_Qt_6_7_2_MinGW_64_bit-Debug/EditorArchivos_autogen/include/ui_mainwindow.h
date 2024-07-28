/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *mbSeleccionarArchivo;
    QAction *mbGuardarArchivo;
    QWidget *wCentralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QFrame *line;
    QPlainTextEdit *pteEditor;
    QMenuBar *menubar;
    QMenu *mbArchivo;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(555, 383);
        MainWindow->setMinimumSize(QSize(555, 0));
        mbSeleccionarArchivo = new QAction(MainWindow);
        mbSeleccionarArchivo->setObjectName("mbSeleccionarArchivo");
        mbGuardarArchivo = new QAction(MainWindow);
        mbGuardarArchivo->setObjectName("mbGuardarArchivo");
        wCentralWidget = new QWidget(MainWindow);
        wCentralWidget->setObjectName("wCentralWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(wCentralWidget->sizePolicy().hasHeightForWidth());
        wCentralWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(wCentralWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        line = new QFrame(wCentralWidget);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        verticalLayout->addWidget(line);

        pteEditor = new QPlainTextEdit(wCentralWidget);
        pteEditor->setObjectName("pteEditor");
        sizePolicy.setHeightForWidth(pteEditor->sizePolicy().hasHeightForWidth());
        pteEditor->setSizePolicy(sizePolicy);
        pteEditor->setAutoFillBackground(true);
        pteEditor->setLineWidth(0);

        verticalLayout->addWidget(pteEditor);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(wCentralWidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 555, 22));
        mbArchivo = new QMenu(menubar);
        mbArchivo->setObjectName("mbArchivo");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(mbArchivo->menuAction());
        mbArchivo->addAction(mbSeleccionarArchivo);
        mbArchivo->addAction(mbGuardarArchivo);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        mbSeleccionarArchivo->setText(QCoreApplication::translate("MainWindow", "Seleccionar archivo", nullptr));
        mbGuardarArchivo->setText(QCoreApplication::translate("MainWindow", "Guardar", nullptr));
        mbArchivo->setTitle(QCoreApplication::translate("MainWindow", "Archivo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
