#ifndef MODIFYPLACEDIALOG_H
#define MODIFYPLACEDIALOG_H

#include <QDialog>
#include "structure/place.h"

namespace Ui
{
class ModifyPlaceDialog;
}

class ModifyPlaceDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ModifyPlaceDialog(Place*             place,
	                           const QStringList& placeIdList,
	                           QWidget*           parent = nullptr);
	~ModifyPlaceDialog();

	QString getNewPlaceId();   // 获取新桩号
	QString getNewPlaceName(); // 获取新地点

private slots:
	void on_pushButton_ok_clicked(); // 确定按钮

private:
	Ui::ModifyPlaceDialog* ui;

	Place*      place;
	QStringList placeIdList;
};

#endif // MODIFYPLACEDIALOG_H
