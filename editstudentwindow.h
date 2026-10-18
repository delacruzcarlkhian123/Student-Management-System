#pragma once

#include <QMainWindow>
#include "ui_editstudentwindow.h"

class editStudentWindow : public QMainWindow
{
	Q_OBJECT

public:
	editStudentWindow(QWidget *parent = nullptr);
	~editStudentWindow();

private slots:
	void on_mainWindow_clicked();
	void on_enterStudentId_clicked();

private:
	Ui::editStudentWindowClass ui;
};

