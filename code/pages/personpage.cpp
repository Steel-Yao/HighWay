#include "personpage.h"
#include "ui_personpage.h"

#include <QMenu>
#include <QMessageBox>
#include "dialogs/addpersondatadialog.h"
#include "dialogs/modifypersondatadialog.h"

PersonPage::PersonPage(DataSet<PersonData>* personData,
                       DeviceSet*           deviceSet,
                       PlaceSet*            placeSet,
                       QWidget*             parent)
	: QMainWindow(parent)
	, ui(new Ui::PersonPage)
	, personData(personData)
	, deviceSet(deviceSet)
	, placeSet(placeSet)
{
	ui->setupUi(this);
	setCentralWidget(ui->splitter);

	iniData();  // 初始化数据
	iniChart(); // 初始化图表
	updateData();
	updateChart();

	ui->dockWidget->setVisible(false);              // 筛选器初始不可见
	ui->groupBox_value_dateTime->setVisible(false); // 时间值设置初始不可见
	// 初始化下拉框数据
	ui->comboBox_value->addItems(personData->deviceIdList());
}

PersonPage::~PersonPage()
{
	delete ui;
}

// 设置所有QDockWidget的可见性
void PersonPage::setDockWidgetVisible(bool visible)
{
	if (ui->dockWidget->isFloating()) ui->dockWidget->setVisible(visible);
}

// 设置部分功能的可用性
void PersonPage::setFunctionsEnabled(bool enable)
{
	ui->actData_add->setEnabled(enable);
	ui->actData_delete->setEnabled(enable);
	ui->actData_modify->setEnabled(enable);
}

// 监测点或设备变化时更新显示
void PersonPage::do_dataChanged()
{
	on_pushButton_filter_clicked();
}

// 筛选器显示控制
void PersonPage::on_actData_filter_triggered(bool checked)
{
	ui->dockWidget->setVisible(checked);
}

// 更新筛选器action状态
void PersonPage::on_dockWidget_visibilityChanged(bool visible)
{
	ui->actData_filter->setChecked(visible);
}

// 初始化数据
void PersonPage::iniData()
{
	model     = new QStandardItemModel(0, 4, this);
	selection = new QItemSelectionModel(model, this);

	// 设置表头行
	QStringList header;
	header << "设备编号"
		   << "探测时间"
		   << "桩号"
		   << "IMEI号";
	model->setHorizontalHeaderLabels(header);

	ui->tableView->setModel(model);
	ui->tableView->setSelectionModel(selection);

	// 选择当前单元格变化时的信号与槽
	connect(selection, &QItemSelectionModel::currentChanged, this, &PersonPage::do_currentChanged);

	// 根据内容调整列宽
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->tableView->horizontalHeader()->setStretchLastSection(true);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

// 初始化图表
void PersonPage::iniChart()
{
	//界面刷新 解决重影
	ui->chartView->setViewportUpdateMode(QChartView::FullViewportUpdate);

	// 创建图表项
	chart = new QChart;
	chart->setTitleFont(QFont(chart->titleFont().family(), 14));
	chart->setTitle("人流随时间变化的曲线");
	chart->setAnimationOptions(QChart::AllAnimations);
	ui->chartView->setChart(chart);
	ui->chartView->setRenderHint(QPainter::Antialiasing);

	// 创建系列
	series = new QSplineSeries;
	series->setName("人流曲线");
	series->setUseOpenGL(true);
	series->setMarkerSize(4);
	series->setPointsVisible(true);
	series->setPointLabelsClipping(false);
	series->setPointLabelsFont(QFont("Times New Roman", 12));
	series->setPointLabelsFormat("@yPoint");
	series->setPointLabelsVisible(true);
	chart->addSeries(series);

	// 创建X轴
	axisX = new QDateTimeAxis;
	axisX->setLabelsFont(QFont("Times New Roman"));
	axisX->setTitleText("时间段");
	chart->addAxis(axisX, Qt::AlignBottom);
	series->attachAxis(axisX);

	// 创建Y轴
	axisY = new QValueAxis;
	axisY->setLabelFormat("%d"); // 设置标签格式为整数
	axisY->setLabelsFont(QFont("Times New Roman"));
	axisY->setTitleText("人流量");
	chart->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);
}

