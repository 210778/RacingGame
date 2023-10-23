#include "Engine/SceneManager.h"
#include "StartScene.h"
#include "Music.h"
#include "Circuit.h"

#include "Engine/Input.h"
#include "Engine/Text.h"

using std::string;
using std::to_string;
using std::stoi;

//�R���X�g���N�^
StartScene::StartScene(GameObject* parent)
	: GameObject(parent, "StartScene"),
	pTextCircuit_(nullptr)
{
}

//������
void StartScene::Initialize()
{
	Circuit::Initialize();

	//����
	pTextCircuit_ = new Text;
	pTextCircuit_->Initialize();
}

//�X�V
void StartScene::Update()
{
	Circuit::SetChosenCircuit(0);

	//���y
	Music::Update();

	if(true || Input::IsKey(DIK_P))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

//�`��
void StartScene::Draw()
{
	int height = 30;

	for (auto& itr : *Circuit::GetCircuitNameArray())
	{
		pTextCircuit_->Draw(Global::GetScreenWidth() / 2, height, itr.c_str());

		height += 40;
	}

}

//�J��
void StartScene::Release()
{
	pTextCircuit_->Release();

}