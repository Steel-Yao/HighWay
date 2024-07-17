#include "deleteuserdialog.h"
#include "ui_deleteuserdialog.h"

#include <QMessageBox>
#include <QPixmap>

DeleteUserDialog::DeleteUserDialog(UserSet* userSet, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::DeleteUserDialog)
	, userSet(userSet)
{
	ui->setupUi(this);
	this->setFixedSize(400, 500);
	this->setWindowTitle("删除观察员");
	QIcon icon(":/icons/user_delete.png");
	this->setWindowIcon(icon);
	// 创建Label并设置背景图片
	QLabel* backgroundLabel = new QLabel(this);
	QPixmap backgroundPixmap(":/imag/DeleteUserDialog_background.png");
	backgroundLabel->setPixmap(backgroundPixmap);
	// 设置Label的大小和位置，将背景图片放在窗口下半部分
	int halfHeight = this->height() / 2;
	backgroundLabel->setGeometry(0, halfHeight / 1.4, this->width(), halfHeight * 1.4);
	backgroundLabel->setScaledContents(true);

	QStringList list = userSet->idList();
	list.removeFirst();
	ui->comboBox->addItems(list);
}

DeleteUserDialog::~DeleteUserDialog()
{
	delete ui;
}

void DeleteUserDialog::on_btn1_clicked()
{
	QString content1 = ui->comboBox->currentText();
	int     a        = userSet->delUser(content1);
	if (!a)
	{
		// 只有当删除失败时，才显示这个消息
		QMessageBox::critical(this, "提示", "删除失败，请检查");
	}
	if (a)
	{
		// 当删除成功时，显示这个消息并清空输入框
		QMessageBox::information(this, "提示", "删除成功");
		ui->comboBox->clear();
		this->hide();
	}
}

void DeleteUserDialog::on_btn2_clicked()
{
	this->hide();
}
