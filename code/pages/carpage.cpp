#include "carpage.h"
#include "ui_carpage.h"

#include <QMessageBox>
#include "pages/videopage.h"
#include "structure/cardata.h"
#include "structure/dataset.h"

CarPage::CarPage(DataSet<CarData>* carData, DataSet<VideoData>* videoData, QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::CarPage)
	, carData(carData)
	, videoData(videoData)
{
	ui->setupUi(this);
	ui->groupBox_value_speed->setVisible(false);
	ui->groupBox_value_dateTime->setVisible(false);

	setCentralWidget(ui->splitter);
	ui->actCar_filter->setChecked(false);
	ui->dockWidget->setVisible(false);

	iniData();
	iniChart();
	iniBarChart();
	iniPeiChart();
	updateData();
	updateChart();
	updateBar();
	updatePei();

	ui->comboBox_value->addItems(carData->deviceIdList());
}
CarPage::~CarPage()
{
	delete ui;
}

void CarPage::setDockWidgetVisible(bool visible)
{
	if (ui->dockWidget->isFloating()) ui->dockWidget->setVisible(visible);
}

// 监测点或设备数据改变时更新显示
void CarPage::do_dataChanged()
{
	on_pushButton_filter_clicked();
}

void CarPage::on_actCar_filter_triggered(bool checked)
{
	ui->dockWidget->setVisible(checked);
}

void CarPage::on_dockWidget_visibilityChanged(bool visible)
{
	ui->actCar_filter->setChecked(visible);
}

void CarPage::on_pushButton_filter_clicked()
{
	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setSortingEnabled(false); // 取消排序
	int rowCount = ui->tableWidget_2->rowCount();
	if (!rowCount)
	{
		updateData();
		updateChart();
		updateBar();
		updatePei();
	} else {
		Filter<CarData> filter;
		for (int i = 0; i < rowCount; i++)
		{
			QTableWidgetItem *keyItem   = ui->tableWidget_2->item(i, 0),
							 *valueItem = ui->tableWidget_2->item(i, 1);
			if (keyItem->text() == "探测时间")
			{
				filter.addCriteria("startDateTime", valueItem->data(Qt::UserRole));
				filter.addCriteria("endDateTime", valueItem->data(Qt::UserRole + 1));
			} else if (keyItem->text() == "车速") {
				filter.addCriteria("minSpeed", valueItem->data(Qt::UserRole));
				filter.addCriteria("maxSpeed", valueItem->data(Qt::UserRole + 1));
			} else {
				filter.addCriteria(keyItem->data(Qt::UserRole).toString(), valueItem->text());
			}
		}
		auto list = carData->filterData(filter);
		if (list.isEmpty())
		{
			QMessageBox::information(this, "提示", "没有符合要求的项！");
			return;
		}
		updateData(list);
		updateChart(list);
		updateBar(list);
		updatePei(list);
	}
	ui->tableWidget->setSortingEnabled(true); // 恢复排序
	ui->tableWidget->sortByColumn(1, Qt::DescendingOrder);
}

void CarPage::iniData()
{
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //行头自适应表格
	ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);

	ui->tableWidget->setColumnCount(5);
	QStringList header;

	header << "设备编号"
		   << "时间"
		   << "桩号"
		   << "速度"
		   << "车牌号";
	ui->tableWidget->setHorizontalHeaderLabels(header);
}

