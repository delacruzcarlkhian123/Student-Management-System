#include "stdafx.h"
#include "centralCode.h"

#include <fstream>
#include <sstream>
#include <string>
#include <QString>
#include <QStringList>
#include <QPasswordDigestor>
#include <QCryptographicHash>
#include <QByteArray>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

namespace
{
	constexpr int PasswordIterations = 6000;
	constexpr int SaltSize = 16;
	constexpr int HashSize = 32;

	QByteArray derivePasswordHash(const QString& password, const QByteArray& salt, int iterations)
	{
		return QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Sha256, password.toUtf8(), salt, iterations, HashSize);
	}

	QByteArray generateSalt()
	{
		QByteArray salt;
		salt.resize(SaltSize);

		for (int i = 0; i < SaltSize; i += 4)
		{
			quint32 value = QRandomGenerator::system()->generate();

			for (int j = 0; j < 4; ++j)
			{
				salt[i + j] = static_cast<char>((value >> (j * 8)) & 0xFF);
			}
		}
		return salt;
	}

	QString createPasswordRecord(const QString& password)
	{
		const QByteArray salt = generateSalt();
		
		const QByteArray hash = derivePasswordHash(password, salt, PasswordIterations);

		return QStringLiteral("%1$%2$%3").arg(QString::fromLatin1(salt.toHex())).arg(PasswordIterations).arg(QString::fromLatin1(hash.toHex()));
	}

	bool verifyPassword(const QString& password, const QString& storedRecord)
	{
		const QStringList parts = storedRecord.split('$');

		if (parts.size() != 3)
		{
			return false;
		}

		const QByteArray salt = QByteArray::fromHex(parts[0].toLatin1());

		bool ok = false;
		const int iterations = parts[1].toInt(&ok);

		const QByteArray storedHash = QByteArray::fromHex(parts[2].toLatin1());

		if (!ok || iterations < 1 || salt.size() != SaltSize || storedHash.size() != HashSize || parts[0].size() != SaltSize * 2 || parts[2].size() != HashSize * 2)
		{
			return false;
		}

		const QByteArray computedHash = derivePasswordHash(password, salt, iterations);

		if (computedHash.size() != storedHash.size())
		{
			return false;
		}

		unsigned char difference = 0;

		for (qsizetype i = 0; i < computedHash.size(); ++i)
		{
			difference |= static_cast<unsigned char>(computedHash[i]) ^ static_cast<unsigned char>(storedHash[i]);
		}

		return difference == 0;
	}
}

void CentralManagement::saveAccountToDatabase(QString& userName, QString passWord)
{
	QSqlQuery query;
	query.prepare("insert into accounts (username, password) values (:userName, :passWord)");
	query.bindValue(":userName", userName);
	query.bindValue(":passWord", passWord);
	if (!(query.exec()))
	{
		qDebug() << "SQL Error :" << query.lastError().text();
	}
}

bool CentralManagement::noEmptyLineEditForLogin(QString& userName, QString& passWord)
{
	if (!(userName.isEmpty() || passWord.isEmpty()))
	{
		return true;
	}
	return false;
}

bool CentralManagement::noEmptyLineEditForRegister(QString& userName, QString& passWord, QString& confirmPassWord)
{
	if (!(userName.isEmpty() || passWord.isEmpty() || confirmPassWord.isEmpty()))
	{
		return true;
	}
	return false;
}

bool CentralManagement::samePassWordAndConfirmPassWord(QString& userName, QString& passWord, QString& confirmPassWord)
{
	if (passWord != confirmPassWord)
	{
		return false;
	}

	const QString passwordRecord = createPasswordRecord(passWord);
	saveAccountToDatabase(userName, passwordRecord);
	return true;
}

bool CentralManagement::userNameIsNotRepeated(QString& userName)
{
	QSqlQuery query;
	query.prepare("select * from accounts where username = :userName");
	query.bindValue(":userName", userName);
	if (!(query.exec()))
	{
		qDebug() << "SQL Error :" << query.lastError().text();
		return false;
	}
	if (!(query.next()))
	{
		return true;
	}
	return false;
}

bool CentralManagement::validStudentName(QString &studentName)
{
	QRegularExpression patternForStudentName("^[a-zA-Z](?:[a-zA-Z]|[ '-.](?=[a-zA-Z])){0,58}[a-zA-Z]$");
	if (patternForStudentName.match(studentName).hasMatch())
	{
		return true;
	}
	return false;
}

bool CentralManagement::validWholeNumber(const QString &line, int &value)
{ 
	std::stringstream ss(line.toStdString());
	char extra;

	if (ss >> value && !(ss >> extra) && value >= 0)
	{
		return true;
	}
	return false;
}

bool CentralManagement::validAgeRange(int &studentAge)
{
	if (studentAge >= 18 && studentAge <= 100)
	{
		return true;
	}
	return false;
}

