#include <unordered_map>
#include "ParticlePackage.h"

namespace ParticlePackage
{
	//�p�[�e�B�N���܂Ƃ�
	EmitterData boosterFire_;   //�΂P
	EmitterData boosterSpark_;  //�΂Q

	EmitterData rainbowFire_;   //���P
	EmitterData rainbowSpark_;  //���Q

	EmitterData smokeCloud_;    //���s��

    EmitterData landingGrass_;  //��

    EmitterData landingDirt_;   //��

    EmitterData wheelSpark_;    //�Ή�


    std::unordered_map<ParticleName, std::vector<EmitterData>> emitter_;

	unsigned long long timeCount_;	//���Ԃ̃J�E���g
    int colorSpeed_ = 2;  //�J�E���g�̃X�s�[�h

    //�Z�b�^�[
	void SetBooster();
    void SetRainbow();
    void SetSmoke();
    void SetGrass();
    void SetDirt();
    void SetSpark();

    //�Z�b�g�p
    void SetParticle(ParticleName key, std::string fileName, XMFLOAT3 position
        , XMFLOAT3 positionErr, int delay, int mass, float lifeTime, float gravity, XMFLOAT3 dir
        , XMFLOAT3 dirErr, float speed, float speedErr, float accel, XMFLOAT2 size, XMFLOAT2 sizeErr
        , XMFLOAT2 scale, XMFLOAT4 colorRGBA, XMFLOAT4 deltaColorRGBA);
    //�Z�b�g���ď�����
    void ParticleInitialize();
};

void ParticlePackage::Initialize()
{
	timeCount_ = 0;

	SetBooster();
    SetRainbow();
    SetSmoke();
    SetGrass();
    SetDirt();
    SetSpark();

    //������
    ParticleInitialize();
}

/// <summary>
/// �G�t�F�N�g�Z�b�g�p
/// </summary>
/// <param name="key">�G�t�F�N�g�̖��O�^</param>
/// <param name="fileName">�摜�t�@�C����</param>
/// <param name="position">�ʒu</param>
/// <param name="positionErr">�ʒu�̌덷</param>
/// <param name="delay">���t���[�������Ƀp�[�e�B�N���𔭐������邩</param>
/// <param name="mass">1�x�ɏo���p�[�e�B�N����</param>
/// <param name="lifeTime">�p�[�e�B�N���̎����i�t���[�����j</param>
/// <param name="gravity">�d��</param>
/// <param name="dir">�p�[�e�B�N���̈ړ�����</param>
/// <param name="dirErr">�ړ������̌덷�i�e���̊p�x�j</param>
/// <param name="speed">1�t���[���̑��x</param>
/// <param name="speedErr">���x�덷�i0�`1�j</param>
/// <param name="accel">�����x</param>
/// <param name="size">�T�C�Y</param>
/// <param name="sizeErr">�T�C�Y�덷�i0�`1�j</param>
/// <param name="scale">1�t���[���̊g�嗦</param>
/// <param name="colorRGBA">�F�iRGBA 0�`1�j</param>
/// <param name="deltaColorRGBA">�F�̕ω���</param>
void ParticlePackage::SetParticle(ParticleName key, std::string fileName, XMFLOAT3 position
    , XMFLOAT3 positionErr, int delay, int mass, float lifeTime, float gravity, XMFLOAT3 dir
    , XMFLOAT3 dirErr, float speed, float speedErr, float accel, XMFLOAT2 size, XMFLOAT2 sizeErr
    , XMFLOAT2 scale, XMFLOAT4 colorRGBA, XMFLOAT4 deltaColorRGBA)
{
    EmitterData ed;

    ed.textureFileName = fileName;
    ed.position = position;
    ed.positionErr = positionErr;
    ed.delay = delay;
    ed.number = mass;
    ed.lifeTime = lifeTime;
    ed.gravity = gravity;
    ed.dir = dir;
    ed.dirErr = dirErr;
    ed.speed = speed;
    ed.speedErr = speedErr;
    ed.accel = accel;
    ed.size = size;
    ed.sizeErr = sizeErr;
    ed.scale = scale;
    ed.color = colorRGBA;
    ed.deltaColor = deltaColorRGBA;

    emitter_[key].push_back(ed);
}

