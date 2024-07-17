#ifndef IMEIGENERATORDIALOG_H
#define IMEIGENERATORDIALOG_H

#include <QDialog>

namespace Ui
{
class IMEIGeneratorDialog;
}

class IMEIGeneratorDialog : public QDialog
{
	Q_OBJECT

public:
	explicit IMEIGeneratorDialog(const QString& current, QWidget* parent = nullptr);
	~IMEIGeneratorDialog();

	QString getIMEI() const;

private slots:
	void on_pushButton_ok_clicked();

private:
	Ui::IMEIGeneratorDialog* ui;

	QString IMEI;
	int     calculateLuhn(const QString& imei);
};

#endif // IMEIGENERATORDIALOG_H
