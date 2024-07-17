#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include "structure/cardata.h"
#include "structure/dataset.h"
#include "structure/persondata.h"

namespace Ui
{
class HomePage;
}

class HomePage : public QMainWindow
{
	Q_OBJECT

public:
	explicit HomePage(DataSet<CarData>*    carData,
	                  DataSet<PersonData>* personData,
	                  QWidget*             parent = nullptr);
	~HomePage();

public slots:
	void do_timeout();

private:
	Ui::HomePage* ui;

	DataSet<CarData>*    carData;
	DataSet<PersonData>* personData;
};

#endif // HOMEPAGE_H
