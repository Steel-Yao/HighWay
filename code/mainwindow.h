#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include "pages/carpage.h"
#include "pages/devicepage.h"
#include "pages/homepage.h"
#include "pages/personpage.h"
#include "pages/placepage.h"
#include "pages/videopage.h"
#include "structure/cardata.h"
#include "structure/dataset.h"
#include "structure/deviceset.h"
#include "structure/persondata.h"
#include "structure/placeset.h"
#include "structure/userset.h"
#include "structure/videodata.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(UserSet*             userSet,
	           DeviceSet*           deviceSet,
	           PlaceSet*            placeSet,
	           DataSet<PersonData>* personData,
	           DataSet<CarData>*    carData,
	           DataSet<VideoData>*  videoSet,
	           QWidget*             parent = nullptr);
	~MainWindow();

public slots:
	void do_userLoaded();
	void do_carLoaded();
	void do_personLoaded();
	void do_deviceLoaded();
	void do_placeLoaded();
	void do_dataLoaded();

	void do_loginUser(QString id); // 传入登录用户

	void do_timer_timeout();       // 计时结束

private slots:
	void on_tabWidget_tabCloseRequested(int index);     // 关闭页面
	void on_tabWidget_currentChanged(int index);        // 设置tabWidget的可见性

	void on_actPage_home_triggered();                   // 显示主页面
	void on_actPage_device_triggered();                 // 显示设备页面
	void on_actPage_place_triggered();                  // 显示监测点页面
	void on_actPage_carData_triggered();                // 显示车流页面
	void on_actPage_personData_triggered();             // 显示人流页面

	void on_actUser_current_triggered();                // 菜单栏“当前用户”
	void on_actUser_manage_triggered();                 // 菜单栏“账户管理”

	void on_actMenu_home_triggered(bool checked);       // 菜单栏“首页”
	void on_actMenu_device_triggered(bool checked);     // 菜单栏“检测设备”
	void on_actMenu_place_triggered(bool checked);      // 菜单栏“监测点”
	void on_actMenu_carData_triggered(bool checked);    // 菜单栏“车流数据”
	void on_actMenu_personData_triggered(bool checked); // 菜单栏“人流数据”

	void on_actAbout_qt_triggered();                    // 关于Qt
	void on_actAbout_app_triggered();                   // 关于本软件

	// 自定义右键菜单
	void on_MainWindow_customContextMenuRequested(const QPoint& pos);

protected:
	void closeEvent(QCloseEvent* event) override; // 退出确认

private:
	Ui::MainWindow* ui;

	QTimer* timer;                    // 计时器
	QLabel* label_curTime;            // 状态栏当前时间

	HomePage*   homePage;             // 首页
	DevicePage* devicePage;           // 设备页面
	PlacePage*  placePage;            // 监测点页面
	CarPage*    carPage;              // 车流页面
	PersonPage* personPage;           // 人流页面
	VideoPage*  videoPage;            // 监控视频页面

	QString currentId;                // 当前登录用户Id
	int     lastIndex;                // 切换界面前的索引

	User*                currentUser; // 当前登录用户
	UserSet*             userSet;
	DeviceSet*           deviceSet;
	PlaceSet*            placeSet;
	DataSet<PersonData>* personData;
	DataSet<CarData>*    carData;
	DataSet<VideoData>*  videoSet;
};
#endif // MAINWINDOW_H