void ParticlePackage::ParticleInitialize()
{
    //�u�[�X�g��
    SetParticle(ParticleName::boost, "image\\PaticleAssets\\circle_W.png", { 0.0f,0.0f,0.0f }
    , { 0.0f,0.0f,0.0f }, 0, 5, 30.0f, 0.0f, {0.0f,0.0f,0.0f}, {50.0f,50.0f,50.0f}, 0.1f, 0.0f, 1.0f
    , {0.8f,0.8f}, {0.1f,0.1f}, {0.98f,0.98f}, {1.0f,1.0f,1.0f,1.0f}, { 0.0f, -0.06f, -0.12f, -0.05f });
        //�Ή�
    SetParticle(ParticleName::boost, "image\\PaticleAssets\\circle_W.png", { 0.0f,0.0f,0.0f }
    , { 0.5f,0.5f,0.5f }, 0, 1, 40.0f, 0.0f, { 0.0f,0.0f,0.0f }, { 90.0f, 90.0f, 90.0f }, 0.1f, 0.0f, 1.0f
    , {0.1f,0.1f}, {0.1f,0.1f}, {1.0f,1.0f}, {1.0f,1.0f,1.0f,1.0f}, { -0.02f, -0.02f, -0.1f, -0.01f });

    //��
    SetParticle(ParticleName::smoke, "image\\PaticleAssets\\cloudA.png", { 0.0f,0.0f,0.0f }
    , { 0.0f,0.0f,0.0f }, 0, 1, 40, 0.0f, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.0f, 0.0f, 1.0f
    , { 0.5f,0.5f }, { 0.0f,0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,0.1f }, { 0.0f,0.0f,0.0f,-0.005f });

    //���n
    SetParticle(ParticleName::grass, "image\\PaticleAssets\\flashA_W.png", { 0.0f,0.0f,0.0f }
    , { 0.5f,0.5f,0.5f }, 0, 1, 30, 0.01f, { 0.0f,1.0f,0.0f }, { 50.0f,50.0f,50.0f }, 0.1f, 0.5f, 1.0f
    , { 0.5f,0.5f }, { 0.5f,0.5f }, { 1.0f,1.0f }, { 0.1f,1.0f,0.0f,0.5f }, { 0.0f,0.0f,0.0f,-0.01f });

    //���n
    SetParticle(ParticleName::dirt, "image\\PaticleAssets\\flashA_W.png", { 0.0f,0.0f,0.0f }
    , { 0.5f,0.5f,0.5f }, 0, 1, 30, 0.01f, { 0.0f,1.0f,0.0f }, { 50.0f,50.0f,50.0f }, 0.1f, 0.5f, 1.0f
    , { 0.75f,0.75f },{ 0.5f,0.5f },{ 1.05f,1.05f },{ 0.5f,0.3f,0.0f,0.5f },{ 0.01f,0.01f,0.0f,-0.02f });
}


