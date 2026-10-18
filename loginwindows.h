#pragma once

#include <QMainWindow>
#include "ui_loginwindows.h"

class loginWindows : public QMainWindow
{
	Q_OBJECT

public:
	loginWindows(QWidget *parent = nullptr);
	~loginWindows();

private slots:
	void on_enterAccount_clicked();
	void on_goToCreateAccountWindow_clicked();

private:
	Ui::loginWindowsClass ui;
};