// 更新数据
void CarPage::updateData(const QList<CarData*>& list)
{
	int               row = 0;
	QTableWidgetItem* item;

	if (list.isEmpty())
	{
		int num = carData->getData().size();
		ui->tableWidget->setRowCount(num);
		for (const auto& i : carData->getData())
		{
			item = new QTableWidgetItem(i.getDeviceId());
			ui->tableWidget->setItem(row, 0, item);

			item = new QTableWidgetItem(QDateTime(i.getDateTime()).toString("yyyy/MM/dd hh:mm:ss"));
			ui->tableWidget->setItem(row, 1, item);

			item = new QTableWidgetItem(i.getPlaceId());
			ui->tableWidget->setItem(row, 2, item);

			item = new QTableWidgetItem(QString("%1").arg(i.getSpeed() / 100.0));
			ui->tableWidget->setItem(row, 3, item);

			item = new QTableWidgetItem(i.getCarId());
			ui->tableWidget->setItem(row, 4, item);

			row++;
		}
		ui->tableWidget->sortByColumn(1, Qt::DescendingOrder);
		return;
	}

	ui->tableWidget->setRowCount(list.size());
	for (const auto& i : list)
	{
		item = new QTableWidgetItem(i->getDeviceId());
		ui->tableWidget->setItem(row, 0, item);

		item = new QTableWidgetItem(i->getDateTime().toString("yyyy/MM/dd hh:mm:ss"));
		ui->tableWidget->setItem(row, 1, item);

		item = new QTableWidgetItem(i->getPlaceId());
		ui->tableWidget->setItem(row, 2, item);

		item = new QTableWidgetItem(QString("%1").arg(i->getSpeed() / 100.0));
		ui->tableWidget->setItem(row, 3, item);

		item = new QTableWidgetItem(i->getCarId());
		ui->tableWidget->setItem(row, 4, item);

		row++;
	}
}

void CarPage::iniChart()
{
	//界面刷新 解决重影
	ui->chartView_carCount->setViewportUpdateMode(QChartView::FullViewportUpdate);

	// 创建图表项
	countChart = new QChart;
	countChart->setTitleFont(QFont(countChart->titleFont().family(), 14));
	countChart->setTitle("车流随时间变化的曲线");
	countChart->setAnimationOptions(QChart::AllAnimations);
	ui->chartView_carCount->setChart(countChart);
	ui->chartView_carCount->setRenderHint(QPainter::Antialiasing);

	// 创建系列
	countSeries = new QSplineSeries;
	countSeries->setName("车流曲线");
	countSeries->setUseOpenGL(true);
	countSeries->setMarkerSize(4);
	countSeries->setPointsVisible(true);
	countSeries->setPointLabelsClipping(false);
	countSeries->setPointLabelsFont(QFont("Times New Roman", 12));
	countSeries->setPointLabelsFormat("@yPoint");
	countSeries->setPointLabelsVisible(true);
	countChart->addSeries(countSeries);

	// 创建X轴
	countAxisX = new QDateTimeAxis;
	countAxisX->setLabelsFont(QFont("Times New Roman"));
	countAxisX->setTitleText("时间段");
	countChart->addAxis(countAxisX, Qt::AlignBottom);
	countSeries->attachAxis(countAxisX);

	// 创建Y轴
	countAxisY = new QValueAxis;
	countAxisY->setLabelFormat("%d"); // 设置标签格式为整数
	countAxisY->setLabelsFont(QFont("Times New Roman"));
	countAxisY->setTitleText("车流量");
	countChart->addAxis(countAxisY, Qt::AlignLeft);
	countSeries->attachAxis(countAxisY);
}

void CarPage::iniBarChart()
{
	ui->chartView_speed->setViewportUpdateMode(QChartView::FullViewportUpdate);

	// 创建图表项
	speedChart = new QChart();
	speedChart->setTitleFont(QFont(countChart->titleFont().family(), 14));
	speedChart->setTitle("不同速度区间的车辆数量");
	speedChart->setAnimationOptions(QChart::AllAnimations);
	ui->chartView_speed->setChart(speedChart);
	ui->chartView_speed->setRenderHint(QPainter::Antialiasing);

	speedSeries = new QBarSeries;
	speedSeries->setLabelsVisible(true);
	speedSeries->setLabelsPosition(QAbstractBarSeries::LabelsPosition::LabelsOutsideEnd);
	speedSeries->setUseOpenGL(true);
	speedChart->addSeries(speedSeries);

	speedAxisX = new QBarCategoryAxis;
	speedAxisX->setLabelsFont(QFont("Times New Roman"));
	speedAxisX->setTitleText("速度(km/h)");
	speedChart->addAxis(speedAxisX, Qt::AlignBottom);
	speedSeries->attachAxis(speedAxisX);

	speedAxisY = new QValueAxis;
	speedAxisY->setLabelFormat("%d"); // 设置标签格式为整数
	speedAxisY->setLabelsFont(QFont("Times New Roman"));
	speedAxisY->setTitleText("车流量");
	speedChart->addAxis(speedAxisY, Qt::AlignLeft);
	speedSeries->attachAxis(speedAxisY);
}

