#include "Global.h"

namespace Global
{
	//�������t�@�C������擾�����͂��̕K�v�ȏ��
	int screenWidth_;		//�X�N���[���̕�
	int screenHeight_;		//�X�N���[���̍���
	int fpsLimit_;			//FPS�i��ʍX�V���x�j
	int isDrawFps_;			//�L���v�V�����Ɍ��݂�FPS��\�����邩�ǂ���

	int currentFPS_;	//���݂�FPS
};

//�X�N���[���̕�
void Global::SetScreenWidth(int value)
{
	screenWidth_ = value;
}
int Global::GetScreenWidth()
{
	return screenWidth_;
}

//�X�N���[���̍���
void Global::SetScreenHeight(int value)
{
	screenHeight_ = value;
}
int Global::GetScreenHeight()
{
	return screenHeight_;
}

//FPS�i��ʍX�V���x�j
void Global::SetFpsLimit(int value)
{
	fpsLimit_ = value;
}
int Global::GetFpsLimit()
{
	return fpsLimit_;
}

//�L���v�V�����Ɍ��݂�FPS��\�����邩�ǂ���
void Global::SetIsDrawFps(bool value)
{
	isDrawFps_ = value;
}
bool Global::GetIsDrawFps()
{
	return isDrawFps_;
}

//���݂�FPS
void Global::SetCurrentFPS(int value)
{
	currentFPS_ = value;
}
//���݂�FPS
int Global::GetCurrentFPS()
{
	return currentFPS_;
}

#include <list>
#include<vector>
#include <chrono>
#include <string>
using std::list;
using std::vector;
using std::string;
namespace TimeLog
{
	//���s���Ԃ𑪒肵�ĕ��ς����߂���
	class TimeLogClass
	{
		std::string name_;
		int number_;
		unsigned long long count_;
		unsigned long long time_;
		long double average_;
	public:
		TimeLogClass(int n) { name_ = "XXX"; number_ = n; count_ = 0; time_ = 0; average_ = 0.0l; };
		~TimeLogClass() {};
		//�J�E���g
		void TimeCount(unsigned long long addTime)
		{
			count_++;
			time_ += addTime;
			average_ = average_ + (addTime - average_) / count_;
		}

	};

	list<TimeLogClass> timeLog_;

	int TimeLog::StartTimerLog(int n)
	{
		//�Ή�����ԍ����Ȃ��Ȃ�V��������Ă��̔ԍ����킽��
		if (n < 0 && n >= timeLog_.size())
		{
			TimeLogClass a(1);
			timeLog_.push_back(a);
			return timeLog_.size() - 1;
		}
		else
		{
			//�K��
			return n;
		}

		for (auto& i : timeLog_)
		{

		}
	}

};