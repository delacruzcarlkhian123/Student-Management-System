#include "stdafx.h"
#include "addstudentwindow.h"
#include "mainwindow.h"
#include "centralCode.h"

#include <QString>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QMessageBox>

addStudentWindow::addStudentWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

addStudentWindow::~addStudentWindow()
{}

QString addStudentWindow::addStudentInfo(QString& studentName, int& studentAge, QString& studentCourse, int& studentId)
{
	return QString(
		"Are you sure you want to add this student's information to the database?\n\n"
		"Student Name:       | %1\n"
		"Student Age:          | %2\n"
		"Student Course:     | %3\n"
		"Student ID:             | %4")
		.arg(studentName)
		.arg(studentAge)
		.arg(studentCourse)
		.arg(studentId);
}

void addStudentWindow::on_checkStudentInfo_clicked()
{
	CentralManagement centralManagement;

	bool validStudentInfo = false;
	QString studentName = ui.studentName->text();
	QString qStringStudentAge = ui.studentAge->text();
	QString studentCourse = ui.studentCourse->text();
	QString qStringStudentId = ui.studentId->text();
	int studentAge;
	int studentId;
	int zero = 0;

	if (centralManagement.noEmptyLineEditForAddStudent(studentName, qStringStudentAge, studentCourse, qStringStudentId))
	{
		if (centralManagement.validStudentName(studentName))
		{
			if (centralManagement.validWholeNumber(qStringStudentAge, studentAge))
			{
				if (centralManagement.validAgeRange(studentAge))
				{
					if (centralManagement.validStudentCourse(studentCourse))
					{
						if (centralManagement.validWholeNumber(qStringStudentId, studentId))
						{
							if (centralManagement.validIdRange(studentId))
							{
								if (centralManagement.idIsNotRepeated(studentId, zero))
								{
									validStudentInfo = true;
								}
								else { ui.info->setText("Id is already existing"); }
							}
							else { ui.info->setText("student id is outside the range"); }
						}
						else { ui.info->setText("The student id is not valid positive whole number"); }
					}
					else { ui.info->setText("Invalid student course"); }
				}
				else { ui.info->setText("Student age is outside the range"); }
			}
			else { ui.info->setText("The student age is not valid positive whole number"); }
		}
		else { ui.info->setText("Invalid student name"); }
	}
	else { ui.info->setText("Please fill in the blank line"); }

	if (validStudentInfo)
	{
		QString message = addStudentInfo(studentName, studentAge, studentCourse, studentId);

		if (QMessageBox::question(this, "Confirmation", message, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			QSqlQuery query;

			query.prepare("insert into students_info (student_name, student_age, student_course, student_id) values (:student_name, :student_age, :student_course, :student_id)");

			query.bindValue(":student_name", studentName);
			query.bindValue(":student_age", studentAge);
			query.bindValue(":student_course", studentCourse);
			query.bindValue(":student_id", studentId);

			if (query.exec())
			{
				ui.info->setText("Student information are added successfully");

				ui.studentName->clear();
				ui.studentAge->clear();
				ui.studentCourse->clear();
				ui.studentId->clear();
			}
			else
			{
				QMessageBox::critical(this, "Database Error", query.lastError().text());
			}
		}
	}
}

void addStudentWindow::on_mainWindow_clicked()
{
	mainWindow* mw = new mainWindow();
	mw->show();
	this->hide();
}

