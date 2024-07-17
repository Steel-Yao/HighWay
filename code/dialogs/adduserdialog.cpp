#include "adduserdialog.h"

#include <QIcon>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

AddUserDialog::AddUserDialog(UserSet *userSet, QDialog *parent)
	: QDialog(parent)
	, userSet(userSet)

{
	// 初始化界面和信号/槽连接
	this->resize(300, 400);
	this->setFixedSize(400, 500);
	this->setWindowTitle("添加观察员");
	QIcon icon(":/icons/user_add.png");
	this->setWindowIcon(icon);

	QFont font1("宋体", 14);
	this->setFont(font1);
	QFont font2("Times New Roman", 14);
	this->setFont(font2);

	// 创建背景图片的QLabel
	QLabel *backgroundLabel = new QLabel(this);
	QPixmap backgroundPixmap(":/imag/AddUserDialog_background.png");
	backgroundLabel->setPixmap(backgroundPixmap); // 设置背景图片
	backgroundLabel->setScaledContents(true);     // 根据标签大小缩放图片

	// 创建输入框
	QLabel *label  = new QLabel("                    请在此添加观察员信息", this);
	QLabel *label2 = new QLabel("账号：", this);
	lineEdit       = new QLineEdit(this);
	lineEdit->setText("");
	QLabel *label1 = new QLabel("密码：", this);
	lineEdit1      = new QLineEdit(this);
	lineEdit1->setEchoMode(QLineEdit::Password);
	lineEdit1->setText("");
	QLabel *label3 = new QLabel("确认密码：", this);
	lineEdit3      = new QLineEdit(this);
	lineEdit3->setEchoMode(QLineEdit::Password);
	lineEdit3->setText("");

	btn1 = new QPushButton(this);
	btn1->setText("确认");
	btn2 = new QPushButton(this);
	btn2->setText("取消");

	// 创建一个垂直布局
	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	// 创建三个水平布局
	QHBoxLayout *layout1 = new QHBoxLayout();
	QHBoxLayout *layout2 = new QHBoxLayout();
	QHBoxLayout *layout3 = new QHBoxLayout();
	QHBoxLayout *layout4 = new QHBoxLayout();
	QHBoxLayout *layout5 = new QHBoxLayout();
	// 将控件添加到对应的水平布局中
	layout1->addWidget(label);

	layout2->addWidget(label2);
	layout2->addWidget(lineEdit);

	layout3->addWidget(label1);
	layout3->addWidget(lineEdit1);

	layout4->addWidget(label3);
	layout4->addWidget(lineEdit3);

	layout5->addWidget(btn1);
	layout5->addWidget(btn2);

	// 将水平布局添加到垂直布局中
	mainLayout->addLayout(layout1);
	mainLayout->addLayout(layout2);
	mainLayout->addLayout(layout3);
	mainLayout->addLayout(layout4);
	mainLayout->addLayout(layout5);
	mainLayout->addWidget(backgroundLabel); // 将背景标签添加到垂直布局中

	// 设置布局
	setLayout(mainLayout);

	connect(btn1, &QPushButton::clicked, this, &AddUserDialog::ButtonClick);
	connect(btn2, &QPushButton::clicked, this, &AddUserDialog::BackMainWindow);
}

void AddUserDialog::addUser()
{
	emit userAdded(); // 发送信号通知用户已添加
}

void AddUserDialog::ButtonClick()
{
	QString content  = lineEdit->text();
	QString content1 = lineEdit1->text();
	QString content3 = lineEdit3->text();

	if (content1 != content3)
	{
		QMessageBox::critical(this, "提示", "两次密码不同，请重新输入");
	} else if ((content1.isEmpty() && content3.isEmpty())) {
		QMessageBox::critical(this, "提示", "密码不能为空");
	} else {
		//存放观察员信息
		int a = userSet->addUser(content, content1, 0);
		if (!a)
		{
			QMessageBox::critical(this, "提示", "账户重名");
		} else {
			QMessageBox::information(this, "提示", "添加成功");
			lineEdit->clear();
			lineEdit1->clear();
			lineEdit3->clear();
			this->hide();
		}
	}
}

void AddUserDialog::BackMainWindow()
{
	this->hide();
}
