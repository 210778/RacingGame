#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

#include "PlayScene.h"
#include "Sample.h"
#include "Vehicle.h"
#include "Ground.h"
#include "ParticlePackage.h"
#include "VehiclePlayer.h"
#include "VehicleOpponent.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hImage_(-1), hModel_(-1)
	,pGround_(nullptr)
{
}

//������
void PlayScene::Initialize()
{
	hImage_ = Image::Load("image\\skyField.png");
	assert(hImage_ >= 0);
	
	hModel_ = Model::Load("model\\SkyFieldSphere.fbx");
	//hModel_ = Model::Load("model\\CarRed2.fbx");
	assert(hModel_ >= 0);

	pGround_ = Instantiate<Ground>(this);
	pGround_->SetChosenCircuit(0);

	//�G�t�F�N�g�p
	ParticlePackage::Initialize();

	float distans = -180;



	int population = 3;
	int playerNumber = 1;

	for (int i = 0; i < population; i++)
	{
		if (i == playerNumber)
		{
			VehiclePlayer* pVehiclePlayer
				= VehicleInstantiate<VehiclePlayer>(this, "model\\Car01_blue.fbx", "model\\wheel01.fbx");
			pVehiclePlayer->SetPosition(pGround_->GetCircuitUnion()->startTransform_[i].position_);
			pVehiclePlayer->SetRotate(pGround_->GetCircuitUnion()->startTransform_[i].rotate_);
			pVehiclePlayer->SetStartTransform(pGround_->GetCircuitUnion()->startTransform_[i]);
		}
		else
		{
			VehicleOpponent* pVehicleOpponent
				= VehicleInstantiate<VehicleOpponent>(this, "model\\Car01_red.fbx", "model\\wheel01.fbx");
			pVehicleOpponent->SetPosition(pGround_->GetCircuitUnion()->startTransform_[i].position_);
			pVehicleOpponent->SetRotate(pGround_->GetCircuitUnion()->startTransform_[i].rotate_);
			pVehicleOpponent->SetStartTransform(pGround_->GetCircuitUnion()->startTransform_[i]);
		}
	}
}

//�X�V
void PlayScene::Update()
{
}

//�`��
void PlayScene::Draw()
{
#if 1
		Transform ImaTra = transform_;
		ImaTra.scale_ = XMFLOAT3(0.25f, 0.25f, 0.25f);
		Image::SetTransform(hImage_, transform_);
		Image::Draw(hImage_);
#else
		Transform modTra = transform_;
		modTra.scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
		modTra.rotate_.y = 180;
		Model::SetTransform(hModel_, modTra);
		Model::Draw(hModel_);
#endif
	
}

//�J��
void PlayScene::Release()
{
}

//�ԗ��N���X�̂��߂̏�����
template <class V>
V* PlayScene::VehicleInstantiate(GameObject* pParent, std::string vehicleName, std::string wheelName)
{
	V* pNewObject = new V(pParent, vehicleName, wheelName);
	if (pParent != nullptr)
	{
		pParent->PushBackChild(pNewObject);
	}
	pNewObject->Initialize();
	return pNewObject;
}