void CarPage::updateBar(const QList<CarData*>& list)
{
	int maxSpeed;
	if (list.isEmpty()) maxSpeed = carData->maxSpeed();
	else
	{
		maxSpeed = list[0]->getSpeed();
		for (const auto& i : list)
		{
			int entry = i->getSpeed();
			if (entry > maxSpeed) maxSpeed = entry;
		}
	}

	// 速度间隔
	int ms = (ui->horizontalSlider->sliderPosition() / 5 + 1) * 500;

	// 确定速度开端
	speedAxisX->clear();
	QMap<int, int> data;
	int            curSpeed = 0;
	while (curSpeed <= maxSpeed)
	{
		data[curSpeed] = 0;
		speedAxisX->append("[" + QString::number(curSpeed / 100) + ", "
		                   + QString::number((curSpeed + ms) / 100) + ")");
		curSpeed += ms;
	}

	// 统计各段速度车辆数
	if (list.isEmpty())
	{
		for (const auto& i : carData->getData())
		{
			int entry = i.getSpeed();
			for (auto i = data.begin(); i != data.end(); i++)
			{
				int start = i.key();
				if (entry >= start && entry < start + ms)
				{
					i.value()++;
					break;
				}
			}
		}
	} else {
		for (const auto& i : list)
		{
			int entry = i->getSpeed();
			for (auto i = data.begin(); i != data.end(); i++)
			{
				int start = i.key();
				if (entry >= start && entry < start + ms)
				{
					i.value()++;
					break;
				}
			}
		}
	}

	// 创建数据集
	QBarSet* speedSet = new QBarSet("车辆数");
	speedSet->setLabelColor(Qt::black);
	speedSet->setLabelFont(QFont("Times New Roman", 12));

	for (auto i = data.begin(); i != data.end(); i++)
	{
		speedSet->append(i.value());
	}

	// 将数据集加入系列
	speedSeries->clear();
	speedSeries->append(speedSet);

	// 设置Y轴
	QList<int> counts   = data.values();
	int        maxCount = *std::max_element(counts.begin(), counts.end());
	speedAxisY->setRange(0, maxCount);
	speedAxisY->applyNiceNumbers();
}

void CarPage::iniPeiChart()
{
	ui->chartView_carId->setViewportUpdateMode(QChartView::FullViewportUpdate);

	carIdSlice = new QPieSlice();

	carIdChart = new QChart();
	carIdChart->setTitle("车辆构成（基于车牌）");
	carIdChart->setTitleFont(QFont(countChart->titleFont().family(), 14));
	carIdChart->setAnimationOptions(QChart::AllAnimations);
	carIdChart->legend()->setAlignment(Qt::AlignRight);
	carIdChart->legend()->setFont(QFont("Times New Roman", 14));
	carIdChart->setAcceptHoverEvents(true);
	ui->chartView_carId->setChart(carIdChart);
	ui->chartView_carId->setRenderHint(QPainter::Antialiasing);
}

void CarPage::updatePei(const QList<CarData*>& list)
{
	int     CarIdNum[5] = {0};
	QString CarId[5]    = {"黄", "绿", "警(白)", "蓝", "其他或NO"};
	if (list.isEmpty())
	{
		for (const auto& it : carData->getData())
		{
			QString firChar = it.getCarId().at(0);
			if (firChar == "黄")
			{
				CarIdNum[0]++;
			} else if (firChar == "绿") {
				CarIdNum[1]++;
			} else if (firChar == "白") {
				CarIdNum[2]++;
			} else if (firChar == "蓝") {
				CarIdNum[3]++;
			} else CarIdNum[4]++;
		}
	} else {
		for (const auto i : list)
		{
			QString firChar = i->getCarId().at(0);
			if (firChar == "黄")
			{
				CarIdNum[0]++;
			} else if (firChar == "绿") {
				CarIdNum[1]++;
			} else if (firChar == "白") {
				CarIdNum[2]++;
			} else if (firChar == "蓝") {
				CarIdNum[3]++;
			} else CarIdNum[4]++;
		}
	}

	QPieSeries* carIdSeries = new QPieSeries;
	carIdSeries->setHoleSize(0.4); // 空心圆大小
	for (int i = 0; i < 5; i++)
	{
		carIdSeries->append(CarId[i], CarIdNum[i]);
	}
	carIdChart->removeAllSeries();
	carIdChart->addSeries(carIdSeries);

	for (int i = 0; i < 5; i++)
	{
		carIdSlice = carIdSeries->slices().at(i); //获取分块
		carIdSlice->setLabel(carIdSlice->label()
		                     + QString::asprintf(": %.0f 辆, %.1f%%",
		                                         carIdSlice->value(),
		                                         carIdSlice->percentage() * 100));
	}
	carIdSeries->setLabelsVisible(false);
	carIdSeries->setLabelsPosition(QPieSlice::LabelPosition::LabelOutside);
	connect(carIdSeries, &QPieSeries::hovered, this, &CarPage::do_hover);
}

