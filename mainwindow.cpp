#include "stdafx.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addstudentwindow.h"
#include "deletestudentwindow.h"
#include "editstudentwindow.h"
#include "searchstudentwindow.h"
#include "centralCode.h"

#include <QSqlTableModel>
#include <QTableView>
#include <QSqlError>
#include <QHeaderView>
#include <QMessageBox>

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QSqlTableModel* model = new QSqlTableModel(this);

    model->setTable("students_info");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    
    if (!(model->select()))
    {
        QMessageBox::critical(this, "Database Error", model->lastError().text());
        return;
    }

    ui.studentTableView->setModel(model);
    ui.studentTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.studentTableView->resizeRowsToContents();
}

mainWindow::~mainWindow()
{}

void mainWindow::on_addStudent_clicked()
{
    addStudentWindow* asw = new addStudentWindow();
    asw->show();
    this->hide();
}

void mainWindow::on_deleteStudent_clicked()
{
    CentralManagement centralManagement;

    if (centralManagement.databaseIsNotEmpty())
    {
        deleteStudentWindow* dsw = new deleteStudentWindow();
        dsw->show();
        this->hide();
    }
    else { ui.extraInfo->setText("Cannot delete student, there is no student yet, add student first"); }
}

void mainWindow::on_editStudent_clicked()
{
    CentralManagement centralManagement;

    if (centralManagement.databaseIsNotEmpty())
    {
        editStudentWindow* esw = new editStudentWindow();
        esw->show();
        this->hide();
    }
    else { ui.extraInfo->setText("Cannot edit student, there is no student yet, add student first"); }
}

void mainWindow::on_searchStudent_clicked()
{
    CentralManagement centralManagement;

    if (centralManagement.databaseIsNotEmpty())
    {
        searchStudentWindow* ssw = new searchStudentWindow();
        ssw->show();
        this->hide();
    }
    else { ui.extraInfo->setText("Cannot search student, there is no student yet, add student first"); }
}

