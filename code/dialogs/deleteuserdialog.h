#ifndef DELETEUSERDIALOG_H
#define DELETEUSERDIALOG_H

#include <QDialog>
#include "structure/userset.h"

namespace Ui
{
class DeleteUserDialog;
}

class DeleteUserDialog : public QDialog
{
	Q_OBJECT

public:
	explicit DeleteUserDialog(UserSet* userSet, QWidget* parent = nullptr);
	~DeleteUserDialog();

private slots:
	void on_btn1_clicked();
	void on_btn2_clicked();

private:
	Ui::DeleteUserDialog* ui;
	UserSet*              userSet;
};

#endif // DELETEUSERDIALOG_H
