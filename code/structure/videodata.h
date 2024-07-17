#ifndef VIDEODATA_H
#define VIDEODATA_H

#include "data.h"
#include "dataset.h"
#include "filter.h"

class VideoData : public Data
{
	friend class DataSet<VideoData>;
	friend class Filter<VideoData>;

public:
	VideoData(const QString&   videoId,
	          const QString&   deviceId,
	          const QString&   placeId,
	          const QDateTime& dateTime)
		: Data(deviceId, placeId, dateTime)
		, videoId(videoId)
	{}

	QString getVideoId() const { return videoId; }
	void    setVideoId(const QString& newVideoId) { videoId = newVideoId; }

private:
	QString videoId;
};

#endif // VIDEODATA_H
