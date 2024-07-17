#include "managepage.h"

#include <QDialog>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QStatusBar>
#include <QTableWidgetItem>
#include <QToolBar>
#include <QVBoxLayout>
#include "dialogs/adduserdialog.h"
#include "dialogs/deleteuserdialog.h"

ManagePage::ManagePage(UserSet *userSet, QWidget *parent)
	: QMainWindow(parent)
	, userSet(userSet)
{
	setAttribute(Qt::WA_DeleteOnClose);
	resize(600, 450);
	this->setWindowTitle("用户管理");
	QIcon icon1(":/icons/user_manage.png");
	this->setWindowIcon(icon1);
	QFont font1("宋体", 14);
	this->setFont(font1);
	QFont font2("Times New Roman", 14);
	this->setFont(font2);
	//创建列表
	listWidget                       = new QListWidget;
	QListWidgetItem *listWidgetItem1 = new QListWidgetItem;
	listWidgetItem1->setData(Qt::FontRole, QFont::Bold);
	listWidgetItem1->setText("序号	ID");

	// 创建一个QBrush对象，并设置颜色为红色
	// QBrush redBrush(Qt::black);

	// // 设置QListWidgetItem的文本颜色
	// listWidgetItem1->setForeground(redBrush);
	// 将第一个列表项添加到QListWidget中

	// 创建一个 QFont 对象，并设置字体大小
	// QFont font;
	// font.setPointSize(12); // 设置字体大小为12

	// 将 QFont 对象应用到 listWidgetItem1
	// listWidgetItem1->setFont(font);
	listWidget->addItem(listWidgetItem1);
	// 设置列表项样式
	// listWidget->setStyleSheet("QListWidget {"
	//                           "    background-color: #f0f0f0;" // 正确设置背景颜色
	//                           "    border-radius: 10px;"       // 正确添加圆角
	//                           "    border: 2px solid #c0c0c0;" // 正确添加边框
	//                           "}"
	//                           "QListWidget::item {"
	//                           "    padding: 10px;" // 设置内边距
	//                           "}"
	//                           "QListWidget::item:selected {"
	//                           "    background-color: #c0c0c0;" // 设置选中状态下的背景颜色
	//                           "}"
	//                           "QListWidget::item:hover {"
	//                           "    background-color: #e0e0e0;" // 设置鼠标悬停状态下的背景颜色
	//                           "}");

	for (const auto &i : userSet->getUsers())
	{
		// 创建表格项目，并添加到表格部件中
		//QListWidgetItem *listWidgetItem = new QListWidgetItem;
		if (!i.getPermission())
		{
			QString m = i.getId();
			// listWidget ->addItem(m);
			//listWidgetItem->setText(m);
			// QPixmap pixmap(500, 500);
			// pixmap.fill(Qt::green);
			// listWidgetItem->setIcon(QIcon(pixmap));
			QString itemText = QString("%1.	%2")
			                       .arg(counter, 2)
			                       .arg(m); // 使用arg()函数确保整齐对齐// 在文本前面加上序号
			QListWidgetItem *listWidgetItem = new QListWidgetItem(itemText);
			listWidget->addItem(listWidgetItem);
			counter++;
		}
	}

	// 创建状态栏
	QStatusBar *stBar = new QStatusBar();
	setStatusBar(stBar);

	// 创建标签并添加到状态栏
	QLabel *label = new QLabel("提示信息：不要轻易修改观察员信息");
	stBar->addWidget(label);

	//创建工具栏，可以有多个，默认不会放在窗口中
	QToolBar *toolBar1 = new QToolBar(this);
	toolBar1->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	toolBar1->setIconSize(QSize(32, 32));
	//addToolBar(toolBar);//可使工具栏放在窗口中，默认工具栏在菜单下方，即整体窗口的上方
	addToolBar(Qt::TopToolBarArea, toolBar1);
	QAction *addAction1 = toolBar1->addAction("增添人员");
	QIcon    icon(":/icons/user_add.png");
	addAction1->setIcon(icon);
	//toolBar1->addSeparator();
	QToolBar *toolBar2 = new QToolBar(this);
	toolBar2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	toolBar2->setIconSize(QSize(32, 32));
	addToolBar(Qt::TopToolBarArea, toolBar2);
	QAction *addAction2 = toolBar2->addAction("删除人员");
	QIcon    icon2(":/icons/user_delete.png");
	addAction2->setIcon(icon2);

	//toolBar3->addSeparator();
	// QToolBar * toolBar4 = new QToolBar(this);
	// addToolBar(Qt::TopToolBarArea,toolBar4);
	// QAction* addAction4 =toolBar4->addAction("查看人员信息");

	connect(addAction1, &QAction::triggered, this, &ManagePage::onAddUserTriggered);
	connect(addAction2, &QAction::triggered, this, &ManagePage::onDelteUserTriggered);

	toolBar1->setMovable(false);
	toolBar2->setMovable(false);

	// toolBar4 ->setMovable(false);

	// 创建垂直布局，并将表格部件和状态栏添加到布局中
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(listWidget);
	layout->addWidget(stBar);

	// 创建一个主部件，将布局设置为主部件的布局
	QWidget *centralWidget = new QWidget;
	centralWidget->setLayout(layout);

	// 设置主部件为主窗口的中心部件
	setCentralWidget(centralWidget);

	this->setFixedSize(400, 500);
}

