#include <algorithm>
#include <iostream>
#include <algorithm>

#include "Engine/SceneManager.h"
#include "StartScene.h"
#include "Music.h"
#include "Circuit.h"
#include "VehicleGlobal.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Text.h"

using std::string;
using std::to_string;
using std::stoi;

//�R���X�g���N�^
StartScene::StartScene(GameObject* parent)
	: GameObject(parent, "StartScene")
	, pTextCircuit_(nullptr), pTextCaption_(nullptr)
	, hImageArrow_(-1)
{
}

//������
void StartScene::Initialize()
{
	//�R�[�X������
	Circuit::Initialize();
	//�ԗ��p�[�c�ǂݍ��ݏ�����
	VehicleGlobal::Initialize();

	//����
	pTextCircuit_ = new Text;
	pTextCircuit_->Initialize();
	pTextCaption_ = new Text;
	pTextCaption_->Initialize();

	//���
	hImageArrow_ = Image::Load("image\\arrow.png");
	assert(hImageArrow_ >= 0);

	//����
	//�R�[�X
	dataSelection_[DataName::circuit].SetDataSelection("Circuit"
		, 0, 0, Circuit::GetCircuitNameArray()->size() - 1);
	assert(dataSelection_[DataName::circuit].maxValue >= 1);

	//�Q���l��
	dataSelection_[DataName::population].SetDataSelection("Population"
		, Circuit::GetChosenCircuit()->startTransform_.size(), 1, Circuit::GetChosenCircuit()->startTransform_.size());
	assert(dataSelection_[DataName::population].maxValue >= 1);

	//�ԗ�
	dataSelection_[DataName::vehicle].SetDataSelection("Vehicle"
		, 0, 0, VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::vehicle)->size());
	assert(dataSelection_[DataName::vehicle].maxValue >= 1);

	//�^�C��
	dataSelection_[DataName::wheel].SetDataSelection("Wheel"
		, 0, 0, VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::wheel)->size());
	assert(dataSelection_[DataName::wheel].maxValue >= 1);

	//����
	selectIndex_.SetDataSelection("index", 0, 0, dataSelection_.size() - 1);
	assert(selectIndex_.maxValue >= 1);

}

//�X�V
void StartScene::Update()
{
	//���y
	Music::Update();


	if (Input::IsKeyDown(DIK_A))
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
	if (Input::IsKeyDown(DIK_D))
	{
		if (dataSelection_[selectIndex_.index].DataAddition(1))
			Music::Play(Music::MusicName::se_select2);
		else
			Music::Play(Music::MusicName::se_se_selectError);
	}

	if (Input::IsKeyDown(DIK_W))
	{
		if(selectIndex_.DataAddition(-1))
			Music::Play(Music::MusicName::se_select);
		else
			Music::Play(Music::MusicName::se_se_selectError);
	}
	if (Input::IsKeyDown(DIK_S))
	{
		if (selectIndex_.DataAddition(1))
			Music::Play(Music::MusicName::se_select);
		else
			Music::Play(Music::MusicName::se_se_selectError);
	}


	if(Input::IsKey(DIK_P))
	{
		//circuitSelect_.DataClamp();
		//Circuit::SetChosenCircuit(circuitSelect_.index);

		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

//�`��
void StartScene::Draw()
{
	pTextCircuit_->Draw(30, 30, "Select Menu");

	int height = 150;

	{		
		dataSelection_[DataName::circuit].DataClamp();
		//�I��
		Circuit::SetChosenCircuit(dataSelection_[DataName::circuit].index);

		string circuitName = Circuit::GetCircuitNameArray()->at(dataSelection_[DataName::circuit].index);

		//���\��
		dataSelection_[DataName::circuit].PrintArrowLR(&circuitName);

		circuitName = dataSelection_[DataName::circuit].title + " : " + circuitName;

		pTextCircuit_->Draw(Global::GetScreenWidth() / 2, height, circuitName.c_str());
	}

	{
		dataSelection_[DataName::population].maxValue = Circuit::GetChosenCircuit()->startTransform_.size();
		dataSelection_[DataName::population].DataClamp();
		string popStr = to_string(dataSelection_[DataName::population].index);

		//���\��
		dataSelection_[DataName::population].PrintArrowLR(&popStr);

		popStr = dataSelection_[DataName::population].title + " : " + popStr;

		pTextCircuit_->Draw(Global::GetScreenWidth() / 2, height + 50, popStr.c_str());
	}
	//�ԗ�
	{
		dataSelection_[DataName::vehicle].DataClamp();

		string nameStr = VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::vehicle)->
			at(dataSelection_[DataName::vehicle].index).first;

		//���\��
		dataSelection_[DataName::vehicle].PrintArrowLR(&nameStr);
		nameStr = dataSelection_[DataName::vehicle].title + " : " + nameStr;
		pTextCircuit_->Draw(Global::GetScreenWidth() / 2, height + 100, nameStr.c_str());
	}
	//�^�C��
	{
		dataSelection_[DataName::wheel].DataClamp();
		string nameStr = VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::wheel)->
			at(dataSelection_[DataName::wheel].index).first;
		//���\��
		dataSelection_[DataName::wheel].PrintArrowLR(&nameStr);
		nameStr = dataSelection_[DataName::wheel].title + " : " + nameStr;
		pTextCircuit_->Draw(Global::GetScreenWidth() / 2, height + 150, nameStr.c_str());
	}

	Transform arrowTrans;
	arrowTrans.position_ = { -0.15f , (selectIndex_.index * -0.18f) + 0.58f, 0.0f };

	Image::SetTransform(hImageArrow_, arrowTrans);
	Image::Draw(hImageArrow_);
}

//�J��
void StartScene::Release()
{
	pTextCircuit_->Release();
	pTextCaption_->Release();
}