void CarPage::updateChart(const QList<CarData*>& list)
{
	countSeries->clear();

	// 确定数据最大和最小时间
	QDateTime minDateTime, maxDateTime;
	if (list.isEmpty())
	{
		minDateTime = carData->earliestDateTime();
		maxDateTime = carData->latestDateTime();
	} else {
		minDateTime = list[0]->getDateTime(), maxDateTime = list[0]->getDateTime();
		for (const auto& i : list)
		{
			QDateTime entry = i->getDateTime();
			if (entry > maxDateTime) maxDateTime = entry;
			else if (entry < minDateTime) minDateTime = entry;
		}
	}

	// 动态确定时间间隔
	qint64  totalMs = minDateTime.msecsTo(maxDateTime);
	qint64  ms;
	QString timeFormat;

	if (totalMs < 3600 * 1000 * 2)         // 小于 2小时
	{
		ms         = 10 * 60 * 1000;       // 间隔 10分钟
		timeFormat = "H:mm";
	} else if (totalMs < 3600 * 1000 * 12) // 小于 12小时
	{
		ms         = 30 * 60 * 1000;       // 间隔 30分钟
		timeFormat = "H:mm";
	} else if (totalMs < 3600 * 1000 * 24) // 小于 1天
	{
		ms         = 3600 * 1000;          // 间隔 1小时
		timeFormat = "H:mm";
	} else if (totalMs < 6.048e8)          // 小于 7天
	{
		ms         = 3600 * 1000 * 12;     // 间隔 12小时
		timeFormat = "M/d H:mm";
	} else if (totalMs < 2.592e9)          // 小于 30天
	{
		ms         = 3600 * 1000 * 24;     // 间隔 1天
		timeFormat = "M/d";
	} else if (totalMs < 7.7776e9)         // 小于 90天
	{
		ms         = 3600 * 1000 * 24 * 7; // 间隔 7天
		timeFormat = "M/d";
	} else if (totalMs < 3.1536e10)        // 小于 1年
	{
		ms         = 2.592e9;              // 间隔 30天
		timeFormat = "M/d";
	} else if (totalMs < 1.5768e11)        // 小于 5年
	{
		ms         = 7.7776e9;             // 间隔 90天
		timeFormat = "yy/M/d";
	} else                                 // 大于 5年
	{
		ms         = 3.1536e10;            // 间隔 1年
		timeFormat = "yy/M";
	}

	QMap<QDateTime, int> data;      // 存储各时间段车流量
	QDateTime            curStart = minDateTime;
	while (curStart <= maxDateTime) // 创建时间段端点
	{
		QDateTime nextStart = curStart.addMSecs(ms);
		data[curStart]      = 0;
		curStart            = nextStart;
	}

	// 统计各时间段车流量
	if (list.isEmpty())
	{
		for (const auto& i : carData->getData())
		{
			QDateTime entry = i.getDateTime();
			for (auto i = data.begin(); i != data.end(); i++)
			{
				QDateTime start = i.key();
				if (entry >= start && entry < start.addMSecs(ms))
				{
					i.value()++;
					break;
				}
			}
		}
	} else {
		for (const auto& i : list)
		{
			QDateTime entry = i->getDateTime();
			for (auto i = data.begin(); i != data.end(); i++)
			{
				QDateTime start = i.key();
				if (entry >= start && entry < start.addMSecs(ms))
				{
					i.value()++;
					break;
				}
			}
		}
	}

	// 将数据加入系列
	for (auto i = data.begin(); i != data.end(); i++)
	{
		countSeries->append(i.key().toMSecsSinceEpoch() + ms / 2, i.value());
	}

	// 设置X轴
	countAxisX->setTickCount(countSeries->count() + 1);
	countAxisX->setRange(minDateTime, curStart);
	countAxisX->setFormat(timeFormat);

	// 设置Y轴
	QList<int> counts   = data.values();
	int        maxCount = *std::max_element(counts.begin(), counts.end());
	countAxisY->setRange(0, maxCount);
	countAxisY->applyNiceNumbers();
}

