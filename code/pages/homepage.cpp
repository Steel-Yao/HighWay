#include "homepage.h"
#include "ui_homepage.h"

#include <QTime>

HomePage::HomePage(DataSet<CarData>* carData, DataSet<PersonData>* personData, QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::HomePage)
	, carData(carData)
	, personData(personData)
{
	ui->setupUi(this);
}

HomePage::~HomePage()
{
	delete ui;
}

void HomePage::do_timeout()
{
	QTime   currentTime = QTime::currentTime();
	QString temp        = QString::asprintf("%02d", currentTime.hour());
	ui->lcdNumber_hour->display(temp);
	temp = QString::asprintf("%02d", currentTime.minute());
	ui->lcdNumber_min->display(temp);
	temp = QString::asprintf("%02d", currentTime.second());
	ui->lcdNumber_sec->display(temp);

	ui->label_carCount->setText(QString::number(carData->getData().size()));
	ui->label_personCount->setText(QString::number(personData->getData().size()));
}