//�G�t�F�N�g�N��
void ParticlePackage::ActParticle(Particle* pParticle, ParticleName pn
    , const XMFLOAT3& position, const XMFLOAT3& direction)
{
    for (auto& itr : emitter_[pn])
    {
        itr.position = position;
        itr.dir = direction;
        pParticle->Start(itr);
    }
}
//�I�[�o�[���[�h
void ParticlePackage::ActParticle(Particle* pParticle, ParticleName pn, const XMFLOAT3& position
    , const XMVECTOR& direction)
{
    XMFLOAT3 dir;
    XMStoreFloat3(&dir, direction);
    ActParticle(pParticle, pn, position, dir);
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
    XMFLOAT4 HSV((timeCount_ % 360) / 360.0f, 1.0f, 1.0f, 0.7f);//�F���A�ʓx�A���x�A�A���t�@
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

void ParticlePackage::ActLandingGrass(Particle* pParticle, XMFLOAT3 position)
{
    landingGrass_.position = position;
    pParticle->Start(landingGrass_);
}

void ParticlePackage::ActLandingDirt(Particle* pParticle, XMFLOAT3 position)
{
    landingDirt_.position = position;
    pParticle->Start(landingDirt_);
}

void ParticlePackage::ActLandingSpark(Particle* pParticle, XMFLOAT3 position)
{
    wheelSpark_.position = position;
    pParticle->Start(wheelSpark_);
}

//�Z�b�^�[
void ParticlePackage::SetBooster()
{
    //�W�F�b�g��
    boosterFire_.textureFileName = "image\\PaticleAssets\\circle_W.png";
    boosterFire_.position = { 0.0f,0.0f,0.0f };
    boosterFire_.positionErr = { 0.2f, 0.2f, 0.2f };
    boosterFire_.delay = 0;
    boosterFire_.number = 5;
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
    //�W�F�b�g�Ή�
    boosterSpark_.textureFileName = "image\\PaticleAssets\\circle_W.png";
    boosterSpark_.position = { 0.0f,0.0f,0.0f };
    boosterSpark_.positionErr = { 0.5f, 0.5f, 0.5f };
    boosterSpark_.delay = 0;
    boosterSpark_.number = 1;
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
    //����
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

    //���Ή�
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
    //��
    smokeCloud_.textureFileName = "image\\PaticleAssets\\cloudA.png";
    smokeCloud_.position = { 0.0f,0.0f,0.0f };
    smokeCloud_.positionErr = { 0.0f,0.0f,0.0f };
    smokeCloud_.delay = 0;
    smokeCloud_.number = 1;
    smokeCloud_.lifeTime = 40;
    smokeCloud_.gravity = 0.0f;
    smokeCloud_.dir = { 0.0f,0.0f,0.0f };
    smokeCloud_.dirErr = { 0.0f,0.0f,0.0f };
    smokeCloud_.speed = 0.0f;
    smokeCloud_.speedErr = 0.0f;
    smokeCloud_.size = { 0.5f,0.5f };
    smokeCloud_.sizeErr = { 0.0f,0.0f };
    smokeCloud_.scale = { 1.0f,1.0f };
    smokeCloud_.color = { 1.0f,1.0f,1.0f,0.1f };
    smokeCloud_.deltaColor = { 0.0f,0.0f,0.0f,-0.005f };
}

void ParticlePackage::SetGrass()
{
    //���n
    landingGrass_.textureFileName = "image\\PaticleAssets\\flashA_W.png";
    landingGrass_.position = { 0.0f,0.0f,0.0f };
    landingGrass_.positionErr = { 0.5f,0.5f,0.5f };
    landingGrass_.delay = 0;
    landingGrass_.number = 1;
    landingGrass_.lifeTime = 30;
    landingGrass_.gravity = 0.01f;
    landingGrass_.dir = { 0.0f,1.0f,0.0f };
    landingGrass_.dirErr = { 50.0f,50.0f,50.0f };
    landingGrass_.speed = 0.1f;
    landingGrass_.speedErr = 0.5f;
    landingGrass_.size = { 0.5f,0.5f };
    landingGrass_.sizeErr = { 0.5f,0.5f };
    landingGrass_.scale = { 1.0f,1.0f };
    landingGrass_.color = { 0.1f,1.0f,0.0f,0.5f };
    landingGrass_.deltaColor = { 0.0f,0.0f,0.0f,0.0f };
}

void ParticlePackage::SetDirt()
{
    //�y
    landingDirt_.textureFileName = "image\\PaticleAssets\\cloudA.png";
    landingDirt_.position = { 0.0f,0.0f,0.0f };
    landingDirt_.positionErr = { 0.5f,0.5f,0.5f };
    landingDirt_.delay = 0;
    landingDirt_.number = 1;
    landingDirt_.lifeTime = 30;
    landingDirt_.gravity = 0.01f;
    landingDirt_.dir = { 0.0f,1.0f,0.0f };
    landingDirt_.dirErr = { 50.0f,50.0f,50.0f };
    landingDirt_.speed = 0.1f;
    landingDirt_.speedErr = 0.5f;
    landingDirt_.size = { 0.75f,0.75f };
    landingDirt_.sizeErr = { 0.5f,0.5f };
    landingDirt_.scale = { 1.05f,1.05f };
    landingDirt_.color = { 0.5f,0.3f,0.0f,0.5f };
    landingDirt_.deltaColor = { 0.01f,0.01f,0.0f,-0.02f };
}

void ParticlePackage::SetSpark()
{
    wheelSpark_.textureFileName = "image\\PaticleAssets\\flashB_R.png";
    wheelSpark_.position = XMFLOAT3(0, 1, 0);
    wheelSpark_.positionErr = XMFLOAT3(0.15, 0.15, 0);
    wheelSpark_.delay = 0;
    wheelSpark_.number = 1;
    wheelSpark_.lifeTime = 2;
    wheelSpark_.gravity = 0;
    wheelSpark_.dir = XMFLOAT3(1, 0, 1);
    wheelSpark_.dirErr = XMFLOAT3(0, 0, 0);
    wheelSpark_.speed = 0.05f;
    wheelSpark_.speedErr = 0.01f;
    wheelSpark_.size = XMFLOAT2(1.25, 1.25);
    wheelSpark_.sizeErr = XMFLOAT2(0.5, 0.5);
    wheelSpark_.scale = XMFLOAT2(1.01, 1.01);
    wheelSpark_.color = XMFLOAT4(1, 1, 0, 1);
    wheelSpark_.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);

#if 0
    wheelSpark_.textureFileName = "image\\PaticleAssets\\flashB_R.png";
    wheelSpark_.position = XMFLOAT3(0, 1, 0);
    wheelSpark_.positionErr = XMFLOAT3(0.1, 0.1, 0);
    wheelSpark_.delay = 0;
    wheelSpark_.number = 1;
    wheelSpark_.lifeTime = 30;
    wheelSpark_.dir = XMFLOAT3(1, 0, 1);
    wheelSpark_.dirErr = XMFLOAT3(0, 20, 0);
    wheelSpark_.gravity = 0.002;
    wheelSpark_.speed = 0.2f;
    wheelSpark_.accel = 0.98;
    wheelSpark_.speedErr = 0.0;
    wheelSpark_.size = XMFLOAT2(0.5, 0.5);
    wheelSpark_.sizeErr = XMFLOAT2(0.1, 0.1);
    wheelSpark_.scale = XMFLOAT2(0.9, 0.9);
    wheelSpark_.color = XMFLOAT4(1, 1, 0, 1);
    wheelSpark_.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);
