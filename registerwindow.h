#pragma once

#include <QMainWindow>
#include "ui_registerwindow.h"

class registerWindow : public QMainWindow
{
	Q_OBJECT

public:
	registerWindow(QWidget *parent = nullptr);
	~registerWindow();

private slots:
	void on_goToLoginWindow_clicked();
	void on_enterCreateAccount_clicked();

private:
	Ui::registerWindowClass ui;
};