void ManagePage::onAddUserTriggered()
{
	AddUserDialog addUser(userSet); // 创建addMan模态对话框
	if (addUser.exec() == QDialog::Accepted)
	{
	} else {
		listWidget->clear();
		counter                          = 1;
		QListWidgetItem *listWidgetItem1 = new QListWidgetItem;
		listWidgetItem1->setText("序号    ID");

		// 创建一个QBrush对象，并设置颜色为红色
		QBrush redBrush(Qt::black);

		// 设置QListWidgetItem的文本颜色
		listWidgetItem1->setForeground(redBrush);
		// 将第一个列表项添加到QListWidget中

		// 创建一个 QFont 对象，并设置字体大小
		QFont font;
		font.setPointSize(12); // 设置字体大小为12

		// 将 QFont 对象应用到 listWidgetItem1
		listWidgetItem1->setFont(font);
		listWidget->addItem(listWidgetItem1);
		for (const auto &i : userSet->getUsers())
		{
			// 创建表格项目，并添加到表格部件中
			//QListWidgetItem *listWidgetItem = new QListWidgetItem;
			if (!i.getPermission())
			{
				QString m = i.getId();
				// listWidget ->addItem(m);
				//listWidgetItem->setText(m);
				// QPixmap pixmap(500, 500);
				// pixmap.fill(Qt::green);
				// listWidgetItem->setIcon(QIcon(pixmap));
				QString itemText = QString("%1.        %2")
				                       .arg(counter, 2)
				                       .arg(m); // 使用arg()函数确保整齐对齐// 在文本前面加上序号
				QListWidgetItem *listWidgetItem = new QListWidgetItem(itemText);
				listWidget->addItem(listWidgetItem);
				counter++;
			}
		}
		this->show();
	}
}

void ManagePage::onDelteUserTriggered()
{
	// 创建 deleteMan 对话框对象
	DeleteUserDialog deleteUserDialog(
		userSet); // 假设 deleteMan 构造函数接受一个 QWidget* 类型的参数

	// 显示模态对话框并等待用户操作
	if (deleteUserDialog.exec() == QDialog::Accepted)
	{
	} else {
		listWidget->clear();
		counter                          = 1;
		QListWidgetItem *listWidgetItem1 = new QListWidgetItem;
		listWidgetItem1->setText("序号    ID");

		// 创建一个QBrush对象，并设置颜色为红色
		QBrush redBrush(Qt::black);

		// 设置QListWidgetItem的文本颜色
		listWidgetItem1->setForeground(redBrush);
		// 将第一个列表项添加到QListWidget中

		// 创建一个 QFont 对象，并设置字体大小
		QFont font;
		font.setPointSize(12); // 设置字体大小为12

		// 将 QFont 对象应用到 listWidgetItem1
		listWidgetItem1->setFont(font);
		listWidget->addItem(listWidgetItem1);
		for (const auto &i : userSet->getUsers())
		{
			// 创建表格项目，并添加到表格部件中
			//QListWidgetItem *listWidgetItem = new QListWidgetItem;
			if (!i.getPermission())
			{
				QString m = i.getId();
				// listWidget ->addItem(m);
				//listWidgetItem->setText(m);
				// QPixmap pixmap(500, 500);
				// pixmap.fill(Qt::green);
				// listWidgetItem->setIcon(QIcon(pixmap));
				QString itemText = QString("%1.        %2")
				                       .arg(counter, 2)
				                       .arg(m); // 使用arg()函数确保整齐对齐// 在文本前面加上序号
				QListWidgetItem *listWidgetItem = new QListWidgetItem(itemText);
				listWidget->addItem(listWidgetItem);
				counter++;
			}
		}
		this->show();
	}
}
