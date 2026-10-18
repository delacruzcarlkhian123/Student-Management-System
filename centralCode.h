#pragma once

class CentralManagement
{
public:
	void saveAccountToDatabase(QString &userName, QString passWord);
	bool usernameExist(QString& username);
	bool passwordExist(QString& username, QString& password);
	bool noEmptyLineEditForRegister(QString& userName, QString& passWord, QString& confirmPassWord);
	bool noEmptyLineEditForLogin(QString& userName, QString& passWord);
	bool samePassWordAndConfirmPassWord(QString& userName, QString& passWord, QString& confirmPassWord);
	bool userNameIsNotRepeated(QString& userName);

	bool validStudentName(QString& studentName);
	bool validWholeNumber(const QString& line, int& value);
	bool validAgeRange(int& studentAge);
	bool validStudentCourse(QString& studentCourse);
	bool validIdRange(int& studentId);
	bool idIsNotRepeated(int& newStudentId, int& currentStudentId);
	bool databaseIsNotEmpty();
	bool noEmptyLineEditForAddStudent(QString &studentName, QString& studentAge, QString& studentCourse, QString& studentId);
	bool noAllEmptyLineEditForEditStudent2(QString& studentName, QString& studentAge, QString& studentCourse, QString& studentId);
	bool studentNameExist(QString &studentName);
	bool studentAgeExist(int& studentAge);
	bool studentCourseExist(QString& studentCourse);
	bool studentIdExist(int &studentId);
	bool noEmptyLineEditForDeleteAndEditStudent(QString& studentId);
	bool noEmptyLineEditForSearchStudent(QString& searchStudent);

};