bool CentralManagement::validStudentCourse(QString &studentCourse)
{
	QRegularExpression patternForStudentCourse("^[a-zA-Z][a-zA-Z0-9]{1,59}$");
	if (patternForStudentCourse.match(studentCourse).hasMatch())
	{
		return true;
	}
	return false;
}

bool CentralManagement::validIdRange(int &studentId)
{
	if (studentId >= 100000 && studentId <= 999999)
	{
		return true;
	}
	return false;
}

bool CentralManagement::noEmptyLineEditForAddStudent(QString& studentName, QString& studentAge, QString& studentCourse, QString& studentId)
{
	if (!(studentName.isEmpty() || studentAge.isEmpty() || studentCourse.isEmpty() || studentId.isEmpty()))
	{
		return true;
	}
	return false;
}

bool CentralManagement::idIsNotRepeated(int &newStudentId, int &currentStudentId)
{
	QSqlQuery query;
	query.prepare("select * from students_info where student_id = :newStudentId and student_id <> :currentStudentId");
	query.bindValue(":newStudentId", newStudentId);
	query.bindValue(":currentStudentId", currentStudentId);
	if (!(query.exec()))
	{
		qDebug() << "SQL Error: " << query.lastError().text();
		return false;
	}
	if (!(query.next()))
	{
		return true;
	}
	return false;
}

bool CentralManagement::databaseIsNotEmpty()
{
	QSqlQuery query;
	query.prepare("select 1 from students_info limit 1");
	if (!(query.exec()))
	{
		qDebug() << "SQL Error: " << query.lastError().text();
		return false;
	}
	if (query.next())
	{
		return true;
	}
	return false;
}

bool CentralManagement::noEmptyLineEditForDeleteAndEditStudent(QString& studentId)
{
	if (!(studentId.isEmpty()))
	{
		return true;
	}
	return false;
}

bool CentralManagement::usernameExist(QString& username)
{
	QSqlQuery query;
	query.prepare("select * from accounts where username = :username");
	query.bindValue(":username", username);
	if (!(query.exec()))
	{
		qDebug() << "SQL Error: " << query.lastError().text();
		return false;
	}
	if (query.next())
	{
		return true;
	}
	return false;
}

bool CentralManagement::passwordExist(QString& username, QString& password)
{
	QSqlQuery query;
	query.prepare("select password from accounts where username = :username");
	query.bindValue(":username", username);
	if (!(query.exec()))
	{
		qDebug() << "SQL Error: " << query.lastError().text();
		return false;
	}
	if (query.next())
	{
		const QString storedRecord = query.value(0).toString();
		return verifyPassword(password, storedRecord);
	}
	return false;
}

bool CentralManagement::studentNameExist(QString &studentName)
{
	QSqlQuery query;
	query.prepare("select * from students_info where student_name = :studentName");
	query.bindValue(":studentName", studentName);
	if (!(query.exec()))
	{
		qDebug() << "SQL Error: " << query.lastError().text();
		return false;
	}
	if (query.next())
	{
		return true;
	}
	return false;
}

bool CentralManagement::studentAgeExist(int &studentAge)
{
	QSqlQuery query;
	query.prepare("select * from students_info where student_age = :studentAge");
	query.bindValue(":studentAge", studentAge);
	if (!(query.exec()))
	{
		qDebug() << "SQL Error: " << query.lastError().text();
		return false;
	}
	if (query.next())
	{
		return true;
	}
	return false;
}

bool CentralManagement::studentCourseExist(QString &studentCourse)
{
	QSqlQuery query;
	query.prepare("select * from students_info where student_course = :studentCourse");
	query.bindValue(":studentCourse", studentCourse);
	if (!(query.exec()))
	{
		qDebug() << "SQL Error: " << query.lastError().text();
		return false;
	}
	if (query.next())
	{
		return true;
	}
	return false;
}

bool CentralManagement::studentIdExist(int& studentId)
{
	QSqlQuery query;
	query.prepare("select * from students_info where student_id = :studentId");
	query.bindValue(":studentId", studentId);
	if (!(query.exec()))
	{
		qDebug() << "SQL Error :" << query.lastError().text();
		return false;
	}
	if (query.next())
	{
		return true;
	}
	return false;
}

bool CentralManagement::noAllEmptyLineEditForEditStudent2(QString& studentName, QString& studentAge, QString& studentCourse, QString& studentId)
{
	if (!(studentName.isEmpty() && studentAge.isEmpty() && studentCourse.isEmpty() && studentId.isEmpty()))
	{
		return true;
	}
	return false;
}

bool CentralManagement::noEmptyLineEditForSearchStudent(QString& searchStudent)
{
	if (!(searchStudent.isEmpty()))
	{
		return true;
	}
	return false;
}