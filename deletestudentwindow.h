#pragma once

#include <QMainWindow>
#include "ui_deletestudentwindow.h"

class deleteStudentWindow : public QMainWindow
{
	Q_OBJECT

public:
	deleteStudentWindow(QWidget *parent = nullptr);
	~deleteStudentWindow();

public:
	QString deleteStudentInfo(QString& studentName, int& studentAge, QString& studentCourse, int& studentId);

private slots:
	void on_mainWindow_clicked();
	void on_enterStudentId_clicked();

private:
	Ui::deleteStudentWindowClass ui;
};

