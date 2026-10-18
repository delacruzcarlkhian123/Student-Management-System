#include "stdafx.h"
#include "editstudentwindow2.h"
#include "editstudentwindow.h"
#include "ui_editstudentwindow2.h"
#include "centralCode.h"

#include <QString>

#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>

#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QMessageBox>

editStudentWindow2::editStudentWindow2(int studentId, QWidget *parent)
	: QMainWindow(parent), 
	  currentStudentId(studentId)
{
	ui.setupUi(this);

	QSqlTableModel* model = new QSqlTableModel(this);

	model->setTable("students_info");
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);

	model->setFilter(QString("student_id = %1").arg(studentId));
	
	if (!(model->select()))
	{
		QMessageBox::critical(this, "Database Error", model->lastError().text());
		return;
	}

	ui.studentTableView->setModel(model);
	ui.studentTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.studentTableView->resizeRowsToContents();
}

editStudentWindow2::~editStudentWindow2()
{}

void editStudentWindow2::on_backToEditStudentWindow1_clicked()
{
	editStudentWindow* esw1 = new editStudentWindow();
	esw1->show();
	this->hide();
}

void editStudentWindow2::on_enterNewChange_clicked()
{
	CentralManagement centralManagement;

	QString studentName = ui.newStudentName->text();
	QString qStringStudentAge = ui.newStudentAge->text();
	int studentAge;
	QString studentCourse = ui.newStudentCourse->text();
	QString qStringStudentId = ui.newStudentId->text();
	int studentId;

	if (centralManagement.noAllEmptyLineEditForEditStudent2(studentName, qStringStudentAge, studentCourse, qStringStudentId))
	{
		QSqlQuery query;

		if (!(studentName.isEmpty()))
		{
			if (centralManagement.validStudentName(studentName))
			{
				query.prepare("update students_info set student_name = :studentName where student_id = :currentStudentId");
				query.bindValue(":studentName", studentName);
				query.bindValue(":currentStudentId", currentStudentId);
				if (!(query.exec()))
				{
					QMessageBox::critical(this, "Database Error", query.lastError().text());
				}
				ui.info->setText("Change name successfully");
				ui.newStudentName->clear();
			}
			else{ ui.info->setText("Invalid student name"); }
		}

		if (!(qStringStudentAge.isEmpty()))
		{
			if (centralManagement.validWholeNumber(qStringStudentAge, studentAge))
			{
				if (centralManagement.validAgeRange(studentAge))
				{
					query.prepare("update students_info set student_age = :studentAge where student_id = :currentStudentId");
					query.bindValue(":studentAge", studentAge);
					query.bindValue(":currentStudentId", currentStudentId);
					if (!(query.exec()))
					{
						QMessageBox::critical(this, "Database Error", query.lastError().text());
					}
					ui.info->setText("Change age successfully");
					ui.newStudentAge->clear();
				}
				else { ui.info->setText("Student age is outside the range"); }
			}
			else{ ui.info->setText("The student age is not valid positive whole number"); }
		}

		if (!(studentCourse.isEmpty()))
		{
			if (centralManagement.validStudentCourse(studentCourse))
			{
				query.prepare("update students_info set student_course = :studentCourse where student_id = :currentStudentId");
				query.bindValue(":studentCourse", studentCourse);
				query.bindValue(":currentStudentId", currentStudentId);
				if (!(query.exec()))
				{
					QMessageBox::critical(this, "Database Error", query.lastError().text());
				}
				ui.info->setText("Change course successfully");
				ui.newStudentCourse->clear();
			}
			else{ ui.info->setText("Invalid student course"); }
		}

		if (!(qStringStudentId.isEmpty()))
		{
			if (centralManagement.validWholeNumber(qStringStudentId, studentId))
			{
				if (centralManagement.validIdRange(studentId))
				{
					if (centralManagement.idIsNotRepeated(studentId, currentStudentId))
					{
						query.prepare("update students_info set student_id = :studentId where student_id = :currentStudentId");
						query.bindValue(":studentId", studentId);
						query.bindValue(":currentStudentId", currentStudentId);
						if (!(query.exec()))
						{
							QMessageBox::critical(this, "Database Error", query.lastError().text());
						}
						currentStudentId = studentId;
						ui.info->setText("Change id successfully");
						ui.newStudentId->clear();
					}
					else { ui.info->setText("Id is already existing"); }
				}
				else { ui.info->setText("student id is outside the range"); }
			}
			else { ui.info->setText("The student id is not valid positive whole number"); }
		}

		QSqlTableModel* model = new QSqlTableModel(this);

		model->setTable("students_info");
		model->setEditStrategy(QSqlTableModel::OnManualSubmit);

		model->setFilter(QString("student_id = %1").arg(currentStudentId));

		if (!(model->select()))
		{
			QMessageBox::critical(this, "Database Error", model->lastError().text());
			return;
		}

		ui.studentTableView->setModel(model);
		ui.studentTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui.studentTableView->resizeRowsToContents();
	}
	else { ui.info->setText("Fill in at least one blank field to make changes"); };
}