void CarPage::on_pushButton_clear_clicked()
{
	ui->tableWidget_2->setRowCount(0);
}

void CarPage::on_tableWidget_2_currentCellChanged(int currentRow,
                                                  int currentColumn,
                                                  int previousRow,
                                                  int previousColumn)
{
	Q_UNUSED(currentColumn)
	Q_UNUSED(previousRow)
	Q_UNUSED(previousColumn)

	if (currentRow == -1) return; // 删除的是最后一条规则

	QTableWidgetItem* item = ui->tableWidget_2->item(currentRow, 0);
	ui->comboBox_type->setCurrentText(item->text());

	item = ui->tableWidget_2->item(currentRow, 1);
	if (ui->comboBox_type->currentIndex() == 1) // 探测时间
	{
		ui->startDateTime->setDateTime(item->data(Qt::UserRole).toDateTime());
		ui->endDateTime->setDateTime(item->data(Qt::UserRole + 1).toDateTime());
	} else if (ui->comboBox_type->currentIndex() == 3) // 车速
	{
		double min = item->data(Qt::UserRole).toDouble() / 100,
			   max = item->data(Qt::UserRole + 1).toDouble() / 100;
		ui->doubleSpinBox->setValue(min);
		ui->doubleSpinBox_2->setValue(max);
	} else ui->comboBox_value->setCurrentText(item->text());
	item = nullptr;
}

void CarPage::on_pushButton_plus_clicked()
{
	int key = ui->comboBox_type->currentIndex();
	if (key == 1) // 探测时间
	{
		QString start = ui->startDateTime->dateTime().toString("yyyy/MM/dd hh:mm:ss");
		QString end   = ui->endDateTime->dateTime().toString("yyyy/MM/dd hh:mm:ss");
		auto    temp  = ui->tableWidget_2->findItems(start + " 至 " + end, Qt::MatchExactly);
		// 验证规则是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			ui->tableWidget_2->insertRow(0);
			QTableWidgetItem* item = new QTableWidgetItem(start + " 至 " + end);
			item->setData(Qt::UserRole, ui->startDateTime->dateTime());
			item->setData(Qt::UserRole + 1, ui->endDateTime->dateTime());
			ui->tableWidget_2->setItem(0, 1, item);
			item = new QTableWidgetItem("探测时间");
			ui->tableWidget_2->setItem(0, 0, item);
		} else           // 规则已添加过
			QMessageBox::critical(this, "添加失败", "已添加相同的规则！");
	} else if (key == 3) //车速
	{
		QString start = QString("%1").arg(ui->doubleSpinBox->value());
		QString end   = QString("%1").arg(ui->doubleSpinBox_2->value());
		auto    temp  = ui->tableWidget_2->findItems(start + " ~ " + end, Qt::MatchExactly);
		// 验证规则是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			ui->tableWidget_2->insertRow(0);
			QTableWidgetItem* item = new QTableWidgetItem(start + " ~ " + end);
			item->setData(Qt::UserRole, ui->doubleSpinBox->value() * 100);
			item->setData(Qt::UserRole + 1, ui->doubleSpinBox_2->value() * 100);
			ui->tableWidget_2->setItem(0, 1, item);
			item = new QTableWidgetItem("车速");
			ui->tableWidget_2->setItem(0, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "添加失败", "已添加相同的规则！");
	} else     // 设备编号、桩号、IMEI号
	{
		QString value = ui->comboBox_value->currentText().trimmed();
		auto    temp  = ui->tableWidget_2->findItems(value, Qt::MatchExactly);
		// 验证是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			ui->tableWidget_2->insertRow(0);
			QTableWidgetItem* item = new QTableWidgetItem(value);
			ui->tableWidget_2->setItem(0, 1, item);

			if (key == 0) // 设备编号
			{
				item = new QTableWidgetItem("设备编号");
				item->setData(Qt::UserRole, "deviceId");
			} else if (key == 2) // 桩号
			{
				item = new QTableWidgetItem("桩号");
				item->setData(Qt::UserRole, "placeId");
			} else {
				item = new QTableWidgetItem("车牌号");
				item->setData(Qt::UserRole, "carId");
			}
			ui->tableWidget_2->setItem(0, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "添加失败", "已添加相同的规则！");
	}
	ui->tableWidget_2->sortByColumn(0, Qt::AscendingOrder);
}
void CarPage::on_pushButton_minus_clicked()
{
	int currentRow = ui->tableWidget_2->currentRow();
	ui->tableWidget_2->removeRow(currentRow);
}

