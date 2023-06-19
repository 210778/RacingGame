#pragma once

#include <string>

namespace Debug
{
	void Log(int value, bool isLineFeed = false);
	void Log(float value, bool isLineFeed = false);
	void Log(std::string value, bool isLineFeed = false);

	//�\�����鎞�Ԃ̒P��
	enum TimeType { _nanoseconds, _microseconds, _milliseconds, _seconds, _minutes, _hours };
	//���Ԍv���J�n�@name:�L�[
	void TimerLogStart(std::string name);
	//���Ԍv���I���@name:�L�[�@timeType:���Ԃ̒P�ʁ@message:�\��������������
	void TimerLogEnd(std::string name, int timeType, std::string message);
	//���Ԍv���I���@name:�L�[�A�\��������������@timeType:���Ԃ̒P��
	void TimerLogEnd(std::string name, int timeType);
	//���Ԍv���I���@name:�L�[�A�\��������������@���Ԃ̒P�ʂ̓~���b
	void TimerLogEnd(std::string name);
};

