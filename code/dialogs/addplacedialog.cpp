#include "addplacedialog.h"
#include "ui_addplacedialog.h"

#include <QMessageBox>
#include <QString>

AddPlaceDialog::AddPlaceDialog(PlaceSet *placeSet, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::AddPlaceDialog)
	, placeSet(placeSet)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
}

AddPlaceDialog::~AddPlaceDialog()
{
	delete ui;
}

void AddPlaceDialog::on_pushButton_ok_clicked()
{
	QString m = ui->lineEdit_placeId->displayText();
	QString n = ui->lineEdit_placeName->displayText();
	if (n.isEmpty())
	{
		QMessageBox::information(this, "提示", "地点不能为空");
	} else {
		int w = placeSet->addPlace(m, n);
		if (!w)
		{
			QMessageBox::information(this, "提示", "重复桩号");
		} else {
			QMessageBox::information(this, "提示", "添加成功");
			accept();
		}
	}
}
