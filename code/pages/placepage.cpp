#include "placepage.h"
#include "ui_placepage.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include "dialogs/addplacedialog.h"
#include "dialogs/modifyplacedialog.h"

PlacePage::PlacePage(PlaceSet*            placeSet,
                     DeviceSet*           deviceSet,
                     DataSet<PersonData>* personData,
                     DataSet<CarData>*    carData,
                     QWidget*             parent)
	: QMainWindow(parent)
	, ui(new Ui::PlacePage)
	, placeSet(placeSet)
	, deviceSet(deviceSet)
	, personData(personData)
	, carData(carData)
{
	ui->setupUi(this);
	model     = new QStandardItemModel(0, 2, this);
	selection = new QItemSelectionModel(model);
	ui->tableView->setModel(model);
	ui->tableView->setSelectionModel(selection);

	showmenu();

	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

PlacePage::~PlacePage()
{
	placeSet->savePlaces();
	delete ui;
}

void PlacePage::showmenu()
{
	//model1->setItem(4,1,item);
	model->setRowCount(0);
	QStringList table_h_headers;
	table_h_headers << "桩号"
					<< "地点";
	model->setHorizontalHeaderLabels(table_h_headers);
	model->setRowCount(placeSet->getPlaces().size());
	int row = 0;
	for (const auto& place : placeSet->getPlaces())
	{
		QString placeId   = place.getPlaceId();
		QString placeName = place.getPlaceName();

		QStandardItem* item  = new QStandardItem(placeId);
		QStandardItem* item1 = new QStandardItem(placeName);

		model->setItem(row, 0, item);
		model->setItem(row, 1, item1);

		// 移动到下一行
		row++;
	}

	// ui->tableView->setColumnWidth(0, 200);
	// ui->tableView->setColumnWidth(1, 200);

	setCentralWidget(ui->tableView);
}

void PlacePage::on_tableView_customContextMenuRequested(const QPoint& pos)
{
	Q_UNUSED(pos)
	QMenu* menuList = new QMenu(this); //创建菜单
	//添加 Action 将其作为菜单项
	menuList->addAction(ui->actPlace_add);
	menuList->addAction(ui->actPlace_delete);
	menuList->addAction(ui->actPlace_modify);

	menuList->exec(QCursor::pos()); //在鼠标光标位置显示快捷菜单
	delete menuList;
}


void PlacePage::on_actPlace_add_triggered()
{
	AddPlaceDialog* addPlaces = new AddPlaceDialog(placeSet, this);
	if (addPlaces->exec() == QDialog::Accepted)
	{
		showmenu();
	}
}

void PlacePage::on_actPlace_delete_triggered()
{
	QModelIndex current = selection->currentIndex();
	if (current.isValid())
	{
		QString placeId = model->index(current.row(), 0).data().toString();
		auto    result  = QMessageBox::question(this,
                                            "确认删除",
                                            "您确定要删除桩号为" + placeId
                                                + "的监测点吗？\n这将同时删除该监测点的设备！");
		if (result == QMessageBox::Yes)
		{
			placeSet->delPlace(placeId);
			emit placeChanged();
		}
	}
	showmenu();
}

void PlacePage::on_actPlace_modify_triggered()
{
	QModelIndex current = selection->currentIndex();
	if (!current.isValid()) return;

	QString       placeId = model->index(current.row(), 0).data().toString();
	Filter<Place> filter;
	filter.addCriteria("placeId", placeId);
	auto        list        = placeSet->filterPlace(filter);
	Place*      place       = list[0];
	QStringList placeIdList = placeSet->placeIdList();
	placeIdList.removeOne(placeId);

	ModifyPlaceDialog* modifyDialog = new ModifyPlaceDialog(place, placeIdList, this);
	if (modifyDialog->exec() == QDialog::Accepted)
	{
		QString newPlaceId   = modifyDialog->getNewPlaceId(),
				newPlaceName = modifyDialog->getNewPlaceName();
		if (newPlaceId != placeId)
		{
			// 更新对应设备的桩号
			Filter<Device> deviceFilter;
			deviceFilter.addCriteria("placeId", placeId);
			auto deviceList = deviceSet->filterDevice(deviceFilter);
			for (auto& i : deviceList) i->setPlaceId(newPlaceId);

			// 更新对应数据的桩号
			Filter<CarData> carFilter;
			carFilter.addCriteria("placeId", placeId);
			auto carList = carData->filterData(carFilter);
			for (auto& i : carList) i->setPlaceId(newPlaceId);

			Filter<PersonData> personFilter;
			personFilter.addCriteria("placeId", placeId);
			auto personList = personData->filterData(personFilter);
			for (auto& i : personList) i->setPlaceId(newPlaceId);

			emit placeChanged();
		}
		place->setPlaceId(newPlaceId);
		place->setPlaceName(newPlaceName);
		showmenu();
	}
	delete modifyDialog;
}
