#include "filter.h"
#include <QList>
#include "cardata.h"
#include "device.h"
#include "persondata.h"
#include "place.h"
#include "user.h"
#include "videodata.h"

template<> // 用户筛选
bool Filter<User>::match(const User& temp) const
{
	if (criteria.contains("id")) // id
	{
		QList<QVariant> ids = criteria.values("id");
		if (!ids.contains(QVariant(temp.id))) return false;
	}
	if (criteria.contains("permission")) // 权限
	{
		QList<QVariant> permissions = criteria.values("permission");
		if (!permissions.contains(QVariant(temp.permission))) return false;
	}
	return true;
}

template<> // 设备筛选
bool Filter<Device>::match(const Device& temp) const
{
	if (criteria.contains("deviceId")) // 设备编号
	{
		QList<QVariant> deviceIds = criteria.values("deviceId");
		if (!deviceIds.contains(QVariant(temp.deviceId))) return false;
	}
	if (criteria.contains("deviceClass")) // 设备类型
	{
		QList<QVariant> deviceClasses = criteria.values("deviceClass");
		if (!deviceClasses.contains(QVariant(temp.deviceClass))) return false;
	}
	if (criteria.contains("placeId")) // 桩号
	{
		QList<QVariant> placeIds = criteria.values("placeId");
		if (!placeIds.contains(QVariant(temp.placeId))) return false;
	}
	return true;
}

template<> // 监测点筛选
bool Filter<Place>::match(const Place& temp) const
{
	if (criteria.contains("placeId")) // 桩号
	{
		QList<QVariant> placeIds = criteria.values("placeId");
		if (!placeIds.contains(QVariant(temp.placeId))) return false;
	}
	return true;
}

template<> // 人流数据筛选
bool Filter<PersonData>::match(const PersonData& temp) const
{
	if (criteria.contains("IMEI")) // IMEI
	{
		QList<QVariant> IMEIs = criteria.values("IMEI");
		if (!IMEIs.contains(QVariant(temp.IMEI))) return false;
	}
	if (criteria.contains("deviceId")) // 设备编号
	{
		QList<QVariant> deviceIds = criteria.values("deviceId");
		if (!deviceIds.contains(QVariant(temp.deviceId))) return false;
	}
	if (criteria.contains("placeId")) // 桩号
	{
		QList<QVariant> placeIds = criteria.values("placeId");
		if (!placeIds.contains(QVariant(temp.placeId))) return false;
	}
	if (criteria.contains("startDateTime")) // 范围筛选-时间
	{
		int             cnt    = 0;
		QList<QVariant> starts = criteria.values("startDateTime"),
						ends   = criteria.values("endDateTime");

		for (int i = 0; i < starts.size(); i++)
		{
			if (temp.dateTime < starts[i].toDateTime() || temp.dateTime > ends[i].toDateTime())
				cnt++;
		}

		if (cnt == starts.size()) return false;
	}
	return true;
}

template<> // 车流数据筛选
bool Filter<CarData>::match(const CarData& temp) const
{
	if (criteria.contains("minSpeed")) // 范围筛选-车速
	{
		int             cnt      = 0;
		QList<QVariant> minSpeed = criteria.values("minSpeed"),
						maxSpeed = criteria.values("maxSpeed");

		for (int i = 0; i < minSpeed.size(); i++)
		{
			if (temp.speed > maxSpeed[i].toInt() || temp.speed < minSpeed[i].toInt()) cnt++;
		}

		if (cnt == minSpeed.size()) return false;
	}
	if (criteria.contains("carId")) // 车牌
	{
		QList<QVariant> carIds = criteria.values("carId");
		if (!carIds.contains(QVariant(temp.carId.toUpper()))) return false;
	}
	if (criteria.contains("deviceId")) // 设备编号
	{
		QList<QVariant> deviceIds = criteria.values("deviceId");
		if (!deviceIds.contains(QVariant(temp.deviceId))) return false;
	}
	if (criteria.contains("placeId")) // 桩号
	{
		QList<QVariant> placeIds = criteria.values("placeId");
		if (!placeIds.contains(QVariant(temp.placeId))) return false;
	}
	if (criteria.contains("startDateTime")) // 时间
	{
		int             cnt    = 0;
		QList<QVariant> starts = criteria.values("startDateTime"),
						ends   = criteria.values("endDateTime");

		for (int i = 0; i < starts.size(); i++)
		{
			if (temp.dateTime < starts[i].toDateTime() || temp.dateTime > ends[i].toDateTime())
				cnt++;
		}

		if (cnt == starts.size()) return false;
	}
	return true;
}

template<> // 视频筛选
bool Filter<VideoData>::match(const VideoData& temp) const
{
	if (criteria.contains("videoId")) // 视频编号
	{
		QList<QVariant> videoIds = criteria.values("videoId");
		if (!videoIds.contains(QVariant(temp.videoId))) return false;
	}
	if (criteria.contains("deviceId")) // 录制设备
	{
		QList<QVariant> deviceIds = criteria.values("deviceId");
		if (!deviceIds.contains(QVariant(temp.deviceId))) return false;
	}
	if (criteria.contains("placeId")) // 桩号
	{
		QList<QVariant> placeIds = criteria.values("placeId");
		if (!placeIds.contains(QVariant(temp.placeId))) return false;
	}
	if (criteria.contains("startDateTime")) // 范围筛选-时间
	{
		int             cnt    = 0;
		QList<QVariant> starts = criteria.values("startDateTime"),
						ends   = criteria.values("endDateTime");

		for (int i = 0; i < starts.size(); i++)
		{
			if (temp.dateTime < starts[i].toDateTime() || temp.dateTime > ends[i].toDateTime())
				cnt++;
		}

		if (cnt == starts.size()) return false;
	}
	return true;
}

template<typename T>
class Filter;
