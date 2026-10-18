#include "stdafx.h"
#include "registerwindow.h"
#include "loginwindows.h"
#include "centralCode.h"
#include <QString>

#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QMessageBox>

registerWindow::registerWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

registerWindow::~registerWindow()
{}

void registerWindow::on_goToLoginWindow_clicked()
{
	loginWindows* lw = new loginWindows();
	lw->show();
	this->hide();
}

void registerWindow::on_enterCreateAccount_clicked()
{
	CentralManagement centralManagement;

	QString userName = ui.createUserName->text();
	QString passWord = ui.createPassword->text();
	QString confirmPassWord = ui.confirmCreatePassword->text();

	if (centralManagement.noEmptyLineEditForRegister(userName, passWord, confirmPassWord))
	{
		if (centralManagement.userNameIsNotRepeated(userName))
		{
			if (centralManagement.samePassWordAndConfirmPassWord(userName, passWord, confirmPassWord))
			{
				ui.userInfo->setText("Created account successfully");

				ui.createUserName->clear();
				ui.createPassword->clear();
				ui.confirmCreatePassword->clear();
			}
			else {ui.userInfo->setText("Password and confirm password are not same");}
		}
		else {ui.userInfo->setText("The username is already existing, try another username");}
	}
	else {ui.userInfo->setText("Please fill in the blank line");}
}

