#ifndef ADDPERSONDATADIALOG_H
#define ADDPERSONDATADIALOG_H

#include <QDialog>
#include "structure/dataset.h"
#include "structure/deviceset.h"
#include "structure/persondata.h"

namespace Ui
{
class AddPersonDataDialog;
}

class AddPersonDataDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddPersonDataDialog(DataSet<PersonData>* personData,
	                             DeviceSet*           deviceSet,
	                             const QStringList&   placeIdList,
	                             QWidget*             parent = nullptr);
	~AddPersonDataDialog();

private slots:
	// 根据桩号更新设备列表
	void on_comboBox_placeId_currentTextChanged(const QString& arg1);

	void on_pushButton_setIMEI_clicked(); // 设置IMEI
	void on_pushButton_ok_clicked();      // 确定按钮

private:
	Ui::AddPersonDataDialog* ui;

	DeviceSet*           deviceSet;
	DataSet<PersonData>* personData;
};

#endif // ADDPERSONDATADIALOG_H
