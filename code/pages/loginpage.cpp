#include "loginpage.h"
#include "ui_loginpage.h"

#include <QMessageBox>
#include "filter.h"

LoginPage::LoginPage(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::LoginPage)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::SplashScreen);

	userSet.loadUsers();
}

LoginPage::~LoginPage()
{
	delete ui;
}

QString LoginPage::getCurrentUser() const
{
	return currentUser;
}

//鼠标按键被按下
void LoginPage::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		moving  = true;
		lastPos = event->globalPosition().toPoint() - this->pos();
	}
	return QDialog::mousePressEvent(event);
}

//鼠标按下左键移动
void LoginPage::mouseMoveEvent(QMouseEvent* event)
{
	QPoint eventPos = event->globalPosition().toPoint();

	if (moving && (event->buttons() & Qt::LeftButton)
	    && (eventPos - lastPos).manhattanLength() > QApplication::startDragDistance())
	{
		move(eventPos - lastPos);
		lastPos = eventPos - this->pos();
	}
	return QDialog::mouseMoveEvent(event);
}

//鼠标按键被释放
void LoginPage::mouseReleaseEvent(QMouseEvent* event)
{
	moving = false; //停止移动
	event->accept();
}

void LoginPage::do_userLoaded(bool ok)
{
	ui->label_loadState->setText("加载车流数据和页面……");
	if (!ok)
		QMessageBox::warning(this,
		                     "数据载入警告",
		                     "载入用户数据时可能发生了错误！\n请检查数据文件！");
}

void LoginPage::do_videoLoaded(bool ok)
{
	ui->label_loadState->setText("数据加载完成！");
	if (!ok)
		QMessageBox::warning(this,
		                     "数据载入警告",
		                     "载入视频数据时可能发生了错误！\n请检查数据文件！");
}

void LoginPage::do_carLoaded(bool ok)
{
	ui->label_loadState->setText("加载人流数据和页面……");
	if (!ok)
		QMessageBox::warning(this,
		                     "数据载入警告",
		                     "载入车流数据时可能发生了错误！\n请检查数据文件！");
}

void LoginPage::do_personLoaded(bool ok)
{
	ui->label_loadState->setText("加载设备数据和页面……");
	if (!ok)
		QMessageBox::warning(this,
		                     "数据载入警告",
		                     "载入人流数据时可能发生了错误！\n请检查数据文件！");
}

void LoginPage::do_deviceLoaded(bool ok)
{
	ui->label_loadState->setText("加载监测点数据和页面……");
	if (!ok)
		QMessageBox::warning(this,
		                     "数据载入警告",
		                     "载入设备数据时可能发生了错误！\n请检查数据文件！");
}

void LoginPage::do_placeLoaded(bool ok)
{
	ui->label_loadState->setText("加载视频数据和页面……");
	if (!ok)
		QMessageBox::warning(this,
		                     "数据载入警告",
		                     "载入监测点数据时可能发生了错误！\n请检查数据文件！");
}

// “登录”按钮
void LoginPage::on_pushButton_ok_clicked()
{
	QString id, password;
	id       = ui->lineEdit_id->text().trimmed();
	password = ui->lineEdit_password->text().trimmed();

	if (id.isEmpty())
	{
		QMessageBox::information(this, "提示", "账号为空！");
		return;
	}

	if (password.isEmpty())
	{
		QMessageBox::information(this, "提示", "密码为空！");
		return;
	}

	Filter<User> filter;
	filter.addCriteria("id", id);
	auto list = userSet.filterUser(filter);

	if (list.empty())
		QMessageBox::information(this, "用户不存在", "找不到用户：" + id + "！\n请检查！");
	else if (password == list[0]->getPassword())
	{
		QString loginMessage = "欢迎！\n";
		if (list[0]->getPermission()) loginMessage += "管理员：" + id;
		else loginMessage += "观察员：" + id;

		QMessageBox::information(this, "登陆成功", loginMessage);
		emit loginAccepted(id);
		accept();
	} else QMessageBox::critical(this, "密码错误", "您提供的密码与对应用户的密码不匹配！请检查！");
}
