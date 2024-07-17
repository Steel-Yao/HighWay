#ifndef PERSONDATA_H
#define PERSONDATA_H

#include "data.h"
#include "dataset.h"
#include "filter.h"

class PersonData : public Data
{
	friend class DataSet<PersonData>;
	friend class Filter<PersonData>;

public:
	PersonData(const QString&   IMEI,
	           const QString&   deviceId,
	           const QString&   placeId,
	           const QDateTime& dateTime)
		: Data(deviceId, placeId, dateTime)
		, IMEI(IMEI)
	{}

	QString getIMEI() const { return IMEI; }
	void    setIMEI(const QString& newIMEI) { IMEI = newIMEI; }

private:
	QString IMEI;
};

#endif // PERSONDATA_H
