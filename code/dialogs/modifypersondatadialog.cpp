#include "modifypersondatadialog.h"
#include "ui_modifypersondatadialog.h"

#include "imeigeneratordialog.h"

ModifyPersonDataDialog::ModifyPersonDataDialog(PersonData*          data,
                                               DataSet<PersonData>* personData,
                                               QWidget*             parent)
	: QDialog(parent)
	, ui(new Ui::ModifyPersonDataDialog)
	, data(data)
	, personData(personData)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);              // 关闭时自动删除
	setWindowFlag(Qt::MSWindowsFixedSizeDialogHint); // 固定窗口大小

	// 初始化部件数据
	ui->comboBox_placeId->addItems(personData->placeIdList());
	ui->comboBox_placeId->setCurrentText(data->getPlaceId());
	ui->comboBox_deviceId->setCurrentText(data->getDeviceId());
	ui->dateTimeEdit->setDateTime(data->getDateTime());
	ui->lineEdit_IMEI->setText(data->getIMEI());
}

ModifyPersonDataDialog::~ModifyPersonDataDialog()
{
	delete ui;
}

// 更新监测设备候选列表
void ModifyPersonDataDialog::on_comboBox_placeId_currentTextChanged(const QString& arg1)
{
	ui->comboBox_deviceId->clear();

	Filter<PersonData> filter;
	filter.addCriteria("placeId", arg1);
	auto list = personData->filterData(filter);
	if (list.isEmpty()) // 没有设备
	{
		ui->comboBox_deviceId->addItem("无设备！");
		return;
	}
	// 有设备
	QStringList deviceIdList;
	for (const auto& i : list)
		if (!deviceIdList.contains(i->getDeviceId())) deviceIdList << i->getDeviceId();

	ui->comboBox_deviceId->addItems(deviceIdList);
}

// 设置IMEI
void ModifyPersonDataDialog::on_pushButton_setIMEI_clicked()
{
	IMEIGeneratorDialog* generator = new IMEIGeneratorDialog(ui->lineEdit_IMEI->text(), this);
	if (generator->exec() == QDialog::Accepted) ui->lineEdit_IMEI->setText(generator->getIMEI());
	delete generator;
}
