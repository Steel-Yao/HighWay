#include "videopage.h"
#include "ui_videopage.h"

#include <QMenu>
#include <QMessageBox>
#include <QStandardItem>
#include <QString>
#include "dialogs/videoplayer.h"


VideoPage::VideoPage(DataSet<VideoData>* videoSet, QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::VideoPage)
	, videoSet(videoSet)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	iniData();
	upData();
	ui->dockWidget->setVisible(false); // 筛选器初始不可见
	ui->groupBox_value->setVisible(false);
	setCentralWidget(ui->tableView);
}

VideoPage::~VideoPage()
{
	delete ui;
}

void VideoPage::iniData()
{
	model     = new QStandardItemModel(this);
	selection = new QItemSelectionModel(model, this);
	ui->tableView->setModel(model);
	ui->tableView->setSelectionModel(selection);

	QStringList head;
	head << "视频编号"
		 << "录制时间"
		 << "桩号"
		 << "录制设备";
	model->setHorizontalHeaderLabels(head);

	//选择当前单元格变化时的信号与槽
	connect(selection, &QItemSelectionModel::currentChanged, this, &VideoPage::do_currentChanged);

	// 根据内容调整列宽
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->tableView->horizontalHeader()->setStretchLastSection(true);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

	ui->comboBox_value->setEditable(false);
	ui->groupBox_value_dateTime->setVisible(true);
}

void VideoPage::upData(const QList<VideoData*>& list)
{
	int            row = 0;
	QStandardItem* item;

	if (list.isEmpty())
	{
		model->setRowCount(videoSet->getData().size()); // 设置数据行数

		for (const auto& i : videoSet->getData())
		{
			item = new QStandardItem(i.getVideoId());
			model->setItem(row, 0, item);

			item = new QStandardItem(i.getDateTime().toString("yyyy/MM/dd hh:mm:ss"));
			model->setItem(row, 1, item);

			item = new QStandardItem(i.getPlaceId());
			model->setItem(row, 2, item);

			item = new QStandardItem(i.getDeviceId());
			model->setItem(row, 3, item);

			row++;
		}
		ui->tableView->sortByColumn(0, Qt::AscendingOrder);
		return;
	}
	model->setRowCount(list.size()); // 设置数据行数

	for (const auto& i : list)
	{
		item = new QStandardItem(i->getVideoId());
		model->setItem(row, 0, item);

		item = new QStandardItem(i->getDateTime().toString("yyyy/MM/dd hh:mm:ss"));
		model->setItem(row, 1, item);

		item = new QStandardItem(i->getPlaceId());
		model->setItem(row, 2, item);

		item = new QStandardItem(i->getDeviceId());
		model->setItem(row, 3, item);

		row++;
	}
}

void VideoPage::do_currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	Q_UNUSED(previous)
	if (!ui->dockWidget->isVisible()) return;

	if (current.isValid())
	{
		ui->comboBox_type->setCurrentIndex(current.column());
		ui->comboBox_value->setCurrentText(current.data().toString());
	}
}

void VideoPage::setDockWidgetVisible(bool visible)
{
	if (ui->dockWidget->isFloating()) ui->dockWidget->setVisible(visible);
}
void VideoPage::on_dockWidget_visibilityChanged(bool visible)
{
	ui->actData_filter->setChecked(visible);
}


void VideoPage::on_comboBox_type_currentIndexChanged(int index)
{
	ui->comboBox_value->clear();

	if (index == 0) // 录制时间
	{
		list1.clear();
		for (const auto& i : videoSet->getData())
		{
			if (!list1.contains(i.getDateTime().toString("yyyy/MM/dd hh:mm:ss")))
				list1 << i.getDateTime().toString("yyyy/MM/dd hh:mm:ss");
		}
		QDateTime earliest = videoSet->earliestDateTime(), latest = videoSet->latestDateTime();

		ui->startDateTime->setDateTimeRange(earliest, latest);
		ui->endDateTime->setDateTimeRange(earliest, latest);

		ui->startDateTime->setDateTime(earliest);
		ui->endDateTime->setDateTime(latest);
		ui->groupBox_value_dateTime->setVisible(true);
		ui->groupBox_value->setVisible(false);
	} else if (index == 1) // 桩号
	{
		list1.clear();
		ui->groupBox_value_dateTime->setVisible(false);
		ui->groupBox_value->setVisible(true);
		list1 = videoSet->placeIdList();
	} else // 录制设备
	{
		list1.clear();
		ui->groupBox_value_dateTime->setVisible(false);
		ui->groupBox_value->setVisible(true);
		list1 = videoSet->deviceIdList();
	}
	ui->comboBox_value->addItems(list1);
}
void VideoPage::on_actData_filter_triggered(bool checked)
{
	ui->dockWidget->setVisible(checked);
}


