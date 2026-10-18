#include "stdafx.h"
#include "editstudentwindow.h"
#include "mainwindow.h"
#include "centralCode.h"
#include "editstudentwindow2.h"

#include <QString>

editStudentWindow::editStudentWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

editStudentWindow::~editStudentWindow()
{}

void editStudentWindow::on_mainWindow_clicked()
{
	mainWindow* mw = new mainWindow();
	mw->show();
	this->hide();
}

void editStudentWindow::on_enterStudentId_clicked()
{
	CentralManagement centralManagement;
	QString qStringStudentId = ui.studentId->text();
	int studentId;

	if (centralManagement.noEmptyLineEditForDeleteAndEditStudent(qStringStudentId))
	{
		if (centralManagement.validWholeNumber(qStringStudentId, studentId))
		{
			if (centralManagement.validIdRange(studentId))
			{
				if (centralManagement.studentIdExist(studentId))
				{
					editStudentWindow2* esw2 = new editStudentWindow2(studentId);
					esw2->show();
					this->hide();
				}
				else { ui.info->setText("student id is not existing"); }
			}
			else { ui.info->setText("student id is outside the range"); }
		}
		else { ui.info->setText("Invalid student id"); }
	}
	else { ui.info->setText("Please fill in the blank line"); }
}

