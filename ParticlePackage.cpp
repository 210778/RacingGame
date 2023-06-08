#include "ParticlePackage.h"

namespace ParticlePackage
{
	//変数
	EmitterData boosterFire_;
	EmitterData boosterSpark_;

	EmitterData rainbowFire_;
	EmitterData rainbowSpark_;

	EmitterData smokeCloud_;

	unsigned long long timeCount_;	//時間のカウント
    const int colorSpeed_ = 2;  //カウントのスピード
    const float operand = 0.0027778f; // 1 / 360

    //セッター
	void SetBooster();
    void SetRainbow();
    void SetSmoke();
};

void ParticlePackage::Initialize()
{
	timeCount_ = 0;

	SetBooster();
    SetRainbow();
    SetSmoke();
}

void ParticlePackage::ActBooster(Particle* pParticle, XMFLOAT3 position, XMVECTOR direction)
{
    boosterFire_.position = position;
    XMStoreFloat3(&boosterFire_.dir, direction);
    pParticle->Start(boosterFire_);

    boosterSpark_.position = position;
    XMStoreFloat3(&boosterSpark_.dir, direction);
    pParticle->Start(boosterSpark_);
}

void ParticlePackage::ActRainbowFire(Particle* pParticle, XMFLOAT3 position)
{
    timeCount_ += colorSpeed_;
    XMFLOAT4 HSV((timeCount_ % 360) * operand, 1.0, 1.0, 0.7);//色相、彩度、明度、アルファ
    XMVECTOR vecRainbowRGB = XMColorHSVToRGB(XMLoadFloat4(&HSV));
    XMFLOAT4 rainbowRGB;
    XMStoreFloat4(&rainbowRGB, vecRainbowRGB);

    rainbowFire_.position = position;
    rainbowFire_.color = rainbowRGB;
    pParticle->Start(rainbowFire_);

    rainbowSpark_.position = position;
    rainbowSpark_.color = rainbowRGB;
    pParticle->Start(rainbowSpark_);
}

void ParticlePackage::ActSmokeCloud(Particle* pParticle, XMFLOAT3 position)
{
    smokeCloud_.position = position;
    pParticle->Start(smokeCloud_);
}

void ParticlePackage::SetBooster()
{
    //ジェット炎
    boosterFire_.textureFileName = "image\\PaticleAssets\\circle_W.png";
    boosterFire_.position = { 0.0f,0.0f,0.0f };
    boosterFire_.positionErr = { 0.2f, 0.2f, 0.2f };
    boosterFire_.delay = 0;
    boosterFire_.number = 10;
    boosterFire_.lifeTime = 30.0f;
    boosterFire_.gravity = 0.0f;
    boosterFire_.dir = { 0.0f,0.0f,0.0f };
    boosterFire_.dirErr = { 50.0f, 50.0f, 50.0f };
    boosterFire_.speed = 0.1f;
    boosterFire_.speedErr = 0.0f;
    boosterFire_.size = { 0.8f, 0.8f };
    boosterFire_.sizeErr = { 0.1f, 0.1f };
    boosterFire_.scale = { 0.98f, 0.98f };
    boosterFire_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    boosterFire_.deltaColor = { 0.0f, -0.06f, -0.12f, -0.05f };
    //ジェット火花
    boosterSpark_.textureFileName = "image\\PaticleAssets\\circle_W.png";
    boosterSpark_.position = { 0.0f,0.0f,0.0f };
    boosterSpark_.positionErr = { 0.5f, 0.5f, 0.5f };
    boosterSpark_.delay = 0;
    boosterSpark_.number = 2;
    boosterSpark_.lifeTime = 40.0f;
    boosterSpark_.gravity = 0.0f;
    boosterSpark_.dir = { 0.0f,0.0f,0.0f };
    boosterSpark_.dirErr = { 90.0f, 90.0f, 90.0f };
    boosterSpark_.speed = 0.1f;
    boosterSpark_.speedErr = 0.0f;
    boosterSpark_.size = { 0.1f, 0.1f };
    boosterSpark_.scale = { 1.0f, 1.0f };
    boosterSpark_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    boosterSpark_.deltaColor = { -0.02f, -0.02f, -0.1f, -0.01f };
}

void ParticlePackage::SetRainbow()
{
    //虹炎
    rainbowFire_.textureFileName = "image\\PaticleAssets\\cloudB.png";
    rainbowFire_.position = { 0.0f,0.0f,0.0f };
    rainbowFire_.positionErr = { 0.2f,0.0f,0.2f };
    rainbowFire_.delay = 0;
    rainbowFire_.number = 1;
    rainbowFire_.lifeTime = 15.0f;
    rainbowFire_.gravity = 0.0f;
    rainbowFire_.dir = { 0.0f,1.0f,0.0f };
    rainbowFire_.dirErr = { 15.0f,0.0f,15.0f };
    rainbowFire_.speed = 0.75f;
    rainbowFire_.speedErr = 0.2f;
    rainbowFire_.size = { 6.0f, 6.0f };
    rainbowFire_.sizeErr = { 0.5f, 0.5f };
    rainbowFire_.scale = { 0.8f, 0.8f };;
    rainbowFire_.color = { 0.0f,0.0f,0.0f,0.0f };
    rainbowFire_.deltaColor = { 0.0f,0.0f,0.0f,-0.01f };

    //虹火花
    rainbowSpark_ = rainbowFire_;
    rainbowSpark_.number = 1;
    rainbowSpark_.positionErr = { 0.8f, 0.0f, 0.8f };
    rainbowSpark_.dir = { 0.0f, 1.0f, 0.0f };
    rainbowSpark_.dirErr = { 10.0f, 10.0f, 10.0f };
    rainbowSpark_.size = { 0.5f, 0.5f };
    rainbowSpark_.scale = { 0.98f, 0.98f };
    rainbowSpark_.lifeTime = 30.0f;
    rainbowSpark_.speed = 0.2f;
    rainbowSpark_.gravity = -0.002f;
}

void ParticlePackage::SetSmoke()
{
    //煙
    smokeCloud_.textureFileName = "image\\PaticleAssets\\cloudA.png";
    smokeCloud_.position = { 0.0f,0.0f,0.0f };
    smokeCloud_.positionErr = { 0.0f,0.0f,0.0f };
    smokeCloud_.delay = 0;
    smokeCloud_.number = 1;
    smokeCloud_.lifeTime = 40;
    smokeCloud_.gravity = 0.0f;
    smokeCloud_.dir = { 0.0f,0.0f,0.0f };
    smokeCloud_.dirErr = { 50.0f,50.0f,50.0f };
    smokeCloud_.speed = 0.0f;
    smokeCloud_.speedErr = 0.0f;
    smokeCloud_.size = { 0.5f,0.5f };
    smokeCloud_.sizeErr = { 0.0f,0.0f };
    smokeCloud_.scale = { 1.0f,1.0f };
    smokeCloud_.color = { 1.0f,1.0f,1.0f,0.04 };
    smokeCloud_.deltaColor = { 0.0f,0.0f,0.0f,-0.003f };
}