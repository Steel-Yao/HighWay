#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "structure/userset.h"

class AddUserDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddUserDialog(UserSet* userSet, QDialog* parent = nullptr);
	QLineEdit*   lineEdit;
	QLineEdit*   lineEdit1;
	QLineEdit*   lineEdit3;
	QPushButton* btn1;
	QPushButton* btn2;

private:
	UserSet* userSet;

signals:
	void userAdded();

public slots:
	void addUser(); // 添加用户的槽函数
	void ButtonClick();
	void BackMainWindow();
};

#endif // ADDUSERDIALOG_H
