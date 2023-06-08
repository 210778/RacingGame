#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

#include "PlayScene.h"
#include "Sample.h"
#include "Vehicle.h"
#include "Ground.h"
#include "ParticlePackage.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hImage_(-1), hModel_(-1)
{
}

//������
void PlayScene::Initialize()
{
	hImage_ = Image::Load("image\\grass-pattern-set\\grass01.jpg");
	assert(hImage_ >= 0);
	
	hModel_ = Model::Load("model\\SkyFieldSphere.fbx");
	//hModel_ = Model::Load("model\\CarRed2.fbx");
	assert(hModel_ >= 0);

	Ground *pGround = Instantiate<Ground>(this);
	//pGround->SetChosenCircuit(0);
	pGround->SetChosenCircuitCheckPoint(0);

	//�G�t�F�N�g�p
	ParticlePackage::Initialize();

	//Vehicle *pVehicle = Instantiate<Vehicle>(this);
	Instantiate<Vehicle>(this);
}

//�X�V
void PlayScene::Update()
{
	if (Input::IsKeyUp(DIK_G))
	{
	}
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