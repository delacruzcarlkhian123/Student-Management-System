#pragma once

#include <QMainWindow>
#include "ui_editstudentwindow2.h"

class editStudentWindow2 : public QMainWindow
{
	Q_OBJECT

private:
	int currentStudentId;

public:
	editStudentWindow2(int studentId, QWidget* parent = nullptr);
	~editStudentWindow2();

private slots:
	void on_backToEditStudentWindow1_clicked();
	void on_enterNewChange_clicked();

private:
	Ui::editStudentWindow2Class ui;
};