#endif
}

#if 0
//������
void TestScene::Initialize()
{
    Camera::SetPosition(XMFLOAT3(0, 10, -20));
    Camera::SetTarget(XMFLOAT3(0, 0, 0));
    hModel_ = Model::Load("model\\GroundGrid.fbx");

    pParticle__ = Instantiate<Particle>(this);

    //��
    {
        EmitterData data;

        //��
        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = XMFLOAT3(-4, 1.5, -4);
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        data.delay = 5;
        data.number = 1;
        data.lifeTime = 60;
        data.gravity = -0.002f;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.01f;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(1.5, 1.5);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.02);
        pParticle__->Start(data);

        //�΂̕�
        data.number = 3;
        data.positionErr = XMFLOAT3(0.8, 0, 0.8);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(10, 10, 10);
        data.size = XMFLOAT2(0.2, 0.2);
        data.scale = XMFLOAT2(0.95, 0.95);
        data.lifeTime = 120;
        data.speed = 0.1f;
        data.gravity = 0;
        pParticle__->Start(data);
    }


    //��
    {
        EmitterData data;

        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = XMFLOAT3(4, 1.5, 4);
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        data.delay = 5;
        data.number = 1;
        data.lifeTime = 150;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.1f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(2, 2);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(1, 1, 1, 0.2);
        data.deltaColor = XMFLOAT4(0, 0, 0, -0.002);
        pParticle__->Start(data);
    }

    //��
    {
        EmitterData data;

        data.textureFileName = "image\\PaticleAssets\\Water.png";
        data.position = XMFLOAT3(4, 3.3, -4.5);
        data.delay = 1;
        data.number = 3;
        data.lifeTime = 50;
        data.dir = XMFLOAT3(0, 0, -1);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.gravity = 0.005;
        data.speed = 0.1f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(1, 1);
        data.sizeErr = XMFLOAT2(0.8, 0.4);
        data.scale = XMFLOAT2(1.02, 1.02);
        data.color = XMFLOAT4(1, 1, 1, 0.1);
        pParticle__->Start(data);

        //���H
        data.textureFileName = "image\\PaticleAssets\\bubleB.png";
        data.position = XMFLOAT3(4, 3.3, -4.5);
        data.positionErr = XMFLOAT3(0.5, 0, 0);
        data.delay = 1;
        data.number = 3;
        data.lifeTime = 50;
        data.dir = XMFLOAT3(0, 0, -1);
        data.dirErr = XMFLOAT3(0, 20, 0);
        data.gravity = 0.005;
        data.speed = 0.1f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(0.3, 0.3);
        data.sizeErr = XMFLOAT2(0, 0);
        data.scale = XMFLOAT2(0.98, 0.98);
        data.color = XMFLOAT4(1, 1, 1, 1);
        pParticle__->Start(data);
    }

    //1
    {
        EmitterData data;
        //wind
        data.textureFileName = "image\\PaticleAssets\\circle_B.png";
        data.position = XMFLOAT3(-8, 1, 5);
        data.positionErr = XMFLOAT3(0.1, 0, 0.1);
        data.delay = 5;
        data.number = 1;
        data.lifeTime = 60;
        data.gravity = -0.002f;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.01f;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(2.5, 2.5);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(0.5, 0.8, 0.0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);
        pParticle__->Start(data);

        data.number = 3;
        data.positionErr = XMFLOAT3(0.8, 0, 0.8);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(10, 10, 10);
        data.size = XMFLOAT2(0.2, 0.2);
        data.scale = XMFLOAT2(0.95, 0.95);
        data.lifeTime = 120;
        data.speed = 0.1f;
        data.gravity = 0;
        pParticle__->Start(data);
    }

}

