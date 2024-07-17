#ifndef MODIFYDEVICEDIALOG_H
#define MODIFYDEVICEDIALOG_H

#include <QDialog>
#include "structure/device.h"

namespace Ui
{
class ModifyDeviceDialog;
}

class ModifyDeviceDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ModifyDeviceDialog(Device*            device,
	                            const QStringList& placeIdList,
	                            const QStringList& deviceIdList,
	                            QWidget*           parent = nullptr);
	~ModifyDeviceDialog();

	QString getNewDeviceId();
	QString getNewPlaceId();

private slots:
	void on_pushButton_ok_clicked(); // 确定按钮

private:
	Ui::ModifyDeviceDialog* ui;

	QStringList deviceIdList;
};

#endif // MODIFYDEVICEDIALOG_H
