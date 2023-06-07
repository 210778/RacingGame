#pragma once
#include "Engine/Particle.h"
//#include "Engine/GameObject.h"
class Particle;

namespace ParticlePackage
{
    //�������i�f�[�^�Z�b�g�j
    void Initialize();

    //�G�t�F�N�g�N���֐�

    /// <summary>
    /// �u�[�X�^�[�G�t�F�N�g�N��
    /// </summary>
    /// <param name="pParticle">�p�[�e�B�N���^�|�C���^</param>
    /// <param name="position">�ʒu</param>
    /// <param name="direction">����</param>
    void ActBooster(Particle* pParticle, XMFLOAT3 position, XMVECTOR direction);
};
#if 0
//�p�[�e�B�N�����܂Ƃ߂邽�߂̃N���X
class ParticlePackage
{
    unsigned long long timeCount_;
    Particle* pParticle_;   //�|�C���^

	EmitterData boosterFire_;
	EmitterData boosterSpark_;

	EmitterData rainbowFire_;
	EmitterData rainbowSpark_;

	EmitterData airCloud_;

	ParticlePackage()
	{
        //��
        boosterFire_.textureFileName = "image\\PaticleAssets\\circle_W.png";
        boosterFire_.position = { 0.0f,0.0f,0.0f };//
        //boosterFire_.position = Model::GetBonePosition(hModel_, "car1_rear");
        //boosterFire_.position.y += Size.toCenter_;
        boosterFire_.positionErr = { 0.2f, 0.2f, 0.2f };
        boosterFire_.delay = 0;
        boosterFire_.number = 10;
        boosterFire_.lifeTime = 30.0f;
        boosterFire_.gravity = 0.0f;
        boosterFire_.dir = { 0.0f,0.0f,0.0f };//
        //XMStoreFloat3(&boosterFire_.dir, -vecZ);
        boosterFire_.dirErr = { 50.0f, 50.0f, 50.0f };
        boosterFire_.speed = 0.1f;
        boosterFire_.speedErr = 0.0f;
        boosterFire_.size = { 0.8f, 0.8f };
        boosterFire_.sizeErr = { 0.1f, 0.1f };
        boosterFire_.scale = { 0.98f, 0.98f };
        boosterFire_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        boosterFire_.deltaColor = { 0.0f, -0.06f, -0.12f, -0.05f };
        //pParticle_->Start(boosterFire_);
        // 
        //�΂̕�
        boosterSpark_.textureFileName = "image\\PaticleAssets\\circle_W.png";
        boosterSpark_.position = { 0.0f,0.0f,0.0f };//
        boosterSpark_.positionErr = { 0.5f, 0.5f, 0.5f };
        boosterSpark_.delay = 0;
        boosterSpark_.number = 2;
        boosterSpark_.lifeTime = 40.0f;
        boosterSpark_.gravity = 0.0f;
        boosterSpark_.dir = { 0.0f,0.0f,0.0f };//
        boosterSpark_.dirErr = { 90.0f, 90.0f, 90.0f };
        boosterSpark_.speed = 0.1f;
        boosterSpark_.speedErr = 0.0f;
        boosterSpark_.size = { 0.1f, 0.1f };
        boosterSpark_.scale = { 1.0f, 1.0f };
        boosterFire_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        boosterSpark_.deltaColor = { -0.02f, -0.02f, -0.1f, -0.01f };
        //pParticle_->Start(boosterSpark_);
	}
	~ParticlePackage()
	{
	}
public:
    /// <summary>
    /// �u�[�X�^�[�G�t�F�N�g�N��
    /// </summary>
    /// <param name="pParticle">�p�[�e�B�N���^�|�C���^</param>
    /// <param name="position">�ʒu</param>
    /// <param name="direction">����</param>
    void ActBooster(Particle* pParticle,XMFLOAT3 position, XMVECTOR direction);

    //�Z�b�^�[�ƃQ�b�^�[
    void SetParticlePosition(float)
    {
        ;
    }
};
#endif
