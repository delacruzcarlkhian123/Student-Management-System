#include "stdafx.h"
#include "deletestudentwindow.h"
#include "mainwindow.h"
#include "centralCode.h"

#include <QString>

#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QMessageBox>

deleteStudentWindow::deleteStudentWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

deleteStudentWindow::~deleteStudentWindow()
{}

void deleteStudentWindow::on_mainWindow_clicked()
{
	mainWindow* mw = new mainWindow();
	mw->show();
	this->hide();
}

QString deleteStudentWindow::deleteStudentInfo(QString& studentName, int& studentAge, QString& studentCourse, int& studentId)
{
	return QString(
		"Are you sure you want to delete this student's information from the database?\n\n"
		"Student Name:       | %1\n"
		"Student Age:          | %2\n"
		"Student Course:     | %3\n"
		"Student ID:             | %4\n")
		.arg(studentName)
		.arg(studentAge)
		.arg(studentCourse)
		.arg(studentId);
}

void deleteStudentWindow::on_enterStudentId_clicked()
{
	CentralManagement centralManagement;

	QString qStringStudentName = ui.studentId->text();
	int studentId;
	bool validOrExistingStudentId = false;

	if (centralManagement.noEmptyLineEditForDeleteAndEditStudent(qStringStudentName))
	{
		if (centralManagement.validWholeNumber(qStringStudentName, studentId))
		{
			if (centralManagement.validIdRange(studentId))
			{
				if (centralManagement.studentIdExist(studentId))
				{
					validOrExistingStudentId = true;
				}
				else { ui.info->setText("student id is not existing"); }
			}
			else { ui.info->setText("student id is outside the range"); }
		}
		else{ ui.info->setText("Invalid student id"); }
	}
	else { ui.info->setText("Please fill in the blank line"); }

	if (validOrExistingStudentId)
	{
		QString studentName;
		int studentAge;
		QString studentCourse;

		QSqlQuery query;
		query.prepare("select student_name, student_age, student_course from students_info where student_id = :studentId");
		query.bindValue(":studentId", studentId);
		if(!(query.exec()))
		{
			QMessageBox::critical(this, "Database Error", query.lastError().text());
			return;
		}
		if(query.next())
		{
			studentName = query.value(0).toString();
			studentAge = query.value(1).toInt();
			studentCourse = query.value(2).toString();
		}

		QString message = deleteStudentInfo(studentName, studentAge, studentCourse, studentId);

		if (QMessageBox::question(this, "Confirmation", message, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			query.prepare("delete from students_info where student_id = :studentId");
			query.bindValue(":studentId", studentId);
			if (!(query.exec()))
			{
				QMessageBox::critical(this, "Database Error", query.lastError().text());
			}
			ui.info->setText("The student information is deleted successfully");
			ui.studentId->clear();
		}
	}
}

