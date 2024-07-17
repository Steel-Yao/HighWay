#ifndef MANAGEPAGE_H
#define MANAGEPAGE_H

#include <QListWidget>
#include <QMainWindow>
#include "structure/userset.h"

class ManagePage : public QMainWindow
{
	Q_OBJECT
public:
	explicit ManagePage(UserSet* userSet, QWidget* parent = nullptr);

	QListWidget* listWidget;
	int          counter = 1;

private:
	UserSet* userSet;

public slots:
	void onAddUserTriggered();
	void onDelteUserTriggered();
};

#endif // MANAGEPAGE_H
