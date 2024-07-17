#ifndef CARPAGE_H
#define CARPAGE_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QtCharts>
#include "structure/cardata.h"
#include "structure/dataset.h"
#include "structure/videodata.h"

namespace Ui
{
class CarPage;
}

class CarPage : public QMainWindow
{
	Q_OBJECT

public:
	explicit CarPage(DataSet<CarData>*   carData,
	                 DataSet<VideoData>* videoData,
	                 QWidget*            parent = nullptr);
	~CarPage();
	void setDockWidgetVisible(bool visible); // 设置所有QDockWidget的可见性

public slots:
	void do_dataChanged(); // 监测点或设备数据改变时更新显示

private slots:
	void on_actCar_filter_triggered(bool checked);
	void on_dockWidget_visibilityChanged(bool visible);

	void on_pushButton_filter_clicked();
	void on_pushButton_clear_clicked();

	void on_tableWidget_2_currentCellChanged(int currentRow,
	                                         int currentColumn,
	                                         int previousRow,
	                                         int previousColumn);

	void on_pushButton_plus_clicked();
	void on_pushButton_minus_clicked();
	void on_pushButton_edit_clicked();

	void on_tableWidget_currentCellChanged(int currentRow,
	                                       int currentColumn,
	                                       int previousRow,
	                                       int previousColumn);
	void on_comboBox_type_currentTextChanged(const QString& arg1);

	void on_doubleSpinBox_editingFinished();
	void on_doubleSpinBox_2_editingFinished();
	void on_startDateTime_editingFinished();
	void on_endDateTime_editingFinished();

	void on_actCar_video_triggered();

	void do_hover(QPieSlice* slice, bool state);

	void on_tableWidget_customContextMenuRequested(const QPoint& pos);

	void on_tabWidget_customContextMenuRequested(const QPoint& pos);

	void on_actChart_carCount_triggered();

	void on_actChart_speed_triggered();

	void on_actChart_carId_triggered();

	void on_horizontalSlider_valueChanged(int value);

private:
	Ui::CarPage*        ui;
	DataSet<CarData>*   carData;
	DataSet<VideoData>* videoData;

	QChart*        countChart;
	QSplineSeries* countSeries;
	QDateTimeAxis* countAxisX;
	QValueAxis*    countAxisY;

	QChart*           speedChart;
	QBarCategoryAxis* speedAxisX;
	QValueAxis*       speedAxisY;
	QBarSeries*       speedSeries;

	QPieSlice* carIdSlice;
	QChart*    carIdChart;

	void iniData();
	void updateData(const QList<CarData*>& list = QList<CarData*>(0));
	void iniChart();
	void updateChart(const QList<CarData*>& list = QList<CarData*>(0));
	void iniBarChart();
	void updateBar(const QList<CarData*>& list = QList<CarData*>(0));
	void iniPeiChart();
	void updatePei(const QList<CarData*>& list = QList<CarData*>(0));
};

#endif // CARPAGE_H
