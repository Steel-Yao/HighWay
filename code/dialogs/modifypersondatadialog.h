#ifndef MODIFYPERSONDATADIALOG_H
#define MODIFYPERSONDATADIALOG_H

#include <QDialog>
#include "structure/persondata.h"

namespace Ui
{
class ModifyPersonDataDialog;
}

class ModifyPersonDataDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ModifyPersonDataDialog(PersonData*          data,
	                                DataSet<PersonData>* personData,
	                                QWidget*             parent = nullptr);
	~ModifyPersonDataDialog();

private slots:
	// 更新监测设备候选列表
	void on_comboBox_placeId_currentTextChanged(const QString& arg1);

	void on_pushButton_setIMEI_clicked(); // 设置IMEI

private:
	Ui::ModifyPersonDataDialog* ui;

	PersonData*          data;
	DataSet<PersonData>* personData;
};

#endif // MODIFYPERSONDATADIALOG_H
