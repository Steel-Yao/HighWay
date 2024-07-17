#ifndef DEVICEPAGE_H
#define DEVICEPAGE_H

#include <QItemSelectionModel>
#include <QMainWindow>
#include <QStandardItemModel>
#include "structure/cardata.h"
#include "structure/dataset.h"
#include "structure/deviceset.h"
#include "structure/persondata.h"
#include "structure/placeset.h"

namespace Ui
{
class DevicePage;
}

class DevicePage : public QMainWindow
{
	Q_OBJECT

public:
	explicit DevicePage(DeviceSet*           deviceSet,
	                    PlaceSet*            placeSet,
	                    DataSet<PersonData>* personData,
	                    DataSet<CarData>*    carData,
	                    QWidget*             parent = nullptr);
	~DevicePage();
	void setDockWidgetVisible(bool visible); // 设置所有QDockWidget的可见性

signals:
	void deviceChanged();

public slots:
	void do_dataChanged(); // 数据更改时更新显示

private slots:
	void on_actData_filter_triggered(bool checked);       // 筛选器显示控制
	void on_dockWidget_visibilityChanged(bool visible);   // 更新筛选器action状态
	void on_comboBox_type_currentIndexChanged(int index); // 更新规则值候选列表

	void on_actDevice_add_triggered();                    // 新增设备
	void on_actDevice_delete_triggered();                 // 删除设备
	void on_actDevice_modify_triggered();                 // 修改设备

	// 根据当前选择更新规则类型和值
	void do_currentChanged(const QModelIndex& current, const QModelIndex& previous);
	void on_tableWidget_currentCellChanged(int currentRow,
	                                       int currentColumn,
	                                       int previousRow,
	                                       int previousColumn);

	void on_pushButton_plus_clicked();                               // 添加筛选规则
	void on_pushButton_minus_clicked();                              // 删除筛选规则
	void on_pushButton_edit_clicked();                               // 编辑筛选规则
	void on_pushButton_clear_clicked();                              // 清空筛选规则
	void on_pushButton_filter_clicked();                             // 筛选按钮

	void on_tableView_customContextMenuRequested(const QPoint& pos); // 自定义右键菜单

private:
	Ui::DevicePage* ui;

	DeviceSet*           deviceSet;
	PlaceSet*            placeSet;
	DataSet<PersonData>* personData;
	DataSet<CarData>*    carData;

	QStandardItemModel*  model;
	QItemSelectionModel* selection;

	void iniData();                                           // 初始化数据
	void updateData(QList<Device*> list = QList<Device*>(0)); // 更新数据
};

#endif // DEVICEPAGE_H
