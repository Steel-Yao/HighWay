#ifndef DATA_H
#define DATA_H

#include <QDateTime>
#include <QString>

class Data
{
public:
	Data(const QString& deviceId, const QString& placeId, const QDateTime& dateTime)
		: deviceId(deviceId)
		, placeId(placeId)
		, dateTime(dateTime)
	{}

	QString   getDeviceId() const { return deviceId; }
	void      setDeviceId(const QString& newDeviceId) { deviceId = newDeviceId; }
	QString   getPlaceId() const { return placeId; }
	void      setPlaceId(const QString& newPlaceId) { placeId = newPlaceId; }
	QDateTime getDateTime() const { return dateTime; }
	void      setDateTime(const QDateTime& newDateTime) { dateTime = newDateTime; }

protected:
	QString   deviceId, placeId;
	QDateTime dateTime;
};

#endif // DATA_H