// 更新数据
void PersonPage::updateData(const QList<PersonData*>& list)
{
	int            row = 0;
	QStandardItem* item;

	if (list.isEmpty())                                   // 全数据
	{
		model->setRowCount(personData->getData().size()); // 设置数据行数

		for (const auto& i : personData->getData())
		{
			item = new QStandardItem(i.getDeviceId());
			model->setItem(row, 0, item);

			item = new QStandardItem(i.getDateTime().toString("yyyy/MM/dd hh:mm:ss"));
			model->setItem(row, 1, item);

			item = new QStandardItem(i.getPlaceId());
			model->setItem(row, 2, item);

			item = new QStandardItem(i.getIMEI());
			model->setItem(row, 3, item);

			row++;
		}
		ui->tableView->sortByColumn(1, Qt::DescendingOrder);
		return;
	}

	// 筛选的数据

	model->setRowCount(list.size()); // 设置数据行数

	for (const auto& i : list)
	{
		item = new QStandardItem(i->getDeviceId());
		model->setItem(row, 0, item);

		item = new QStandardItem(i->getDateTime().toString("yyyy/MM/dd hh:mm:ss"));
		model->setItem(row, 1, item);

		item = new QStandardItem(i->getPlaceId());
		model->setItem(row, 2, item);

		item = new QStandardItem(i->getIMEI());
		model->setItem(row, 3, item);

		row++;
	}
}

// 更新图表
void PersonPage::updateChart(const QList<PersonData*>& list)
{
	series->clear();

	// 确定数据最大和最小时间
	QDateTime minDateTime, maxDateTime;
	if (list.isEmpty())
	{
		minDateTime = personData->earliestDateTime();
		maxDateTime = personData->latestDateTime();
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

	QMap<QDateTime, int> data;     // 存储各时间段人流量
	QDateTime            curStart = minDateTime;
	while (curStart <= maxDateTime) // 创建时间段端点
	{
		QDateTime nextStart = curStart.addMSecs(ms);
		data[curStart]      = 0;
		curStart            = nextStart;
	}

	// 统计各时间段人数
	if (list.isEmpty())
	{
		for (const auto& i : personData->getData())
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
		series->append(i.key().toMSecsSinceEpoch() + ms / 2, i.value());
	}

	// 设置X轴
	axisX->setTickCount(series->count() + 1);
	axisX->setRange(minDateTime, curStart);
	axisX->setFormat(timeFormat);

	// 设置Y轴
	QList<int> counts   = data.values();
	int        maxCount = *std::max_element(counts.begin(), counts.end());
	axisY->setRange(0, maxCount * 1.25);
}

// 更新规则值候选列表
void PersonPage::on_comboBox_type_currentIndexChanged(int index)
{
	if (index == 1) // 探测时间
	{
		QDateTime earliest = personData->earliestDateTime(), latest = personData->latestDateTime();

		ui->startDateTime->setDateTimeRange(earliest, latest);
		ui->endDateTime->setDateTimeRange(earliest, latest);

		ui->startDateTime->setDateTime(earliest);
		ui->endDateTime->setDateTime(latest);

		ui->groupBox_value_dateTime->setVisible(true);
		ui->groupBox_value->setVisible(false);
	} else {
		QStringList list;
		ui->comboBox_value->clear();

		if (index == 0) // 设备编号
		{
			ui->comboBox_value->setEditable(true);
			list = personData->deviceIdList();
		} else if (index == 2) // 桩号
		{
			ui->comboBox_value->setEditable(false);
			list = personData->placeIdList();
		} else // IMEI号
		{
			ui->comboBox_value->setEditable(true);
			list = personData->IMEIList();
		}
		ui->comboBox_value->addItems(list);

		ui->groupBox_value->setVisible(true);
		ui->groupBox_value_dateTime->setVisible(false);
	}
}

// 根据当前选择更新规则类型和值
void PersonPage::do_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	Q_UNUSED(previous)
	if (!ui->dockWidget->isVisible()) return;

	if (current.isValid())
	{
		ui->comboBox_type->setCurrentIndex(current.column());
		ui->comboBox_value->setCurrentText(current.data().toString());
	}
}
void PersonPage::on_tableWidget_currentCellChanged(int currentRow,
                                                   int currentColumn,
                                                   int previousRow,
                                                   int previousColumn)
{
	Q_UNUSED(currentColumn)
	Q_UNUSED(previousRow)
	Q_UNUSED(previousColumn)

	if (currentRow == -1) return; // 删除的是最后一条规则

	QTableWidgetItem* item = ui->tableWidget->item(currentRow, 0);
	ui->comboBox_type->setCurrentText(item->text());

	item = ui->tableWidget->item(currentRow, 1);
	if (ui->comboBox_type->currentIndex() == 1) // 探测时间
	{
		ui->startDateTime->setDateTime(item->data(Qt::UserRole).toDateTime());
		ui->endDateTime->setDateTime(item->data(Qt::UserRole + 1).toDateTime());
	} else ui->comboBox_value->setCurrentText(item->text());
}

