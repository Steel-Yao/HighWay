#include "adddevicedialog.h"
#include "ui_adddevicedialog.h"

#include <QMessageBox>

AddDeviceDialog::AddDeviceDialog(DeviceSet*         deviceSet,
                                 const QStringList& placeIdList,
                                 QWidget*           parent)
	: QDialog(parent)
	, ui(new Ui::AddDeviceDialog)
	, deviceSet(deviceSet)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);              // 关闭时自动删除
	setWindowFlag(Qt::MSWindowsFixedSizeDialogHint); // 固定窗口大小
	ui->comboBox_placeId->addItems(placeIdList);
}

AddDeviceDialog::~AddDeviceDialog()
{
	delete ui;
}

// 根据类型更新编号前缀
void AddDeviceDialog::on_comboBox_class_currentIndexChanged(int index)
{
	if (index) ui->label_deviceId->setText("P2021126"); // 4G探针
	else ui->label_deviceId->setText("D20215012");      // 监控探头
}

// 确定按钮
void AddDeviceDialog::on_pushButton_ok_clicked()
{
	QString deviceId, temp = ui->lineEdit_deviceId->displayText();
	if (ui->comboBox_class->currentIndex()) deviceId = "P2021126" + temp;
	else deviceId = "D20215012" + temp;

	if (deviceSet->addDevice(deviceId,
	                         ui->comboBox_class->currentText(),
	                         ui->comboBox_placeId->currentText()))
		accept();
	else QMessageBox::critical(this, "添加失败", "已存在相同编号的设备！");
}
