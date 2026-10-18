#include "stdafx.h"
#include "loginwindows.h"
#include "mainwindow.h"
#include "registerwindow.h"
#include "centralCode.h"
#include <QString>


loginWindows::loginWindows(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

loginWindows::~loginWindows()
{}


void loginWindows::on_enterAccount_clicked()
{
	CentralManagement centralManagement;

	QString userName = ui.userName->text();
	QString passWord = ui.passWord->text();

	if (centralManagement.noEmptyLineEditForLogin(userName, passWord))
	{
		if (centralManagement.usernameExist(userName))
		{
			if(centralManagement.passwordExist(userName, passWord))
			{
				mainWindow* mw = new mainWindow();
				mw->show();
				this->hide();
			}
			else { ui.info->setText("The password is not existing"); }
		}
		else { ui.info->setText("The username is not existing"); }
	}
	else { ui.info->setText("Please fill in the blank line"); }
}


void loginWindows::on_goToCreateAccountWindow_clicked()
{
	registerWindow* rw = new registerWindow();
	rw->show();
	this->hide();
}