#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>
#include "structure/deviceset.h"

namespace Ui
{
class AddDeviceDialog;
}

class AddDeviceDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddDeviceDialog(DeviceSet*         deviceSet,
	                         const QStringList& placeIdList,
	                         QWidget*           parent = nullptr);
	~AddDeviceDialog();

private slots:
	void on_comboBox_class_currentIndexChanged(int index); // 根据类型更新编号前缀
	void on_pushButton_ok_clicked();                       // 确定按钮

private:
	Ui::AddDeviceDialog* ui;
	DeviceSet*           deviceSet;
};

#endif // ADDDEVICEDIALOG_H
