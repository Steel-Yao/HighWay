#include <QApplication>
#include "dataloader.h"
#include "mainwindow.h"
#include "pages/loginpage.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	// 创建数据类
	DataSet<PersonData> personData;
	DataSet<CarData>    carData;
	UserSet             userSet;
	DeviceSet           deviceSet(&personData, &carData);
	PlaceSet            plcaceSet(&deviceSet, &personData, &carData);
	DataSet<VideoData>  videoSet;

	// 创建登陆界面
	LoginPage* loginPage = new LoginPage;

	// 创建主界面
	MainWindow mainWindow(&userSet, &deviceSet, &plcaceSet, &personData, &carData, &videoSet);

	// 创建后台线程加载数据
	DataLoader* dataLoader = new DataLoader(&userSet,
	                                        &deviceSet,
	                                        &plcaceSet,
	                                        &personData,
	                                        &carData,
	                                        &videoSet,
	                                        &mainWindow);


	// 显示数据加载进度
	QObject::connect(dataLoader, &DataLoader::userLoaded, loginPage, &LoginPage::do_userLoaded);
	QObject::connect(dataLoader, &DataLoader::carLoaded, loginPage, &LoginPage::do_carLoaded);
	QObject::connect(dataLoader, &DataLoader::personLoaded, loginPage, &LoginPage::do_personLoaded);
	QObject::connect(dataLoader, &DataLoader::deviceLoaded, loginPage, &LoginPage::do_deviceLoaded);
	QObject::connect(dataLoader, &DataLoader::placeLoaded, loginPage, &LoginPage::do_placeLoaded);
	QObject::connect(dataLoader, &DataLoader::videoLoaded, loginPage, &LoginPage::do_videoLoaded);

	// 数据加载完成后创建界面
	QObject::connect(dataLoader, &DataLoader::userLoaded, &mainWindow, &MainWindow::do_userLoaded);
	QObject::connect(dataLoader, &DataLoader::carLoaded, &mainWindow, &MainWindow::do_carLoaded);
	QObject::connect(dataLoader,
	                 &DataLoader::personLoaded,
	                 &mainWindow,
	                 &MainWindow::do_personLoaded);
	QObject::connect(dataLoader,
	                 &DataLoader::deviceLoaded,
	                 &mainWindow,
	                 &MainWindow::do_deviceLoaded);
	QObject::connect(dataLoader, &DataLoader::placeLoaded, &mainWindow, &MainWindow::do_placeLoaded);
	QObject::connect(dataLoader, &DataLoader::dataLoaded, &mainWindow, &MainWindow::do_dataLoaded);

	// QObject::connect(dataLoader,
	//                  &DataLoader::dataLoaded,
	//                  dataLoader,
	//                  &DataLoader::deleteLater); // 数据加载完成后，后台线程类自动删除

	// 控制主窗口的显示
	QObject::connect(loginPage, &LoginPage::loginAccepted, [&]() {
		if (dataLoader->isRunning()) // 数据还未加载完成，等待数据加载完成后再显示主窗口
			QObject::connect(dataLoader, &DataLoader::dataLoaded, &mainWindow, &MainWindow::show);
		else mainWindow.show(); // 数据加载完成，直接进入主窗口
	});

	// 将登录用户传给主窗口
	QObject::connect(loginPage, &LoginPage::loginAccepted, &mainWindow, &MainWindow::do_loginUser);
	// QObject::connect(dataLoader, &DataLoader::dataLoaded, &mainWindow, &MainWindow::do_loginUser);

	dataLoader->start();                              // 开始加载数据

	int result = loginPage->exec();                   // 显示登陆界面
	if (result == QDialog::Accepted) return a.exec(); // 程序正常运行
	else
	{
		// 用户取消登录，终止程序
		dataLoader->quit(); // 确保后台线程停止
		dataLoader->wait(); // 等待线程完全退出
		return 0;
	}
}
