#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private slots:
    void on_addStudent_clicked();
    void on_deleteStudent_clicked();
    void on_editStudent_clicked();
    void on_searchStudent_clicked();

private:
    Ui::mainWindowClass ui;
};

