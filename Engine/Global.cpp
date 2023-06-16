#include "Global.h"

namespace Global
{
	//初期化ファイルから取得されるはずの必要な情報
	int screenWidth_;		//スクリーンの幅
	int screenHeight_;		//スクリーンの高さ
	int fpsLimit_;			//FPS（画面更新速度）
	int isDrawFps_;			//キャプションに現在のFPSを表示するかどうか

	int currentFPS_;	//現在のFPS
};

//スクリーンの幅
void Global::SetScreenWidth(int value)
{
	screenWidth_ = value;
}
int Global::GetScreenWidth()
{
	return screenWidth_;
}

//スクリーンの高さ
void Global::SetScreenHeight(int value)
{
	screenHeight_ = value;
}
int Global::GetScreenHeight()
{
	return screenHeight_;
}

//FPS（画面更新速度）
void Global::SetFpsLimit(int value)
{
	fpsLimit_ = value;
}
int Global::GetFpsLimit()
{
	return fpsLimit_;
}

//キャプションに現在のFPSを表示するかどうか
void Global::SetIsDrawFps(bool value)
{
	isDrawFps_ = value;
}
bool Global::GetIsDrawFps()
{
	return isDrawFps_;
}

//現在のFPS
void Global::SetCurrentFPS(int value)
{
	currentFPS_ = value;
}
//現在のFPS
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
	//実行時間を測定して平均を求めたい
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
		//カウント
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
		//対応する番号がないなら新しく作ってその番号をわたす
		if (n < 0 && n >= timeLog_.size())
		{
			TimeLogClass a(1);
			timeLog_.push_back(a);
			return timeLog_.size() - 1;
		}
		else
		{
			//適当
			return n;
		}

		for (auto& i : timeLog_)
		{

		}
	}

};