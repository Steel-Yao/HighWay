#ifndef FILTER_H
#define FILTER_H

#include <QMultiHash>
#include <QString>
#include <QVariant>

template<typename T>
class Filter
{
public:
	void addCriteria(const QString& key, const QVariant& value) { criteria.insert(key, value); }
	void delCriteria(const QString& key) { criteria.remove(key); }
	void delCriteria(const QString& key, const QVariant& value) { criteria.remove(key, value); }
	bool match(const T& temp) const;
	const QMultiHash<QString, QVariant>& getCriteria() const { return criteria; }

private:
	QMultiHash<QString, QVariant> criteria;
};

#endif // FILTER_H