void VideoPage::on_tableWidget_currentCellChanged(int currentRow,
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
	item = nullptr;
}


void VideoPage::on_pushButton_plus_clicked()
{
	int key = ui->comboBox_type->currentIndex();
	if (key == 0) // 探测时间
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
			item = new QTableWidgetItem("录制时间");
			ui->tableWidget->setItem(0, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "添加失败", "已添加相同的规则！");
	} else {
		QString value = ui->comboBox_value->currentText().trimmed();
		auto    temp  = ui->tableWidget->findItems(value, Qt::MatchExactly);
		// 验证是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			ui->tableWidget->insertRow(0);
			QTableWidgetItem* item = new QTableWidgetItem(value);
			ui->tableWidget->setItem(0, 1, item);

			if (key == 1)
			{
				item = new QTableWidgetItem("桩号");
				item->setData(Qt::UserRole, "placeId");
			} else // 桩号
			{
				item = new QTableWidgetItem("录制设备");
				item->setData(Qt::UserRole, "deviceId");
			}
			ui->tableWidget->setItem(0, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "添加失败", "已添加相同的规则！");
	}
	ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
}


void VideoPage::on_pushButton_minus_clicked()
{
	ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}


void VideoPage::on_pushButton_edit_clicked()
{
	int key = ui->comboBox_type->currentIndex(), currentRow = ui->tableWidget->currentRow();
	if (key == 0) // 时间
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
			item = new QTableWidgetItem("录制时间");
			ui->tableWidget->setItem(currentRow, 0, item);
		} else // 规则已添加过
			QMessageBox::critical(this, "修改失败", "已存在相同的规则！");
	} else {
		QString value = ui->comboBox_value->currentText().trimmed();
		auto    temp  = ui->tableWidget->findItems(value, Qt::MatchExactly);
		// 验证是否已添加
		if (temp.isEmpty()) // 规则未添加过
		{
			QTableWidgetItem* item = new QTableWidgetItem(value);
			ui->tableWidget->setItem(currentRow, 1, item);

			if (key == 1)
			{
				item = new QTableWidgetItem("桩号");
				item->setData(Qt::UserRole, "placeId");
			} else // 桩号
			{
				item = new QTableWidgetItem("录制设备");
				item->setData(Qt::UserRole, "deviceId");
			}
			ui->tableWidget->setItem(currentRow, 0, item);

		} else // 规则已添加过
			QMessageBox::critical(this, "修改失败", "已存在相同的规则！");
	}
	ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
}


void VideoPage::on_pushButton_clear_clicked()
{
	ui->tableWidget->setRowCount(0);
}


void VideoPage::on_pushButton_filter_clicked()
{
	model->setRowCount(0);
	int rowCount = ui->tableWidget->rowCount();
	if (!rowCount) upData();
	else
	{
		Filter<VideoData> filter;

		for (int i = 0; i < rowCount; i++)
		{
			QTableWidgetItem *keyItem   = ui->tableWidget->item(i, 0),
							 *valueItem = ui->tableWidget->item(i, 1);
			if (keyItem->text() == "录制时间")
			{
				filter.addCriteria("startDateTime", valueItem->data(Qt::UserRole));
				filter.addCriteria("endDateTime", valueItem->data(Qt::UserRole + 1));
			} else {
				filter.addCriteria(keyItem->data(Qt::UserRole).toString(), valueItem->text());
			}
		}
		auto list = videoSet->filterData(filter);
		if (list.isEmpty())
		{
			QMessageBox::information(this, "提示", "没有符合要求的项！");
			return;
		}
		upData(list);
	}
	ui->tableView->sortByColumn(0, Qt::AscendingOrder);
}

void VideoPage::on_tableView_customContextMenuRequested(const QPoint& pos)
{
	Q_UNUSED(pos)
	QMenu* menuList = new QMenu(this);
	menuList->addSeparator();
	menuList->addAction(ui->actData_filter);
	menuList->exec(QCursor::pos());
	delete menuList;
}


void VideoPage::on_play_action_triggered()
{
	QModelIndex index = selection->currentIndex();
	if (index.isValid())
	{
		QModelIndex  current = model->index(index.row(), 0);
		QString      path    = "./data/video/" + current.data().toString() + ".mp4";
		VideoPlayer* player  = new VideoPlayer(path, this);
		player->setWindowTitle(current.data().toString());
		player->setWindowIcon(QIcon(":/icons/video.png"));
		player->exec();
	}
}


void VideoPage::on_startDateTime_editingFinished()
{
	ui->endDateTime->setMinimumDateTime(ui->startDateTime->dateTime());
}


void VideoPage::on_endDateTime_editingFinished()
{
	ui->startDateTime->setMaximumDateTime(ui->endDateTime->dateTime());
}
