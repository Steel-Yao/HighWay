#ifndef USER_H
#define USER_H

#include <QString>
#include "filter.h"

class User
{
	friend class UserSet;
	friend class Filter<User>;

public:
	User() {}
	User(const QString& id, const QString& password, int permission)
		: id(id)
		, password(password)
		, permission(permission)
	{}

	QString getId() const { return id; }
	void    setId(const QString& newId) { id = newId; }
	QString getPassword() const { return password; }
	void    setPassword(const QString& newPassword) { password = newPassword; }
	int     getPermission() const { return permission; }
	void    setPermission(int newPermission) { permission = newPermission; }

private:
	QString id, password;
	int     permission;
};

#endif // USER_H
