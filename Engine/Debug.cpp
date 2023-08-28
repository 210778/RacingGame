#include <windows.h>
#include "Debug.h"

void Debug::Log(int value, bool isLineFeed)
{
	char str[256];
	sprintf_s(str, "%d", value);
	Debug::Log(str, isLineFeed);
}

void Debug::Log(float value, bool isLineFeed)
{
	char str[256];
	sprintf_s(str, "%f", value);
	Debug::Log(str, isLineFeed);
}

void Debug::Log(std::string value, bool isLineFeed)
{
	if (isLineFeed)	value += "\n";
	OutputDebugString(value.c_str());
}

#include <list>
#include<vector>
#include <chrono>
#include <map>

using std::list;
using std::vector;
using std::string;
using std::to_string;
using std::map;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::minutes;
using std::chrono::hours;

namespace Debug
{
	//実行時間を測定して平均を求めたい
	class TimeLogClass
	{
	public:
		unsigned long long count_;
		long double time_;
		long double average_;

		system_clock::time_point startTime_;
		system_clock::time_point endTime_;

		int nameType_;

		TimeLogClass()
		{
			count_ = 0;
			time_ = 0;
			average_ = 0.0l;
			nameType_ = _microseconds;
		}
		~TimeLogClass() {};

		//カウント
		void TimeCount(long double addTime)
		{
			count_++;

			//最初の値は外れ値になりがちなので無視
			if (count_ < 10){
				return;
			};

			time_ += addTime;

			average_ = average_ + (addTime - average_) / (long double)count_;
		}
		//計測開始
		void TimeCountStart()
		{
			startTime_ = system_clock::now();
		}
		//計測終了
		void TimeCountEnd(int timeType)
		{
			endTime_ = system_clock::now();

			auto alfa = duration_cast<milliseconds>(endTime_ - startTime_).count();
			auto bravo = endTime_ - startTime_;

			//頭がわるい気がする
			switch (timeType){
			default:
			case TimeType::_nanoseconds:
				TimeCount(duration_cast<nanoseconds>(endTime_ - startTime_).count()); break;
			case TimeType::_microseconds:
				TimeCount(duration_cast<microseconds>(endTime_ - startTime_).count()); break;
			case TimeType::_milliseconds:
				TimeCount(duration_cast<milliseconds>(endTime_ - startTime_).count()); break;
			case TimeType::_seconds:
				TimeCount(duration_cast<seconds>(endTime_ - startTime_).count()); break;
			case TimeType::_minutes:
				TimeCount(duration_cast<minutes>(endTime_ - startTime_).count()); break;
			case TimeType::_hours:
				TimeCount(duration_cast<minutes>(endTime_ - startTime_).count()); break;
			}
		}
	};

	map<string, TimeLogClass> timeLog_;

	//カウント開始
	void TimerLogStart(string name)
	{
		//キー が設定されているか？
		if (timeLog_.find(name) != timeLog_.end())
		{
			//カウント開始
			timeLog_[name].TimeCountStart();
		}
		else
		{
			//設定されていない場合の処理
			TimeLogClass tlc;
			timeLog_[name] = tlc;
		}
	}
	//経過時間の表示
	void TimerLogPrint(string name, int timeType, string message)
	{
		//キー が設定されているか？
		if (timeLog_.find(name) == timeLog_.end())
			return;

		//単位
		string unit;
		switch (timeType) {
		default:unit = "unit"; break;
		case TimeType::_nanoseconds:unit	= "ns"; break;
		case TimeType::_microseconds:unit	= "μs"; break;
		case TimeType::_milliseconds:unit	= "ms"; break;
		case TimeType::_seconds:unit		= "s"; break;
		case TimeType::_minutes:unit		= "m"; break;
		case TimeType::_hours:unit			= "h"; break; }

		string debugMessage = "Timer log::";
		debugMessage += message + "\n";
		debugMessage += "\t平均時間「" + to_string(timeLog_[name].average_) + unit + "」\n";
		debugMessage += "\t実行回数「" + to_string(timeLog_[name].count_) + "」\n";
		Debug::Log(debugMessage, true);
	}
	//カウント終了
	void TimerLogEnd(string name, int timeType, string message)
	{
		//キー が設定されているか？
		if (timeLog_.find(name) == timeLog_.end())
			return;

		//カウント終了
		timeLog_[name].TimeCountEnd(timeType);
	}
	//カウント終了　メッセージ省略版
	void TimerLogEnd(string name, int timeType)
	{
		TimerLogEnd(name, timeType, name);
	}
	//キーのみ
	void TimerLogEnd(string name)
	{
		TimerLogEnd(name, timeLog_[name].nameType_, name);
	}

	void TimerLogPrintAll()
	{
		for (const auto& itr : timeLog_)
		{
			string name = (itr).first;
			//表示
			TimerLogPrint(name, timeLog_[name].nameType_, name);
		}
	}
};