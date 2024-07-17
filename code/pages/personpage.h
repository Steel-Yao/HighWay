#ifndef PERSONPAGE_H
#define PERSONPAGE_H

#include <QItemSelectionModel>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QtCharts>
#include "structure/dataset.h"
#include "structure/deviceset.h"
#include "structure/persondata.h"
#include "structure/placeset.h"

namespace Ui
{
class PersonPage;
}

class PersonPage : public QMainWindow
{
	Q_OBJECT

public:
	explicit PersonPage(DataSet<PersonData>* personData,
	                    DeviceSet*           deviceSet,
	                    PlaceSet*            placeSet,
	                    QWidget*             parent = nullptr);
	~PersonPage();

	void setDockWidgetVisible(bool visible); // 设置所有QDockWidget的可见性
	void setFunctionsEnabled(bool enable);   // 设置部分功能的可用性

public slots:
	void do_dataChanged(); // 监测点或设备变化时更新显示

private slots:
	void on_actData_filter_triggered(bool checked);       // 筛选器显示控制
	void on_dockWidget_visibilityChanged(bool visible);   // 更新筛选器action状态
	void on_comboBox_type_currentIndexChanged(int index); // 更新规则值候选列表

	// 根据当前选择更新规则类型和值
	void do_currentChanged(const QModelIndex& current, const QModelIndex& previous);
	void on_tableWidget_currentCellChanged(int currentRow,
	                                       int currentColumn,
	                                       int previousRow,
	                                       int previousColumn);

	// 动态设置开始结束时间的范围
	void on_startDateTime_editingFinished();
	void on_endDateTime_editingFinished();

	void on_pushButton_plus_clicked();   // 添加筛选规则
	void on_pushButton_minus_clicked();  // 删除筛选规则
	void on_pushButton_edit_clicked();   // 编辑筛选规则
	void on_pushButton_clear_clicked();  // 清空筛选规则
	void on_pushButton_filter_clicked(); // 筛选按钮

	void on_actData_add_triggered();     // 新增数据
	void on_actData_delete_triggered();  // 删除数据
	void on_actData_modify_triggered();  // 修改数据

	// 自定义右键菜单
	void on_tableView_customContextMenuRequested(const QPoint& pos);

private:
	Ui::PersonPage* ui;

	DataSet<PersonData>* personData;
	DeviceSet*           deviceSet;
	PlaceSet*            placeSet;

	QStandardItemModel*  model;
	QItemSelectionModel* selection;

	QChart*        chart;
	QSplineSeries* series;
	QDateTimeAxis* axisX;
	QValueAxis*    axisY;

	void iniData();                                                           // 初始化数据
	void iniChart();                                                          // 初始化图表

	void updateData(const QList<PersonData*>& list = QList<PersonData*>(0));  // 更新数据
	void updateChart(const QList<PersonData*>& list = QList<PersonData*>(0)); // 更新图表
};

#endif // PERSONPAGE_H
