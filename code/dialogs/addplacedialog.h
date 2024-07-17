#ifndef ADDPLACEDIALOG_H
#define ADDPLACEDIALOG_H

#include <QDialog>
#include "structure/place.h"
#include "structure/placeset.h"

namespace Ui
{
class AddPlaceDialog;
}

class AddPlaceDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddPlaceDialog(PlaceSet* placeSet, QWidget* parent = nullptr);
	~AddPlaceDialog();
private slots:
	void on_pushButton_ok_clicked();

private:
	Ui::AddPlaceDialog* ui;
	PlaceSet*           placeSet;
};

#endif // ADDPLACEDIALOG_H
