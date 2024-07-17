#ifndef PLACE_H
#define PLACE_H

#include <QObject>
#include <QString>
#include "filter.h"

class Place
{
	friend class PlaceSet;
	friend class Filter<Place>;

public:
	Place(const QString& placeId, const QString& placeName)
		: placeId(placeId)
		, placeName(placeName)
	{}

	QString getPlaceId() const { return placeId; }
	void    setPlaceId(const QString& newPlaceId) { placeId = newPlaceId; }
	QString getPlaceName() const { return placeName; }
	void    setPlaceName(const QString& newPlaceName) { placeName = newPlaceName; }

private:
	QString placeId, placeName;
};

#endif // PLACE_H
