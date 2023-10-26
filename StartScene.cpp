#include <algorithm>
#include <iostream>
#include <algorithm>

#include "Engine/SceneManager.h"
#include "StartScene.h"
#include "Music.h"
#include "Circuit.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Text.h"

using std::string;
using std::to_string;
using std::stoi;

//�R���X�g���N�^
StartScene::StartScene(GameObject* parent)
	: GameObject(parent, "StartScene")
	, pTextCircuit_(nullptr), circuitIndex_(0)
	, hImageArrow_(-1)
	, chosenCircuit_(0)
{
}

//������
void StartScene::Initialize()
{
	Circuit::Initialize();

	//����
	pTextCircuit_ = new Text;
	pTextCircuit_->Initialize();

	hImageArrow_ = Image::Load("image\\arrow.png");

	//����
	circuitSelect_.maxValue = Circuit::GetCircuitNameArray()->size() - 1;
	circuitSelect_.minValue = 0;
}

//�X�V
void StartScene::Update()
{
	//���y
	Music::Update();

	bool test = false;
	if (Input::IsKeyDown(DIK_A))
	{
		test = circuitSelect_.DataAddition(-1);
	}
	if (Input::IsKeyDown(DIK_D))
	{
		test = circuitSelect_.DataAddition(1);
	}

	if(Input::IsKey(DIK_P))
	{
		circuitSelect_.DataClamp();
		Circuit::SetChosenCircuit(circuitSelect_.index);

		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

//�`��
void StartScene::Draw()
{
	int height = 150;

	{		
		if (Circuit::GetCircuitNameArray()->size() < 0)
			return;

		circuitSelect_.DataClamp();
		string circuitName = Circuit::GetCircuitNameArray()->at(circuitSelect_.index);

		circuitName = (circuitSelect_.index > circuitSelect_.minValue) ? "< " + circuitName : "  " + circuitName;
		circuitName = (circuitSelect_.index < circuitSelect_.maxValue) ? circuitName + " >" : circuitName + "  ";

		pTextCircuit_->Draw(Global::GetScreenWidth() / 2, height, circuitName.c_str());
	}

	Image::SetTransform(hImageArrow_, transform_);
	Image::Draw(hImageArrow_);
}

//�J��
void StartScene::Release()
{
	pTextCircuit_->Release();

}