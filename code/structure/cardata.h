#ifndef CARDATA_H
#define CARDATA_H

#include "data.h"
#include "dataset.h"
#include "filter.h"

class CarData : public Data
{
	friend class DataSet<CarData>;
	friend class Filter<CarData>;

public:
	CarData(int              speed,
	        const QString&   carId,
	        const QString&   deviceId,
	        const QString&   placeId,
	        const QDateTime& dateTime)
		: Data(deviceId, placeId, dateTime)
		, carId(carId)
		, speed(speed)
	{}

	QString getCarId() const { return carId; }
	void    setCarId(const QString& newCarId) { carId = newCarId; }
	int     getSpeed() const { return speed; }
	void    setSpeed(int newSpeed) { speed = newSpeed; }

private:
	QString carId;
	int     speed;
};

#endif // CARDATA_H