void CarPage::on_pushButton_edit_clicked()
{
	int key = ui->comboBox_type->currentIndex(), currentRow = ui->tableWidget_2->currentRow();
	if (key == 1) // 探测时间
	{
		QString start = ui->startDateTime->dateTime().toString("yyyy/MM/dd hh:mm:ss");
		QString end   = ui->endDateTime->dateTime().toString("yyyy/MM/dd hh:mm:ss");
		auto    temp  = ui->tableWidget_2->findItems(start + " 至 " + end, Qt::MatchExactly);
		// 验证规则是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			QTableWidgetItem* item = new QTableWidgetItem(start + " 至 " + end);
			item->setData(Qt::UserRole, ui->startDateTime->dateTime());
			item->setData(Qt::UserRole + 1, ui->endDateTime->dateTime());
			ui->tableWidget_2->setItem(currentRow, 1, item);
			item = new QTableWidgetItem("探测时间");
			ui->tableWidget_2->setItem(currentRow, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "修改失败", "已存在相同的规则！");
	} else if (key == 3) {
		QString start = QString("%1").arg(ui->doubleSpinBox->value());
		QString end   = QString("%1").arg(ui->doubleSpinBox_2->value());
		auto    temp  = ui->tableWidget_2->findItems(start + " ~ " + end, Qt::MatchExactly);
		// 验证规则是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			QTableWidgetItem* item = new QTableWidgetItem(start + " ~ " + end);
			item->setData(Qt::UserRole, ui->doubleSpinBox->value() * 100);
			item->setData(Qt::UserRole + 1, ui->doubleSpinBox_2->value() * 100);
			ui->tableWidget_2->setItem(0, 1, item);
			item = new QTableWidgetItem("车速");
			ui->tableWidget_2->setItem(0, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "添加失败", "已添加相同的规则！");
	} else     // 设备编号、桩号、车牌号
	{
		QString value = ui->comboBox_value->currentText().trimmed();
		auto    temp  = ui->tableWidget_2->findItems(value, Qt::MatchExactly);
		// 验证是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			QTableWidgetItem* item = new QTableWidgetItem(value);
			ui->tableWidget_2->setItem(currentRow, 1, item);

			if (key == 0) // 设备编号
			{
				item = new QTableWidgetItem("设备编号");
				item->setData(Qt::UserRole, "deviceId");
			} else if (key == 2) // 桩号
			{
				item = new QTableWidgetItem("桩号");
				item->setData(Qt::UserRole, "placeId");
			} else {
				item = new QTableWidgetItem("车牌号");
				item->setData(Qt::UserRole, "carId");
			}
			ui->tableWidget_2->setItem(currentRow, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "修改失败", "已存在相同的规则！");
	}
	ui->tableWidget_2->sortByColumn(0, Qt::AscendingOrder);
}

void CarPage::on_tableWidget_currentCellChanged(int currentRow,
                                                int currentColumn,
                                                int previousRow,
                                                int previousColumn)
{
	Q_UNUSED(previousRow)
	Q_UNUSED(previousColumn)
	if (currentRow == -1 || !ui->dockWidget->isVisible()) return;
	ui->comboBox_type->setCurrentIndex(currentColumn);
	ui->comboBox_value->setCurrentText(ui->tableWidget->item(currentRow, currentColumn)->text());
}

