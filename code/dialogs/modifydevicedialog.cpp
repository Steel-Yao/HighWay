#include "modifydevicedialog.h"
#include "ui_modifydevicedialog.h"

#include <QMessageBox>

ModifyDeviceDialog::ModifyDeviceDialog(Device*            device,
                                       const QStringList& placeIdList,
                                       const QStringList& deviceIdList,
                                       QWidget*           parent)
	: QDialog(parent)
	, ui(new Ui::ModifyDeviceDialog)
	, deviceIdList(deviceIdList)
{
	ui->setupUi(this);
	setWindowFlag(Qt::MSWindowsFixedSizeDialogHint); // 固定窗口大小

	ui->comboBox_placeId->addItems(placeIdList);

	QString deviceClass = device->getDeviceClass();
	ui->label_deviceClass->setText(deviceClass);

	if (deviceClass == "监控探头") ui->label_deviceId->setText("D20215012");
	else ui->label_deviceId->setText("P2021126");

	ui->lineEdit_deviceId->setText(device->getDeviceId().last(4));
}

ModifyDeviceDialog::~ModifyDeviceDialog()
{
	delete ui;
}

QString ModifyDeviceDialog::getNewDeviceId()
{
	return ui->label_deviceId->text() + ui->lineEdit_deviceId->displayText();
}

QString ModifyDeviceDialog::getNewPlaceId()
{
	return ui->comboBox_placeId->currentText();
}

// 确定按钮
void ModifyDeviceDialog::on_pushButton_ok_clicked()
{
	QString tempId = ui->lineEdit_deviceId->displayText();

	QString newDeviceId = ui->label_deviceId->text() + tempId;
	if (deviceIdList.contains(newDeviceId)) // Id重复
		QMessageBox::critical(this, "修改失败", "已存在相同编号的设备！");
	else accept();
}
