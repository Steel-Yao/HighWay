#include "devicepage.h"
#include "ui_devicepage.h"

#include <QMenu>
#include <QMessageBox>
#include "dialogs/adddevicedialog.h"
#include "dialogs/modifydevicedialog.h"

DevicePage::DevicePage(DeviceSet*           deviceSet,
                       PlaceSet*            placeSet,
                       DataSet<PersonData>* personData,
                       DataSet<CarData>*    carData,
                       QWidget*             parent)
	: QMainWindow(parent)
	, ui(new Ui::DevicePage)
	, deviceSet(deviceSet)
	, placeSet(placeSet)
	, personData(personData)
	, carData(carData)
{
	ui->setupUi(this);
	setCentralWidget(ui->tableView);

	ui->dockWidget->setVisible(false); // 筛选器初始不可见

	iniData();                         // 初始化数据
	updateData();
	// 初始化下拉框数据
	ui->comboBox_value->addItems(deviceSet->deviceIdList());
}

DevicePage::~DevicePage()
{
	delete ui;
}

// 设置所有QDockWidget的可见性
void DevicePage::setDockWidgetVisible(bool visible)
{
	if (ui->dockWidget->isFloating()) ui->dockWidget->setVisible(visible);
}

// 监测点变化时更新显示
void DevicePage::do_dataChanged()
{
	on_pushButton_filter_clicked();
}

// 筛选器显示控制
void DevicePage::on_actData_filter_triggered(bool checked)
{
	ui->dockWidget->setVisible(checked);
}

// 更新筛选器action状态
void DevicePage::on_dockWidget_visibilityChanged(bool visible)
{
	ui->actData_filter->setChecked(visible);
}

// 更新规则值候选列表
void DevicePage::on_comboBox_type_currentIndexChanged(int index)
{
	QStringList list;
	ui->comboBox_value->clear();

	if (index == 0) // 设备编号
	{
		ui->comboBox_value->setEditable(true);
		list = deviceSet->deviceIdList();
	} else if (index == 2) // 桩号
	{
		ui->comboBox_value->setEditable(false);
		list = deviceSet->placeIdList();
	} else // 设备类型
	{
		ui->comboBox_value->setEditable(false);
		list << "监控探头"
			 << "4G探针";
	}
	ui->comboBox_value->addItems(list);
}

// 新增设备
void DevicePage::on_actDevice_add_triggered()
{
	AddDeviceDialog* addDeviceDialog = new AddDeviceDialog(deviceSet, placeSet->placeIdList(), this);
	if (addDeviceDialog->exec() == QDialog::Accepted) on_pushButton_filter_clicked();
}

// 删除设备
void DevicePage::on_actDevice_delete_triggered()
{
	QModelIndex current = selection->currentIndex();
	if (current.isValid())
	{
		QString deviceId = model->index(current.row(), 0).data().toString();
		auto    result   = QMessageBox::question(this,
                                            "确认删除",
                                            "您确定要删除编号为 " + deviceId + " 的设备吗？");
		if (result == QMessageBox::Yes)
		{
			// 删除设备，刷新列表
			deviceSet->delDevice(deviceId);
			on_pushButton_filter_clicked();
			emit deviceChanged();
		}
	}
}

// 修改设备
void DevicePage::on_actDevice_modify_triggered()
{
	QModelIndex current = selection->currentIndex();
	if (!current.isValid()) return;

	QString        deviceId = model->index(current.row(), 0).data().toString();
	Filter<Device> filter;
	filter.addCriteria("deviceId", deviceId);
	auto    list   = deviceSet->filterDevice(filter);
	Device* device = list[0];

	QStringList deviceIdList = deviceSet->deviceIdList();
	deviceIdList.removeOne(deviceId);

	ModifyDeviceDialog* modifyDeviceDialog = new ModifyDeviceDialog(device,
	                                                                placeSet->placeIdList(),
	                                                                deviceIdList,
	                                                                this);
	if (modifyDeviceDialog->exec() == QDialog::Accepted)
	{
		QString newDeviceId = modifyDeviceDialog->getNewDeviceId(),
				newPlaceId  = modifyDeviceDialog->getNewPlaceId();
		// 更新对应数据的设备编号
		if (device->getDeviceClass() == "监控探头" && device->getDeviceId() != newDeviceId)
		{
			Filter<CarData> filter;
			filter.addCriteria("deviceId", deviceId);
			auto list = carData->filterData(filter);
			for (auto& i : list) i->setDeviceId(newDeviceId);

			emit deviceChanged();
		} else if (device->getDeviceId() != newDeviceId) // 4G探针
		{
			Filter<PersonData> filter;
			filter.addCriteria("deviceId", deviceId);
			auto list = personData->filterData(filter);
			for (auto& i : list) i->setDeviceId(newDeviceId);

			emit deviceChanged();
		}
		device->setDeviceId(newDeviceId);
		device->setPlaceId(newPlaceId);
		on_pushButton_filter_clicked();
	}
	delete modifyDeviceDialog;
}

