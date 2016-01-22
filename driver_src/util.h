#pragma once

#include <algorithm>
#include <array>

// メディアンフィルタ
template<size_t N>
class MedianFilter
{
	static_assert(N > 0, "'N' must be >0");

public:
	MedianFilter() : mRingPos(0), mFull(false) {}
	
	double update(double v) {
		// リングバッファ更新
		mBuffer[mRingPos] = v;
		// リングバッファ位置更新
		++mRingPos;
		if(mRingPos >= N) {
			mRingPos =0;
			mFull = true;
		}
		// メディアン値取得対象サイズ取得
		size_t targetNum = N;
        if(!mFull) targetNum = mRingPos;
		
		// メディアン算出
		auto buf = mBuffer;
		std::sort(buf.begin(), buf.end());
		
		return *(buf.begin() + N/2);
	}

	bool isFull()const noexcept { 
		return mFull; 
	}

private:
	std::array<double, N> mBuffer;
	size_t mRingPos;
	bool mFull;
};


// エッジ検出器
class EdgeDetector {
public:
	enum class Status {
		Unknown,
		Raise,	// L->H
		Fall,   // H->L
		High,	// H
		Low,	// L
	};
	enum class Level {
		Low,		// v <= L
		Undefined, 	// L < v < H or INF
		High,		// H <= v
	};

public:
	EdgeDetector(double lowThreshold, double highThreshold);
	
	Level level(double value)const;
	Status update(double value);

private:
	Status mStatus;
	double mLowThreshold; // H->L 閾値
	double mHighThreshold;// L->H 閾値
};
