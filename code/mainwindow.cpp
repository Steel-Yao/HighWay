#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QIcon>
#include <QList>
#include <QMenu>
#include <QMessageBox>
#include "pages/currentuserpage.h"
#include "pages/managepage.h"

MainWindow::MainWindow(UserSet*             userSet,
                       DeviceSet*           deviceSet,
                       PlaceSet*            placeSet,
                       DataSet<PersonData>* personData,
                       DataSet<CarData>*    carData,
                       DataSet<VideoData>*  videoSet,
                       QWidget*             parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, lastIndex(-1)
	, userSet(userSet)
	, deviceSet(deviceSet)
	, placeSet(placeSet)
	, personData(personData)
	, carData(carData)
	, videoSet(videoSet)
{
	ui->setupUi(this);

	setCentralWidget(ui->tabWidget);

	label_curTime = new QLabel(this);
	ui->statusBar->addPermanentWidget(label_curTime);

	timer = new QTimer(this);
	timer->stop();
	timer->setTimerType(Qt::CoarseTimer);
	timer->setInterval(1000);
	timer->setSingleShot(false);
	connect(timer, &QTimer::timeout, this, &MainWindow::do_timer_timeout);
	timer->start();
}

MainWindow::~MainWindow()
{
	// 将数据写入文件
	userSet->saveUsers();
	deviceSet->saveDevices();
	placeSet->savePlaces();
	personData->saveData();
	carData->saveData();

	delete ui;
}

void MainWindow::do_userLoaded() {}

// 车流页面
void MainWindow::do_carLoaded()
{
	carPage = new CarPage(carData, videoSet, this);
}

// 人流页面
void MainWindow::do_personLoaded()
{
	personPage = new PersonPage(personData, deviceSet, placeSet, this);
}

// 设备页面
void MainWindow::do_deviceLoaded()
{
	devicePage = new DevicePage(deviceSet, placeSet, personData, carData, this);
}

// 监测点页面
void MainWindow::do_placeLoaded()
{
	placePage = new PlacePage(placeSet, deviceSet, personData, carData, this);
}

// 首页-数据加载完成
void MainWindow::do_dataLoaded()
{
	// 数据变化时更新显示的信号与槽连接
	connect(placePage, &PlacePage::placeChanged, devicePage, &DevicePage::do_dataChanged);
	connect(placePage, &PlacePage::placeChanged, personPage, &PersonPage::do_dataChanged);
	connect(placePage, &PlacePage::placeChanged, carPage, &CarPage::do_dataChanged);
	connect(devicePage, &DevicePage::deviceChanged, personPage, &PersonPage::do_dataChanged);
	connect(devicePage, &DevicePage::deviceChanged, carPage, &CarPage::do_dataChanged);

	// 首页
	homePage = new HomePage(carData, personData, this);
	QIcon icon;
	icon.addFile(":/icons/home.png"); // 图标
	ui->tabWidget->addTab(homePage, icon, "首页");
	connect(timer, &QTimer::timeout, homePage, &HomePage::do_timeout);
}

// 传入登录用户
void MainWindow::do_loginUser(QString id)
{
	if (userSet->getUsers().size() != 0)
	{
		Filter<User> filter;
		filter.addCriteria("id", id);
		auto list   = userSet->filterUser(filter);
		currentUser = list[0];
	}
	// 根据登录用户权限设置部分功能可用性
	ui->actUser_manage->setEnabled(currentUser->getPermission());
}

// 计时结束
void MainWindow::do_timer_timeout()
{
	label_curTime->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
}

// 关闭页面
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
	QWidget* page = ui->tabWidget->widget(index);

	// 移除对应页面
	if (index < 0) return;
	ui->tabWidget->removeTab(index);

	// 更新菜单栏复选状态
	if (page == homePage) ui->actMenu_home->setChecked(false);
	else if (page == placePage) ui->actMenu_place->setChecked(false);
	else if (page == devicePage)
	{
		devicePage->setDockWidgetVisible(false);
		ui->actMenu_device->setChecked(false);
	} else if (page == carPage) {
		carPage->setDockWidgetVisible(false);
		ui->actMenu_carData->setChecked(false);
	} else {
		personPage->setDockWidgetVisible(false);
		ui->actMenu_personData->setChecked(false);
	}
}

// 设置tabWidget和dockWidget的可见性
void MainWindow::on_tabWidget_currentChanged(int index)
{
	bool visible = ui->tabWidget->count() > 0; // 是否还有页面
	ui->tabWidget->setVisible(visible);

	if (lastIndex != -1)
	{
		QWidget* last = ui->tabWidget->widget(lastIndex);
		if (last == carPage) carPage->setDockWidgetVisible(false);
		else if (last == personPage) personPage->setDockWidgetVisible(false);
		else if (last == devicePage) devicePage->setDockWidgetVisible(false);
	}
	lastIndex = index; // 更新索引
}

// 显示主页面
void MainWindow::on_actPage_home_triggered()
{
	// 设置图标
	QIcon icon;
	icon.addFile(":/icons/home.png");

	ui->tabWidget->insertTab(0, homePage, icon, "首页"); // 添加到最前方
	ui->tabWidget->setCurrentIndex(0);                   // 显示主页面

	ui->actMenu_home->setChecked(true);                  // 更新菜单栏复选状态
}