//�X�V
void TestScene::Update()
{
    if (Input::IsKeyDown(DIK_SPACE))
    {
        EmitterData data;
        data.textureFileName = "image\\PaticleAssets\\CloudC.png";
        data.position = XMFLOAT3(0, 0.05, 0);
        data.delay = 0;
        data.number = 80;
        data.lifeTime = 20;
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(90, 90, 90);
        data.speed = 0.1f;
        data.speedErr = 0.8;
        data.size = XMFLOAT2(1, 1);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(1.05, 1.05);
        data.color = XMFLOAT4(1, 1, 0.1, 1);
        data.deltaColor = XMFLOAT4(0, -1.0 / 20, 0, -1.0 / 20);
        pParticle__->Start(data);


        data.delay = 0;
        data.number = 80;
        data.lifeTime = 100;
        data.positionErr = XMFLOAT3(0.5, 0, 0.5);
        data.dir = XMFLOAT3(0, 1, 0);
        data.dirErr = XMFLOAT3(90, 90, 90);
        data.speed = 0.25f;
        data.speedErr = 1;
        data.accel = 0.93;
        data.size = XMFLOAT2(0.1, 0.1);
        data.sizeErr = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(0.99, 0.99);
        data.color = XMFLOAT4(1, 1, 0.1, 1);
        data.deltaColor = XMFLOAT4(0, 0, 0, 0);
        data.gravity = 0.003f;
        pParticle__->Start(data);
    }

    if (Input::IsKey(DIK_V))
    {
        EmitterData data;

        data.textureFileName = "image\\PaticleAssets\\flashB_R.png";
        data.position = XMFLOAT3(0, 1, 0);
        data.positionErr = XMFLOAT3(0.15, 0.15, 0);
        data.delay = 0;
        data.number = 1;
        data.lifeTime = 2;
        data.gravity = 0;
        data.dir = XMFLOAT3(1, 0, 1);
        data.dirErr = XMFLOAT3(0, 0, 0);
        data.speed = 0.05f;
        data.speedErr = 0.01f;
        data.size = XMFLOAT2(1.25, 1.25);
        data.sizeErr = XMFLOAT2(0.5, 0.5);
        data.scale = XMFLOAT2(1.01, 1.01);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);
        pParticle__->Start(data);

        data.textureFileName = "image\\PaticleAssets\\flashB_R.png";
        data.position = XMFLOAT3(0, 1, 0);
        data.positionErr = XMFLOAT3(0.1, 0.1, 0);
        data.delay = 0;
        data.number = 1;
        data.lifeTime = 30;
        data.dir = XMFLOAT3(1, 0, 1);
        data.dirErr = XMFLOAT3(0, 20, 0);
        data.gravity = 0.002;
        data.speed = 0.2f;
        data.accel = 0.98;
        data.speedErr = 0.0;
        data.size = XMFLOAT2(0.5, 0.5);
        data.sizeErr = XMFLOAT2(0.1, 0.1);
        data.scale = XMFLOAT2(0.9, 0.9);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.01);
        pParticle_->Start(data);
    }

    if (Input::IsKey(DIK_B))
    {
        EmitterData data;

        //��
        data.textureFileName = "image\\PaticleAssets\\CloudA.png";
        data.position = XMFLOAT3(-5, 1.0, -4);
        data.positionErr = XMFLOAT3(0.15, 0.15, 0.15);
        data.delay = 0;
        data.number = 4;
        data.lifeTime = 15;
        data.gravity = 0.001f;
        data.dir = XMFLOAT3(-1, 0, 0);
        data.dirErr = XMFLOAT3(0.75, 0.75, 0.75);
        data.speed = 0.4f;
        data.speedErr = 0.01;
        data.size = XMFLOAT2(0.5, 0.5);
        data.sizeErr = XMFLOAT2(0.2, 0.2);
        data.scale = XMFLOAT2(1.15, 1.15);
        data.color = XMFLOAT4(1, 0.2, 0, 1);
        data.deltaColor = XMFLOAT4(0.0, 0.02, 0.01, -0.075);
        pParticle_->Start(data);
    }
}
#endif
//power oh-ra
#if 0
if (!Input::IsKey(DIK_C))
{
    EmitterData data;

    //��
    data.textureFileName = "image\\PaticleAssets\\CloudA.png";
    data.position = transform_.position_;
    data.positionErr = XMFLOAT3(0.1, 0, 0.1);
    //data.delay = 1;
    data.number = 1;
    data.lifeTime = 10;
    data.gravity = -0.002f;
    data.dir = XMFLOAT3(0, 1, 0);
    data.dirErr = XMFLOAT3(0, 0, 0);
    data.speed = 0.75f;
    data.speedErr = 0.2;
    data.size = XMFLOAT2(5, 5);
    data.sizeErr = XMFLOAT2(0.4, 0.4);
    data.scale = XMFLOAT2(0.9, 0.9);
    data.color = XMFLOAT4(1, 1, 0, 1);
    data.deltaColor = XMFLOAT4(0, 0, 0.1, -0.2);
    pParticle->Start(data);

    //�΂̕�
    data.number = 1;
    data.positionErr = XMFLOAT3(0.8, 0, 0.8);
    data.dir = XMFLOAT3(0, 1, 0);
    data.dirErr = XMFLOAT3(10, 10, 10);
    data.size = XMFLOAT2(0.5, 0.5);
    data.scale = XMFLOAT2(0.98, 0.98);
    data.lifeTime = 30;
    data.speed = 0.2f;
    data.gravity = 0.005;
    data.color = XMFLOAT4(1, 1, 0, 1);
    data.deltaColor = XMFLOAT4(0, 0, 0.02, -0.015);
    pParticle->Start(data);
}
#endif