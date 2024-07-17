#include "imeigeneratordialog.h"
#include "ui_imeigeneratordialog.h"

IMEIGeneratorDialog::IMEIGeneratorDialog(const QString& current, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::IMEIGeneratorDialog)
{
	ui->setupUi(this);
	setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
	if (!current.isEmpty())
	{
		ui->lineEdit_tac->setText(current.first(8));
		ui->lineEdit_snr->setText(current.sliced(8, 6));
	}
}

IMEIGeneratorDialog::~IMEIGeneratorDialog()
{
	delete ui;
}

void IMEIGeneratorDialog::on_pushButton_ok_clicked()
{
	QString tac = ui->lineEdit_tac->displayText(), snr = ui->lineEdit_snr->displayText(),
			temp = tac + snr;
	IMEI         = temp + QString::number(calculateLuhn(temp));
	accept();
}

QString IMEIGeneratorDialog::getIMEI() const
{
	return IMEI;
}

// 计算校验位
int IMEIGeneratorDialog::calculateLuhn(const QString& imei)
{
	int sum = 0;

	// 从右向左遍历IMEI号
	for (int i = 0; i < imei.size(); ++i)
	{
		int digit = imei[imei.size() - 1 - i].digitValue(); // 当前位的数字

		// 奇数位处理
		if (i % 2 == 0) sum += digit;
		else // 偶数位处理
		{
			digit *= 2;
			if (digit > 9) digit -= 9; // 等价于将其分成两个单独的数字并相加

			sum += digit;
		}
	}

	// 计算校验位
	int checkDigit = (10 - (sum % 10)) % 10;
	return checkDigit;
}