// 显示设备页面
void MainWindow::on_actPage_device_triggered()
{
	// 判断是否已添加
	int index = ui->tabWidget->indexOf(devicePage);
	if (index == -1) // 如果未添加，则添加上
	{
		QIcon icon;  // 设置图标
		icon.addFile(":/icons/device.png");
		index = ui->tabWidget->addTab(devicePage, icon, "监测设备");
	}
	ui->tabWidget->setCurrentIndex(index); // 显示设备界面

	ui->actMenu_device->setChecked(true);  // 更新菜单栏复选状态
}

// 显示监测点页面
void MainWindow::on_actPage_place_triggered()
{
	// 判断是否已添加
	int index = ui->tabWidget->indexOf(placePage);
	if (index == -1) // 如果未添加，则添加上
	{
		QIcon icon;  // 设置图标
		icon.addFile(":/icons/place.png");
		index = ui->tabWidget->addTab(placePage, icon, "监测点");
	}
	ui->tabWidget->setCurrentIndex(index); // 显示监测点界面

	ui->actMenu_place->setChecked(true);   // 更新菜单栏复选状态
}

// 显示车流页面
void MainWindow::on_actPage_carData_triggered()
{
	// 判断是否已添加
	int index = ui->tabWidget->indexOf(carPage);
	if (index == -1) // 如果未添加，则添加上
	{
		QIcon icon;  // 设置图标
		icon.addFile(":/icons/car.png");
		index = ui->tabWidget->addTab(carPage, icon, "车流数据");
	}
	ui->tabWidget->setCurrentIndex(index); // 显示车流界面

	ui->actMenu_carData->setChecked(true); // 更新菜单栏复选状态
}

// 显示人流页面
void MainWindow::on_actPage_personData_triggered()
{
	// 判断是否已添加
	int index = ui->tabWidget->indexOf(personPage);
	if (index == -1) // 如果未添加，则添加上
	{
		// 根据登陆用户的权限设置功能的可用性
		personPage->setFunctionsEnabled(currentUser->getPermission());

		QIcon icon;
		icon.addFile(":/icons/person.png"); // 设置图标
		index = ui->tabWidget->addTab(personPage, icon, "人流数据");
	}
	ui->tabWidget->setCurrentIndex(index);    // 显示人流界面

	ui->actMenu_personData->setChecked(true); // 更新菜单栏复选状态
}

// 菜单栏“当前用户”
void MainWindow::on_actUser_current_triggered()
{
	CurrentUserPage* currentUserPage = new CurrentUserPage(currentUser, userSet);
	currentUserPage->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
	currentUserPage->exec();
}

// 菜单栏“用户管理”
void MainWindow::on_actUser_manage_triggered()
{
	ManagePage* managePage = new ManagePage(userSet, this);
	managePage->show();
}

// 退出确认
void MainWindow::closeEvent(QCloseEvent* event)
{
	QMessageBox::StandardButton result;
	result = QMessageBox::question(this, "确认退出", "您确定要退出本程序吗？\n所有数据将会被保存。");
	if (result == QMessageBox::Yes) event->accept(); //退出
	else event->ignore();                            //不退出
}

// 菜单栏“首页”
void MainWindow::on_actMenu_home_triggered(bool checked)
{
	if (checked) on_actPage_home_triggered();
	else
	{
		int index = ui->tabWidget->indexOf(homePage);
		ui->tabWidget->removeTab(index);
	}
}

// 菜单栏“检测设备”
void MainWindow::on_actMenu_device_triggered(bool checked)
{
	if (checked) on_actPage_device_triggered();
	else
	{
		int index = ui->tabWidget->indexOf(devicePage);
		ui->tabWidget->removeTab(index);
		devicePage->setDockWidgetVisible(false);
	}
}

// 菜单栏“监测点”
void MainWindow::on_actMenu_place_triggered(bool checked)
{
	if (checked) on_actPage_place_triggered();
	else
	{
		int index = ui->tabWidget->indexOf(placePage);
		ui->tabWidget->removeTab(index);
	}
}

// 菜单栏“车流数据”
void MainWindow::on_actMenu_carData_triggered(bool checked)
{
	if (checked) on_actPage_carData_triggered();
	else
	{
		int index = ui->tabWidget->indexOf(carPage);
		ui->tabWidget->removeTab(index);
		carPage->setDockWidgetVisible(false);
	}
}

// 菜单栏“人流数据”
void MainWindow::on_actMenu_personData_triggered(bool checked)
{
	if (checked) on_actPage_personData_triggered();
	else
	{
		int index = ui->tabWidget->indexOf(personPage);
		ui->tabWidget->removeTab(index);
		personPage->setDockWidgetVisible(false);
	}
}

// 关于Qt
void MainWindow::on_actAbout_qt_triggered()
{
	QMessageBox::aboutQt(this, "关于Qt");
}

// 关于本软件
void MainWindow::on_actAbout_app_triggered()
{
	QMessageBox::about(this, "关于本软件", "武易高速道路运输监测数据展示系统\nBy 猴猿三结义");
}

// 自定义右键菜单
void MainWindow::on_MainWindow_customContextMenuRequested(const QPoint& pos)
{
	Q_UNUSED(pos)
	QMenu* menu = new QMenu(this);
	menu->addAction(ui->actMenu_home);
	menu->addSeparator();
	menu->addAction(ui->actMenu_place);
	menu->addAction(ui->actMenu_device);
	menu->addSeparator();
	menu->addAction(ui->actMenu_carData);
	menu->addAction(ui->actMenu_personData);
	menu->exec(QCursor::pos());
	delete menu;
}
