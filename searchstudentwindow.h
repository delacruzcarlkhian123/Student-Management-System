#pragma once

#include <QMainWindow>
#include "ui_searchstudentwindow.h"

class searchStudentWindow : public QMainWindow
{
	Q_OBJECT

public:
	searchStudentWindow(QWidget *parent = nullptr);
	~searchStudentWindow();

private slots:
	void on_mainWindow_clicked();
	void on_enterSearchStudent_clicked();

private:
	Ui::searchStudentWindowClass ui;
};

