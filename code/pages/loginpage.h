#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
#include "structure/userset.h"

namespace Ui
{
class LoginPage;
}

class LoginPage : public QDialog
{
	Q_OBJECT

public:
	explicit LoginPage(QWidget* parent = nullptr);
	~LoginPage();

	QString getCurrentUser() const;

signals:
	void loginAccepted(QString id);

private:
	Ui::LoginPage* ui;

	bool   moving = false; // 界面是否在移动
	QPoint lastPos;        // 鼠标最后位置

	UserSet userSet;
	QString currentUser; // 成功登陆的用户id

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

public slots:
	void do_carLoaded(bool ok);
	void do_personLoaded(bool ok);
	void do_deviceLoaded(bool ok);
	void do_placeLoaded(bool ok);
	void do_userLoaded(bool ok);
	void do_videoLoaded(bool ok);

private slots:
	void on_pushButton_ok_clicked(); // “登录”按钮
};

#endif // LOGINPAGE_H