void CarPage::on_comboBox_type_currentTextChanged(const QString& arg1)
{
	if (arg1 == "探测时间")
	{
		QDateTime earliest = carData->earliestDateTime(), latest = carData->latestDateTime();

		ui->startDateTime->setDateTimeRange(earliest, latest);
		ui->endDateTime->setDateTimeRange(earliest, latest);

		ui->startDateTime->setDateTime(earliest);
		ui->endDateTime->setDateTime(latest);

		ui->groupBox_value_dateTime->setVisible(true);
		ui->groupBox_value->setVisible(false);
		ui->groupBox_value_speed->setVisible(false);
	} else if (arg1 == "车速") {
		double minSpeed = carData->minSpeed() / 100.0, maxSpeed = carData->maxSpeed() / 100.0;

		ui->doubleSpinBox->setRange(minSpeed, maxSpeed);
		ui->doubleSpinBox_2->setRange(minSpeed, maxSpeed);
		ui->doubleSpinBox->setValue(minSpeed);
		ui->doubleSpinBox_2->setValue(maxSpeed);

		ui->groupBox_value_dateTime->setVisible(false);
		ui->groupBox_value->setVisible(false);
		ui->groupBox_value_speed->setVisible(true);
	} else {
		QStringList list;
		ui->comboBox_value->clear();
		if (arg1 == "设备编号")
		{
			ui->comboBox_value->setEditable(true);
			list = carData->deviceIdList();
		} else if (arg1 == "桩号") {
			ui->comboBox_value->setEditable(false);
			list = carData->placeIdList();
		} else {
			ui->comboBox_value->setEditable(true);
			list = carData->carIdList();
		}
		ui->comboBox_value->addItems(list);

		ui->groupBox_value->setVisible(true);
		ui->groupBox_value_speed->setVisible(false);
		ui->groupBox_value_dateTime->setVisible(false);
	}
}

void CarPage::on_doubleSpinBox_editingFinished()
{
	ui->doubleSpinBox_2->setMinimum(ui->doubleSpinBox->value());
}

void CarPage::on_doubleSpinBox_2_editingFinished()
{
	ui->doubleSpinBox->setMaximum(ui->doubleSpinBox_2->value());
}

void CarPage::on_startDateTime_editingFinished()
{
	ui->endDateTime->setMinimumDateTime(ui->startDateTime->dateTime());
}

void CarPage::on_endDateTime_editingFinished()
{
	ui->startDateTime->setMaximumDateTime(ui->endDateTime->dateTime());
}

void CarPage::on_actCar_video_triggered()
{
	VideoPage* videoPage = new VideoPage(videoData, this);
	videoPage->show();
}

void CarPage::do_hover(QPieSlice* slice, bool state)
{
	slice->setExploded(state);
	slice->setLabelVisible(state);
}

void CarPage::on_tableWidget_customContextMenuRequested(const QPoint& pos)
{
	Q_UNUSED(pos)
	QMenu* menuList = new QMenu(this);
	menuList->addAction(ui->actCar_video);
	menuList->addSeparator();
	menuList->addAction(ui->actCar_filter);
	menuList->exec(QCursor::pos());
	delete menuList;
}

void CarPage::on_tabWidget_customContextMenuRequested(const QPoint& pos)
{
	Q_UNUSED(pos)
	QMenu* menuList = new QMenu(this);
	menuList->addAction(ui->actChart_carCount);
	menuList->addAction(ui->actChart_speed);
	menuList->addAction(ui->actChart_carId);
	menuList->exec(QCursor::pos());
	delete menuList;
}

void CarPage::on_actChart_carCount_triggered()
{
	ui->tabWidget->setCurrentIndex(0);
}

void CarPage::on_actChart_speed_triggered()
{
	ui->tabWidget->setCurrentIndex(1);
}

void CarPage::on_actChart_carId_triggered()
{
	ui->tabWidget->setCurrentIndex(2);
}

void CarPage::on_horizontalSlider_valueChanged(int value)
{
	ui->label_speed->setText("速度间隔：" + QString::number((value / 5 + 1) * 5) + " km/h");
}
