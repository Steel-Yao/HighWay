#include "currentuserpage.h"
#include "ui_currentuserpage.h"

#include <QInputDialog>
#include <QMessageBox>

CurrentUserPage::CurrentUserPage(User* currentUser, UserSet* userSet, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::CurrentUserPage)
	, currentUser(currentUser)
	, userSet(userSet)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);

	ui->lineEdit_id->setText(currentUser->getId());
	ui->lineEdit_password->setText(currentUser->getPassword());
	if (currentUser->getPermission()) ui->label_permission->setText("管理员");
	else ui->label_permission->setText("观察员");
}

CurrentUserPage::~CurrentUserPage()
{
	delete ui;
}

void CurrentUserPage::on_pushButton_modify_clicked()
{
	if (!ui->lineEdit_id->isEnabled()) // 判断是否已经验证身份
	{                                  // 未验证身份
		bool    ok       = false;
		QString password = QInputDialog::getText(this,
		                                         "身份验证",
		                                         "请输入" + currentUser->getId()
		                                             + "的密码以验证您的身份",
		                                         QLineEdit::Password,
		                                         "",
		                                         &ok); // 密码验证
		if (ok)
		{
			if (password.trimmed() == currentUser->getPassword())
			{
				QMessageBox::information(this, "验证通过", "现在您可以修改您的用户信息了！");
				ui->lineEdit_id->setEnabled(true);
				ui->lineEdit_password->setEnabled(true);
			} else QMessageBox::critical(this, "验证失败", "密码与对应的用户不匹配！");
		}
	} else { // 已验证身份
		QString newId       = ui->lineEdit_id->text().trimmed(),
				newPassword = ui->lineEdit_password->text().trimmed();

		for (const auto& i : userSet->getUsers()) // 判断重名
		{
			if (i.getId() != currentUser->getId() && i.getId() == newId)
			{
				QMessageBox::critical(this, "修改失败", "您提供的用户名已被占用！");
				return;
			}
		}

		QMessageBox::StandardButton result;
		result = QMessageBox::question(this,
		                               "确认修改",
		                               "您确认要修改用户信息吗？\n请牢记修改后的信息！");

		if (result == QMessageBox::Yes)
		{
			currentUser->setId(newId);
			currentUser->setPassword(newPassword);
		}
	}
}
