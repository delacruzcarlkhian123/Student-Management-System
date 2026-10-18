#pragma once

#include <QMainWindow>
#include "ui_addstudentwindow.h"

class addStudentWindow : public QMainWindow
{
	Q_OBJECT

public:
	addStudentWindow(QWidget *parent = nullptr);
	~addStudentWindow();

public:
	QString addStudentInfo(QString& studentName, int& studentAge, QString& studentCourse, int& studentId);

private slots:
	void on_mainWindow_clicked();
	void on_checkStudentInfo_clicked();

private:
	Ui::addStudentWindowClass ui;
};

