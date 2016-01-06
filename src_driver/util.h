#pragma once

#include <algorithm>
#include <array>

// メディアンフィルタ

template<class T, size_t N>
class MedianFilter
{
	static_assert(N > 0, "'N' must be >0");

public:
	MedianFilter() : mRingPos(0), mFull(false) {}
	
	T update(T v) {
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

private:
	std::array<T, N> mBuffer;
	size_t mRingPos;
	bool mFull;
};
