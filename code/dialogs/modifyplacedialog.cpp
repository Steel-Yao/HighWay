#include "modifyplacedialog.h"
#include "ui_modifyplacedialog.h"

#include <QMessageBox>

ModifyPlaceDialog::ModifyPlaceDialog(Place* place, const QStringList& placeIdList, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ModifyPlaceDialog)
	, place(place)
	, placeIdList(placeIdList)
{
	ui->setupUi(this);
	setWindowFlag(Qt::MSWindowsFixedSizeDialogHint); // 固定窗口大小

	ui->lineEdit_placeId->setText(place->getPlaceId());
	ui->lineEdit_placeName->setText(place->getPlaceName());
}

ModifyPlaceDialog::~ModifyPlaceDialog()
{
	delete ui;
}

// 获取新桩号
QString ModifyPlaceDialog::getNewPlaceId()
{
	return ui->lineEdit_placeId->displayText();
}

// 获取新地点
QString ModifyPlaceDialog::getNewPlaceName()
{
	return ui->lineEdit_placeName->text();
}

// 确定按钮
void ModifyPlaceDialog::on_pushButton_ok_clicked()
{
	QString newPlaceId   = ui->lineEdit_placeId->displayText(),
			newPlaceName = ui->lineEdit_placeName->text();
	if (placeIdList.contains(newPlaceId))
		QMessageBox::critical(this, "修改失败", "已存在相同桩号的监测点！");
	else if (newPlaceName.isEmpty()) QMessageBox::critical(this, "修改失败", "地点不能为空！");
	else accept();
}
