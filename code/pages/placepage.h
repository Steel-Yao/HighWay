#ifndef PLACEPAGE_H
#define PLACEPAGE_H

#include <QItemSelectionModel>
#include <QMainWindow>
#include <QStandardItemModel>
#include "structure/cardata.h"
#include "structure/dataset.h"
#include "structure/deviceset.h"
#include "structure/persondata.h"
#include "structure/place.h"
#include "structure/placeset.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class PlacePage;
}
QT_END_NAMESPACE

class PlacePage : public QMainWindow
{
	Q_OBJECT

public:
	PlacePage(PlaceSet*            placeSet,
	          DeviceSet*           deviceSet,
	          DataSet<PersonData>* personData,
	          DataSet<CarData>*    carData,
	          QWidget*             parent = nullptr);
	~PlacePage();

signals:
	void placeChanged();

private slots:
	void on_tableView_customContextMenuRequested(const QPoint& pos);

	void on_actPlace_add_triggered();
	void on_actPlace_delete_triggered();
	void on_actPlace_modify_triggered();

private:
	Ui::PlacePage* ui;

	PlaceSet*            placeSet;
	DeviceSet*           deviceSet;
	DataSet<PersonData>* personData;
	DataSet<CarData>*    carData;

	QStandardItemModel*  model;
	QItemSelectionModel* selection;

	void showmenu();
};
#endif // PLACEPAGE_H