// 根据当前选择更新规则类型和值
void DevicePage::do_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	Q_UNUSED(previous)
	if (!ui->dockWidget->isVisible()) return;

	if (current.isValid())
	{
		ui->comboBox_type->setCurrentIndex(current.column());
		ui->comboBox_value->setCurrentText(current.data().toString());
	}
}
void DevicePage::on_tableWidget_currentCellChanged(int currentRow,
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
	ui->comboBox_value->setCurrentText(item->text());
}

// 添加筛选规则
void DevicePage::on_pushButton_plus_clicked()
{
	int     key   = ui->comboBox_type->currentIndex();
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
		} else if (key == 1) // 设备类型
		{
			item = new QTableWidgetItem("设备类型");
			item->setData(Qt::UserRole, "deviceClass");
		} else // 桩号
		{
			item = new QTableWidgetItem("桩号");
			item->setData(Qt::UserRole, "placeId");
		}
		ui->tableWidget->setItem(0, 0, item);
	} else // 规则已添加过
		QMessageBox::critical(this, "添加失败", "已添加相同的规则！");
	ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
}

// 删除筛选规则
void DevicePage::on_pushButton_minus_clicked()
{
	ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

// 编辑筛选规则
void DevicePage::on_pushButton_edit_clicked()
{
	int     key = ui->comboBox_type->currentIndex(), currentRow = ui->tableWidget->currentRow();
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
		} else if (key == 1) // 设备类型
		{
			item = new QTableWidgetItem("设备类型");
			item->setData(Qt::UserRole, "deviceClass");
		} else // 桩号
		{
			item = new QTableWidgetItem("桩号");
			item->setData(Qt::UserRole, "placeId");
		}
		ui->tableWidget->setItem(currentRow, 0, item);

	} else // 规则已添加过
		QMessageBox::critical(this, "修改失败", "已存在相同的规则！");

	ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
}

// 清空筛选规则
void DevicePage::on_pushButton_clear_clicked()
{
	ui->tableWidget->setRowCount(0);
}

// 筛选按钮
void DevicePage::on_pushButton_filter_clicked()
{
	model->setRowCount(0);
	int rowCount = ui->tableWidget->rowCount();
	if (!rowCount) updateData();
	else
	{
		Filter<Device> filter;
		for (int i = 0; i < rowCount; i++)
		{
			QTableWidgetItem *keyItem   = ui->tableWidget->item(i, 0),
							 *valueItem = ui->tableWidget->item(i, 1);
			filter.addCriteria(keyItem->data(Qt::UserRole).toString(), valueItem->text());
		}
		auto list = deviceSet->filterDevice(filter);
		if (list.isEmpty())
		{
			QMessageBox::information(this, "提示", "没有符合要求的项！");
			return;
		}
		updateData(list);
	}
	ui->tableView->sortByColumn(0, Qt::AscendingOrder);
}

// 初始化数据
void DevicePage::iniData()
{
	model     = new QStandardItemModel(0, 3, this);
	selection = new QItemSelectionModel(model, this);

	ui->tableView->setModel(model);
	ui->tableView->setSelectionModel(selection);

	QStringList head;
	head << "设备编号"
		 << "设备类型"
		 << "桩号";
	model->setHorizontalHeaderLabels(head); // 设置表头行

	//选择当前单元格变化时的信号与槽
	connect(selection, &QItemSelectionModel::currentChanged, this, &DevicePage::do_currentChanged);

	// 根据内容调整列宽
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->tableView->horizontalHeader()->setStretchLastSection(true);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

// 更新数据
void DevicePage::updateData(QList<Device*> list)
{
	int            row = 0;
	QStandardItem* item;

	if (list.isEmpty())
	{
		model->setRowCount(deviceSet->getDevices().size()); // 设置数据行数

		for (const auto& i : deviceSet->getDevices())
		{
			item = new QStandardItem(i.getDeviceId());
			model->setItem(row, 0, item);

			item = new QStandardItem(i.getDeviceClass());
			model->setItem(row, 1, item);

			item = new QStandardItem(i.getPlaceId());
			model->setItem(row, 2, item);

			row++;
		}
		ui->tableView->sortByColumn(0, Qt::AscendingOrder);
		return;
	}


	model->setRowCount(list.size()); // 设置数据行数

	for (const auto& i : list)
	{
		item = new QStandardItem(i->getDeviceId());
		model->setItem(row, 0, item);

		item = new QStandardItem(i->getDeviceClass());
		model->setItem(row, 1, item);

		item = new QStandardItem(i->getPlaceId());
		model->setItem(row, 2, item);

		row++;
	}
}

// 自定义右键菜单
void DevicePage::on_tableView_customContextMenuRequested(const QPoint& pos)
{
	Q_UNUSED(pos)
	QMenu* menuList = new QMenu(this);
	menuList->addAction(ui->actDevice_add);
	menuList->addAction(ui->actDevice_delete);
	menuList->addAction(ui->actDevice_modify);
	menuList->addSeparator();
	menuList->addAction(ui->actData_filter);
	menuList->exec(QCursor::pos());
	delete menuList;
}
