#pragma once

#include <string>

namespace Debug
{
	void Log(int value, bool isLineFeed = false);
	void Log(float value, bool isLineFeed = false);
	void Log(std::string value, bool isLineFeed = false);

	//表示する時間の単位
	enum TimeType { _nanoseconds, _microseconds, _milliseconds, _seconds, _minutes, _hours };
	//時間計測開始　name:キー
	void TimerLogStart(std::string name);
	//時間計測終了　name:キー　timeType:時間の単位　message:表示したい文字列
	void TimerLogEnd(std::string name, int timeType, std::string message);
	//時間計測終了　name:キー、表示したい文字列　timeType:時間の単位
	void TimerLogEnd(std::string name, int timeType);
	//時間計測終了　name:キー、表示したい文字列　時間の単位はミリ秒
	void TimerLogEnd(std::string name);
};

