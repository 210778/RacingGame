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
//#include <string>
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
	//���s���Ԃ𑪒肵�ĕ��ς����߂���
	class TimeLogClass
	{
	public:

		unsigned long long count_;
		long double time_;
		long double average_;

		system_clock::time_point startTime_;
		system_clock::time_point  endTime_;

		int nameType_;

		TimeLogClass()
		{
			count_ = 0;
			time_ = 0;
			average_ = 0.0l;
			nameType_ = _microseconds;
		}
		~TimeLogClass() {};
		//�J�E���g
		void TimeCount(long double addTime)
		{
			count_++;

			//�Ȃ�ƂȂ��ŏ��̒l�𖳎����Ă݂�
			if (count_ < 10){
				return;
			};

			time_ += addTime;

			average_ = average_ + (addTime - average_) / (long double)count_;
		}
		//�v���J�n
		void TimeCountStart()
		{
			startTime_ = system_clock::now();
		}
		//�v���I��
		void TimeCountEnd(int timeType)
		{
			endTime_ = system_clock::now();

			auto alfa = duration_cast<milliseconds>(endTime_ - startTime_).count();
			auto bravo = endTime_ - startTime_;

			//������邢�C������
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

	//�J�E���g�J�n
	void TimerLogStart(string name)
	{
		//�L�[ ���ݒ肳��Ă��邩�H
		if (timeLog_.find(name) != timeLog_.end())
		{
			//�J�E���g�J�n
			timeLog_[name].TimeCountStart();
		}
		else
		{
			//�ݒ肳��Ă��Ȃ��ꍇ�̏���
			TimeLogClass tlc;
			timeLog_[name] = tlc;
		}
	}
	//�o�ߎ��Ԃ̕\��
	void TimerLogPrint(string name, int timeType, string message)
	{
		//�L�[ ���ݒ肳��Ă��邩�H
		if (timeLog_.find(name) == timeLog_.end())
			return;

		//�P��
		string unit;
		switch (timeType) {
		default:unit = "unit"; break;
		case TimeType::_nanoseconds:unit	= "ns"; break;
		case TimeType::_microseconds:unit	= "��s"; break;
		case TimeType::_milliseconds:unit	= "ms"; break;
		case TimeType::_seconds:unit		= "s"; break;
		case TimeType::_minutes:unit		= "m"; break;
		case TimeType::_hours:unit			= "h"; break; }

		string debugMessage = "Timer log::";
		debugMessage += message + "\n";
		debugMessage += "\t���ώ��ԁu" + to_string(timeLog_[name].average_) + unit + "�v\n";
		debugMessage += "\t���v�񐔁u" + to_string(timeLog_[name].count_) + "�v\n";
		Debug::Log(debugMessage, true);
	}
	//�J�E���g�I��
	void TimerLogEnd(string name, int timeType, string message)
	{
		//�L�[ ���ݒ肳��Ă��邩�H
		if (timeLog_.find(name) == timeLog_.end())
			return;

		//�J�E���g�I��
		timeLog_[name].TimeCountEnd(timeType);
		//�\��
		//TimerLogPrint(name, timeType, message);
	}
	//�J�E���g�I���@���b�Z�[�W�ȗ���
	void TimerLogEnd(string name, int timeType)
	{
		TimerLogEnd(name, timeType, name);
	}
	//�L�[�̂�
	void TimerLogEnd(string name)
	{
		TimerLogEnd(name, timeLog_[name].nameType_, name);
	}

	void TimerLogPrintAll()
	{
		for (const auto& itr : timeLog_)
		{
			string name = (itr).first;
			//�\��
			TimerLogPrint(name, timeLog_[name].nameType_, name);
		}
	}
};

#if 0
std::chrono::system_clock::time_point  start, end; // �^�� auto �ŉ�
start = std::chrono::system_clock::now(); // �v���J�n����
long long elapsed = 0;
string message = "";

#define chronoS start = std::chrono::system_clock::now();
#define chronoG end = std::chrono::system_clock::now();\
elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();\
message = "Debug::";\
message += std::to_string(elapsed);\
Debug::Log(message.c_str(), true);

chronoS;
static long long count3 = 0;
static long long time3 = 0;
static string ave3 = "";
XMStoreFloat3(&transform_.position_, acceleration_ + XMLoadFloat3(&transform_.position_));
Debug::Log("�ʒu�Ƀx�N�g���E�o�[�W�����R");
chronoG;
count3++;
time3 += elapsed;
ave3 = std::to_string((long double)time3 / (long double)count3);
ave3 += "/time:" + std::to_string(time3) + "/count:" + std::to_string(count3);
Debug::Log(ave3.c_str(), true);
Debug::Log("", true);
#endif