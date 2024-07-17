#ifndef CURRENTUSERPAGE_H
#define CURRENTUSERPAGE_H

#include <QDialog>
#include "structure/user.h"
#include "structure/userset.h"

namespace Ui
{
class CurrentUserPage;
}

class CurrentUserPage : public QDialog
{
	Q_OBJECT

public:
	explicit CurrentUserPage(User* currentUser, UserSet* userSet, QWidget* parent = nullptr);
	~CurrentUserPage();

private slots:
	void on_pushButton_modify_clicked();

private:
	Ui::CurrentUserPage* ui;
	User*                currentUser;
	UserSet*             userSet;
};

#endif // CURRENTUSERPAGE_H
