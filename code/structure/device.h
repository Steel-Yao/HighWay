#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include "filter.h"

class Device
{
	friend class DeviceSet;
	friend class Filter<Device>;

public:
	Device(const QString& deviceId, const QString& deviceClass, const QString& placeId)
		: deviceId(deviceId)
		, deviceClass(deviceClass)
		, placeId(placeId)
	{}

	QString getDeviceId() const { return deviceId; }
	void    setDeviceId(const QString& newDeviceId) { deviceId = newDeviceId; }
	QString getDeviceClass() const { return deviceClass; }
	void    setDeviceClass(const QString& newDeviceClass) { deviceClass = newDeviceClass; }
	QString getPlaceId() const { return placeId; }
	void    setPlaceId(const QString& newPlaceId) { placeId = newPlaceId; }

private:
	QString deviceId, deviceClass, placeId;
};

#endif // DEVICE_H