// 动态设置开始结束时间的范围
void PersonPage::on_startDateTime_editingFinished()
{
	ui->endDateTime->setMinimumDateTime(ui->startDateTime->dateTime());
}
void PersonPage::on_endDateTime_editingFinished()
{
	ui->startDateTime->setMaximumDateTime(ui->endDateTime->dateTime());
}

// 添加筛选规则
void PersonPage::on_pushButton_plus_clicked()
{
	int key = ui->comboBox_type->currentIndex();
	if (key == 1) // 探测时间
	{
		QString start = ui->startDateTime->dateTime().toString("yyyy/MM/dd hh:mm:ss");
		QString end   = ui->endDateTime->dateTime().toString("yyyy/MM/dd hh:mm:ss");
		auto    temp  = ui->tableWidget->findItems(start + " 至 " + end, Qt::MatchExactly);
		// 验证规则是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			ui->tableWidget->insertRow(0);
			QTableWidgetItem* item = new QTableWidgetItem(start + " 至 " + end);
			item->setData(Qt::UserRole, ui->startDateTime->dateTime());
			item->setData(Qt::UserRole + 1, ui->endDateTime->dateTime());
			ui->tableWidget->setItem(0, 1, item);
			item = new QTableWidgetItem("探测时间");
			ui->tableWidget->setItem(0, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "添加失败", "已添加相同的规则！");
	} else     // 设备编号、桩号、IMEI号
	{
		QString value = ui->comboBox_value->currentText().trimmed();
		auto    temp  = ui->tableWidget->findItems(value, Qt::MatchExactly);
		// 验证是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			ui->tableWidget->insertRow(0);
			QTableWidgetItem* item = new QTableWidgetItem(value);
			ui->tableWidget->setItem(0, 1, item);

			if (key == 0) // 设备编号
			{
				item = new QTableWidgetItem("设备编号");
				item->setData(Qt::UserRole, "deviceId");
			} else if (key == 2) // 桩号
			{
				item = new QTableWidgetItem("桩号");
				item->setData(Qt::UserRole, "placeId");
			} else // IMEI号
			{
				item = new QTableWidgetItem("IMEI号");
				item->setData(Qt::UserRole, "IMEI");
			}
			ui->tableWidget->setItem(0, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "添加失败", "已添加相同的规则！");
	}
	ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
}

// 删除筛选规则
void PersonPage::on_pushButton_minus_clicked()
{
	ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

// 编辑筛选规则
void PersonPage::on_pushButton_edit_clicked()
{
	int key = ui->comboBox_type->currentIndex(), currentRow = ui->tableWidget->currentRow();
	if (key == 1) // 探测时间
	{
		QString start = ui->startDateTime->dateTime().toString("yyyy/MM/dd hh:mm:ss");
		QString end   = ui->endDateTime->dateTime().toString("yyyy/MM/dd hh:mm:ss");
		auto    temp  = ui->tableWidget->findItems(start + " 至 " + end, Qt::MatchExactly);
		// 验证规则是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			QTableWidgetItem* item = new QTableWidgetItem(start + " 至 " + end);
			item->setData(Qt::UserRole, ui->startDateTime->dateTime());
			item->setData(Qt::UserRole + 1, ui->endDateTime->dateTime());
			ui->tableWidget->setItem(currentRow, 1, item);
			item = new QTableWidgetItem("探测时间");
			ui->tableWidget->setItem(currentRow, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "修改失败", "已存在相同的规则！");
	} else     // 设备编号、桩号、IMEI号
	{
		QString value = ui->comboBox_value->currentText().trimmed();
		auto    temp  = ui->tableWidget->findItems(value, Qt::MatchExactly);
		// 验证是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			QTableWidgetItem* item = new QTableWidgetItem(value);
			ui->tableWidget->setItem(currentRow, 1, item);

			if (key == 0) // 设备编号
			{
				item = new QTableWidgetItem("设备编号");
				item->setData(Qt::UserRole, "deviceId");
			} else if (key == 2) // 桩号
			{
				item = new QTableWidgetItem("桩号");
				item->setData(Qt::UserRole, "placeId");
			} else // IMEI号
			{
				item = new QTableWidgetItem("IMEI号");
				item->setData(Qt::UserRole, "IMEI");
			}
			ui->tableWidget->setItem(currentRow, 0, item);

		} else // 规则已添加过
			QMessageBox::critical(this, "修改失败", "已存在相同的规则！");
	}
	ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
}

