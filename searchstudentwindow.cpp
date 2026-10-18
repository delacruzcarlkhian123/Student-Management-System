#include "stdafx.h"
#include "searchstudentwindow.h"
#include "mainwindow.h"
#include "centralCode.h"
#include "ui_searchstudentwindow.h"

#include <QString> 

#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>

#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QMessageBox>

searchStudentWindow::searchStudentWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

searchStudentWindow::~searchStudentWindow()
{}

void searchStudentWindow::on_mainWindow_clicked()
{
	mainWindow* mw = new mainWindow();
	mw->show();
	this->hide();
}

void searchStudentWindow::on_enterSearchStudent_clicked()
{
	QString qStringSearchStudent = ui.searchStudentInput->text();
	int searchStudent;
	bool searchResultExist = false;

	CentralManagement centralManagement;

	if (centralManagement.noEmptyLineEditForSearchStudent(qStringSearchStudent))
	{
		QSqlTableModel* model = new QSqlTableModel(this);

		model->setTable("students_info");
		model->setEditStrategy(QSqlTableModel::OnManualSubmit);

		if(centralManagement.validWholeNumber(qStringSearchStudent, searchStudent))
		{
			if(centralManagement.validIdRange(searchStudent))
			{
				if (centralManagement.studentIdExist(searchStudent))
				{
					model->setFilter(QString("student_id = %1").arg(searchStudent));
					ui.info->setText("Student ID search successfully");
					searchResultExist = true;
				}
				else { ui.info->setText("Student ID is not existing"); }
			}
			else 
			{
				if(centralManagement.validAgeRange(searchStudent))
				{ 
					if (centralManagement.studentAgeExist(searchStudent))
					{
						model->setFilter(QString("student_age = %1").arg(searchStudent));
						ui.info->setText("Student age search successfully");
						searchResultExist = true;
					}
					else { ui.info->setText("Student age is not existing"); }
				}
				else{ ui.info->setText("Student ID or age are both outside the range"); }
			}
		}
		else 
		{
			if (centralManagement.validStudentName(qStringSearchStudent))
			{
				if (centralManagement.studentNameExist(qStringSearchStudent))
				{
					QString escapeName = qStringSearchStudent;
					escapeName.replace("'", "''");
					model->setFilter(QString("student_name = '%1'").arg(escapeName));
					ui.info->setText("Student name search successfully");
					searchResultExist = true;
				}
				else { ui.info->setText("Student name is not existing"); }
			}
			else 
			{
				if (centralManagement.validStudentCourse(qStringSearchStudent))
				{
					if (centralManagement.studentCourseExist(qStringSearchStudent))
					{
						QString escapeName = qStringSearchStudent;
						escapeName.replace("'", "''");
						model->setFilter(QString("student_course = '%1'").arg(escapeName));
						ui.info->setText("Student course search successfully");
						searchResultExist = true;
					}
					else { ui.info->setText("Student course is not existing"); }
				}
				else { ui.info->setText("Invalid input. Please enter a valid name, course, age, or ID."); }
			}
		}

		if (searchResultExist)
		{
			if (!(model->select()))
			{
				QMessageBox::critical(this, "Database Error: ", model->lastError().text());
				return;
			}

			ui.studentTableView->setModel(model);
			ui.studentTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
			ui.studentTableView->resizeRowsToContents();
			ui.searchStudentInput->clear();
		}

	}
	else { ui.info->setText("Please fill in the blank line"); }
}
