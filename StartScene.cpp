#include <algorithm>
#include <iostream>
#include <algorithm>

#include "Engine/SceneManager.h"
#include "StartScene.h"
#include "Music.h"
#include "Circuit.h"
#include "VehicleGlobal.h"
#include "VehicleInput.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Text.h"

using std::string;
using std::to_string;
using std::stoi;

//コンストラクタ
StartScene::StartScene(GameObject* parent)
	: GameObject(parent, "StartScene")
	, pTextCircuit_(nullptr), pTextCaption_(nullptr)
	, hImageArrow_(-1)
	, captionWidthOperand_(0.142f), captionHeight_(150), captionUpperHeight_(50)
	, sceneTitlePosition_({ 30.0f,30.0f })
	, countSpeed_(0.1f), arrwoBace_(-0.85f), sinOperand_(0.02f)
	, indexOperand_(-0.137f), indexUpper_(0.58f), indexLastUpper_(1.0f)
{
}

//初期化
void StartScene::Initialize()
{
	//コース初期化
	Circuit::Initialize();
	//車両パーツ読み込み初期化
	VehicleGlobal::Initialize();

	//文字
	pTextCircuit_ = new Text;
	pTextCircuit_->Initialize();
	pTextCaption_ = new Text;
	pTextCaption_->Initialize();

	//矢印
	hImageArrow_ = Image::Load("image\\arrow.png");
	assert(hImageArrow_ >= 0);

	//項目
	//コース
	dataSelection_[DataName::circuit].SetDataSelection("Circuit"
		, 0, 0, Circuit::GetCircuitNameArray()->size() - 1);
	assert(dataSelection_[DataName::circuit].maxValue >= 1);

	//参加人数
	dataSelection_[DataName::population].SetDataSelection("Population"
		, Circuit::GetChosenCircuit()->startTransform_.size(), 1, Circuit::GetChosenCircuit()->startTransform_.size());
	assert(dataSelection_[DataName::population].maxValue >= 1);

	//車両
	dataSelection_[DataName::vehicle].SetDataSelection("Vehicle"
		, 0, 0, VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::vehicle)->size() - 1);
	assert(dataSelection_[DataName::vehicle].maxValue >= 1);

	//タイヤ
	dataSelection_[DataName::wheel].SetDataSelection("Wheel"
		, 0, 0, VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::wheel)->size() - 1);
	assert(dataSelection_[DataName::wheel].maxValue >= 1);

	//索引
	selectIndex_.SetDataSelection("index", 0, 0, dataSelection_.size() - 1);
	assert(selectIndex_.maxValue >= 1);

	//決定項目
	selectIndex_.maxValue += 1;
}

//更新
void StartScene::Update()
{
	//操作
	VehicleInput::Update();

	//音楽
	Music::Update();

	XMFLOAT3  tes = Input::GetPadStickL();
	tes = Input::GetPadStickR();

	if (VehicleInput::GetInput(VehicleInput::Button::selectLeft))
	{
		if (dataSelection_[selectIndex_.index].DataAddition(-1))
		{
			Music::Play(Music::MusicName::se_select2);
		}
		else
		{
			Music::Play(Music::MusicName::se_se_selectError);
		}
	}
	if (VehicleInput::GetInput(VehicleInput::Button::selectRight))
	{
		if (dataSelection_[selectIndex_.index].DataAddition(1))
			Music::Play(Music::MusicName::se_select2);
		else
			Music::Play(Music::MusicName::se_se_selectError);
	}

	if (VehicleInput::GetInput(VehicleInput::Button::selectUp))
	{
		if(selectIndex_.DataAddition(-1))
			Music::Play(Music::MusicName::se_select);
		else
			Music::Play(Music::MusicName::se_se_selectError);
	}
	if (VehicleInput::GetInput(VehicleInput::Button::selectDown))
	{
		if (selectIndex_.DataAddition(1))
			Music::Play(Music::MusicName::se_select);
		else
			Music::Play(Music::MusicName::se_se_selectError);
	}


	if(VehicleInput::GetInput(VehicleInput::Button::selectDecide))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

//描画
void StartScene::Draw()
{
	pTextCircuit_->Draw(sceneTitlePosition_.x, sceneTitlePosition_.y, "[Select Menu]");

	int width = Global::GetScreenWidth() * captionWidthOperand_;
	int count = 0;

	//コース
	PrintParagraph(DataName::circuit, Circuit::GetCircuitNameArray()->at(dataSelection_[DataName::circuit].index)
		, width, captionHeight_ + captionUpperHeight_ * count++);
	Circuit::SetChosenCircuit(dataSelection_[DataName::circuit].index);	//選択

	//人数
	dataSelection_[DataName::population].maxValue = Circuit::GetChosenCircuit()->startTransform_.size();
	PrintParagraph(DataName::population, to_string(dataSelection_[DataName::population].index), width, captionHeight_ + captionUpperHeight_ * count++);
	VehicleGlobal::SetChosenPopulation(dataSelection_[DataName::population].index);	//選択

	//車両
	PrintParagraph(DataName::vehicle, VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::vehicle)->
		at(dataSelection_[DataName::vehicle].index).first, width, captionHeight_ + captionUpperHeight_ * count++);
	VehicleGlobal::SetChosenVehicleName(VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::vehicle)->
		at(dataSelection_[DataName::vehicle].index).second);	//選択

	//タイヤ
	PrintParagraph(DataName::wheel, VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::wheel)->
		at(dataSelection_[DataName::wheel].index).first, width, captionHeight_ + captionUpperHeight_ * count++);
	VehicleGlobal::SetChosenWheelName(VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::wheel)->
		at(dataSelection_[DataName::wheel].index).second);	//選択

	//決定
	pTextCircuit_->Draw(width, captionHeight_ + captionUpperHeight_ * ++count, "[Start]");//ここだけ離れた位置

	{
		//秒数カウント
		static float count = 0.0f;
		count += countSpeed_;

		float bace = arrwoBace_;
		float pos = (sin(count) * sinOperand_) + bace;

		Transform arrowTrans;

		//選択の索引が最後なら離れた位置にする
		float height = (selectIndex_.index * indexOperand_);
		if(selectIndex_.index >= selectIndex_.maxValue)
			height = ((selectIndex_.index + indexLastUpper_) * indexOperand_);

		arrowTrans.position_ = { pos , height + indexUpper_, 0.0f };

		Image::SetTransform(hImageArrow_, arrowTrans);
		Image::Draw(hImageArrow_);
	}
}

//開放
void StartScene::Release()
{
	pTextCircuit_->Release();
	pTextCaption_->Release();
}

//項目の設定と表示
void StartScene::PrintParagraph(DataName dn, std::string str, int width, int height)
{
	dataSelection_[dn].DataClamp();
	string nameStr = str;

	//矢印表示
	dataSelection_[dn].PrintArrowLR(&nameStr);

	nameStr = dataSelection_[dn].title + " : " + nameStr;

	pTextCircuit_->Draw(width, height, nameStr.c_str());
}