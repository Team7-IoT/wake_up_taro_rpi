#include "util.h"
#include <cassert>
#include <cfloat>

EdgeDetector::EdgeDetector(double lowThreshold, double highThreshold)
	: mStatus(Status::Unknown)
	, mLowThreshold(lowThreshold)
	, mHighThreshold(highThreshold)
{
	assert(lowThreshold < highThreshold);
}

EdgeDetector::Level EdgeDetector::level(double value)const
{
	if(value == DBL_MAX) {
		return Level::Undefined;
	} else if(value <= mLowThreshold) {
		return Level::Low;
	} else if(value >= mHighThreshold) {
		return Level::High;
	} else {
		return Level::Undefined;
	}
}

EdgeDetector::Status EdgeDetector::update(double value)
{
	auto lv = level(value);

	switch(mStatus) {
	case Status::Unknown:
		if(lv != Level::High) {
			mStatus = Status::Low;
			return Status::Low;
		} else {
			mStatus = Status::High;
			return Status::High;
		}
		break;
	case Status::Low:
		if(lv != Level::High) {
			return mStatus; // hold
		} else {
			mStatus = Status::High;
			return Status::Raise;
		}
		break;
	case Status::High:
		if(lv != Level::Low) {
			return mStatus; // hold
		} else {
			mStatus = Status::Low;
			return Status::Fall;
		}
		break;
	default:
		assert(false);
		return mStatus;
	}
}

