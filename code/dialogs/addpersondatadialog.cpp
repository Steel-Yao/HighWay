#include "addpersondatadialog.h"
#include "ui_addpersondatadialog.h"

#include <QMessageBox>
#include "imeigeneratordialog.h"

AddPersonDataDialog::AddPersonDataDialog(DataSet<PersonData>* personData,
                                         DeviceSet*           deviceSet,
                                         const QStringList&   placeIdList,
                                         QWidget*             parent)
	: QDialog(parent)
	, ui(new Ui::AddPersonDataDialog)
	, deviceSet(deviceSet)
	, personData(personData)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);              // 关闭时自动删除
	setWindowFlag(Qt::MSWindowsFixedSizeDialogHint); // 固定窗口大小

	ui->comboBox_placeId->addItems(placeIdList);
	ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

AddPersonDataDialog::~AddPersonDataDialog()
{
	delete ui;
}

// 根据桩号更新设备列表
void AddPersonDataDialog::on_comboBox_placeId_currentTextChanged(const QString& arg1)
{
	ui->comboBox_deviceId->clear();

	Filter<Device> filter;
	filter.addCriteria("placeId", arg1);
	filter.addCriteria("deviceClass", "4G探针");
	auto list = deviceSet->filterDevice(filter);
	if (list.isEmpty()) // 没有设备
	{
		ui->comboBox_deviceId->addItem("无可用设备！");
		return;
	}
	// 有设备
	QStringList deviceIdList;
	for (const auto& i : list)
		if (!deviceIdList.contains(i->getDeviceId())) deviceIdList << i->getDeviceId();

	ui->comboBox_deviceId->addItems(deviceIdList);
}

// 设置IMEI
void AddPersonDataDialog::on_pushButton_setIMEI_clicked()
{
	IMEIGeneratorDialog* generator = new IMEIGeneratorDialog(ui->lineEdit_IMEI->text(), this);
	if (generator->exec() == QDialog::Accepted) ui->lineEdit_IMEI->setText(generator->getIMEI());
	delete generator;
}

// 确定按钮
void AddPersonDataDialog::on_pushButton_ok_clicked()
{
	if (ui->lineEdit_IMEI->text().isEmpty())
		QMessageBox::critical(this, "添加失败", "请设置IMEI号！");
	else if (ui->comboBox_deviceId->currentText() == "无可用设备！")
		QMessageBox::critical(this, "添加失败", "无可用设备！");
	else
	{
		QString tempTime = ui->dateTimeEdit->dateTime().toString("yyyy/MM/dd hh:mm:ss");
		personData->addData(ui->lineEdit_IMEI->text(),
		                    ui->comboBox_deviceId->currentText(),
		                    ui->comboBox_placeId->currentText(),
		                    QDateTime::fromString(tempTime, "yyyy/MM/dd hh:mm:ss"));
		accept();
	}
}