// 清空筛选规则
void PersonPage::on_pushButton_clear_clicked()
{
	ui->tableWidget->setRowCount(0);
}

// 筛选按钮
void PersonPage::on_pushButton_filter_clicked()
{
	model->setRowCount(0);
	int rowCount = ui->tableWidget->rowCount();
	if (!rowCount)
	{
		updateData();
		updateChart();
	} else {
		Filter<PersonData> filter;
		for (int i = 0; i < rowCount; i++)
		{
			QTableWidgetItem *keyItem   = ui->tableWidget->item(i, 0),
							 *valueItem = ui->tableWidget->item(i, 1);
			if (keyItem->text() == "探测时间")
			{
				filter.addCriteria("startDateTime", valueItem->data(Qt::UserRole));
				filter.addCriteria("endDateTime", valueItem->data(Qt::UserRole + 1));
			} else {
				filter.addCriteria(keyItem->data(Qt::UserRole).toString(), valueItem->text());
			}
		}
		auto list = personData->filterData(filter);
		if (list.isEmpty())
		{
			QMessageBox::information(this, "提示", "没有符合要求的项！");
			return;
		}
		updateData(list);
		updateChart(list);
	}
	ui->tableView->sortByColumn(1, Qt::DescendingOrder);
}

// 新增数据
void PersonPage::on_actData_add_triggered()
{
	AddPersonDataDialog* addDataDialog = new AddPersonDataDialog(personData,
	                                                             deviceSet,
	                                                             placeSet->placeIdList(),
	                                                             this);
	if (addDataDialog->exec() == QDialog::Accepted) on_pushButton_filter_clicked();
}

// 删除数据
void PersonPage::on_actData_delete_triggered()
{
	QModelIndex current = selection->currentIndex();
	if (!current.isValid()) return;

	QString   IMEI     = model->index(current.row(), 3).data().toString();
	QDateTime dateTime = QDateTime::fromString(model->index(current.row(), 1).data().toString(),
	                                           "yyyy/MM/dd hh:mm:ss");
	auto      result   = QMessageBox::question(this,
                                        "确认删除",
                                        "您确定要删除该条数据吗？\n删除的数据无法恢复！");
	if (result == QMessageBox::Yes)
	{
		qDebug() << personData->delData(IMEI, dateTime);
		on_pushButton_filter_clicked();
	}
}

// 修改数据
void PersonPage::on_actData_modify_triggered()
{
	QModelIndex current = selection->currentIndex();
	if (!current.isValid()) return;

	QString   IMEI     = model->index(current.row(), 3).data().toString();
	QDateTime dateTime = QDateTime::fromString(model->index(current.row(), 1).data().toString(),
	                                           "yyyy/MM/dd hh:mm:ss");

	Filter<PersonData> filter;
	filter.addCriteria("IMEI", IMEI);
	filter.addCriteria("startDateTime", dateTime);
	filter.addCriteria("endDateTime", dateTime);
	auto        list = personData->filterData(filter);
	PersonData* data = list.first();

	ModifyPersonDataDialog* modifyDataDialog = new ModifyPersonDataDialog(data, personData, this);
	if (modifyDataDialog->exec() == QDialog::Accepted) on_pushButton_filter_clicked();
}

// 自定义右键菜单
void PersonPage::on_tableView_customContextMenuRequested(const QPoint& pos)
{
	Q_UNUSED(pos)
	QMenu* menuList = new QMenu(this);
	// menuList->addAction(ui->actData_add);
	// menuList->addAction(ui->actData_delete);
	// menuList->addAction(ui->actData_modify);
	// menuList->addSeparator();
	menuList->addAction(ui->actData_filter);
	menuList->exec(QCursor::pos());
	delete menuList;
}
