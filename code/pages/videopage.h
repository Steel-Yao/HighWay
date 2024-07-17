#ifndef VIDEOPAGE_H
#define VIDEOPAGE_H

#include <QItemSelectionModel>
#include <QMainWindow>
#include <QStandardItemModel>
#include "structure/dataset.h"
#include "structure/videodata.h"

namespace Ui
{
class VideoPage;
}

class VideoPage : public QMainWindow
{
	Q_OBJECT

public:
	explicit VideoPage(DataSet<VideoData>* videoSet, QWidget* parent = nullptr);
	~VideoPage();
	void setDockWidgetVisible(bool visible);

private slots:
	void do_currentChanged(const QModelIndex& current, const QModelIndex& previous);
	void on_dockWidget_visibilityChanged(bool visible);

	void on_comboBox_type_currentIndexChanged(int index);

	void on_actData_filter_triggered(bool checked);

	void on_tableWidget_currentCellChanged(int currentRow,
	                                       int currentColumn,
	                                       int previousRow,
	                                       int previousColumn);

	void on_pushButton_plus_clicked();

	void on_pushButton_minus_clicked();

	void on_pushButton_edit_clicked();

	void on_pushButton_clear_clicked();

	void on_pushButton_filter_clicked();

	void on_tableView_customContextMenuRequested(const QPoint& pos);

	void on_play_action_triggered();

	void on_startDateTime_editingFinished();

	void on_endDateTime_editingFinished();

private:
	Ui::VideoPage*       ui;
	DataSet<VideoData>*  videoSet;
	QStandardItemModel*  model;
	QItemSelectionModel* selection;
	QStringList          list1;

	void iniData();
	void upData(const QList<VideoData*>& list = QList<VideoData*>(0));
};

#endif // VIDEOPAGE_H
