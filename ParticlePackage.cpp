#include <map>
#include "ParticlePackage.h"

namespace ParticlePackage
{
    std::map<ParticleName, std::vector<EmitterData>> emitter_;

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
    //�������`�F�b�N
    if (!(emitter_.empty()))
    {
        return;
    }

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
    SetParticle(ParticleName::boost, "image\\PaticleAssets\\circle_W.png", { 0.f,0.f,0.f }
    , { 0.2f,0.2f,0.2f }, 0, 3, 15.f, 0.f, {0.f,0.f,0.f}, {60.f,60.f,60.f}, 0.1f, 0.05f, 1.f
    , {0.8f,0.8f}, {0.1f,0.1f}, {0.98f,0.98f}, {1.f,1.f,1.f,1.f}, { 0.f, -0.06f, -0.12f, -0.05f });
        //�Ή�
    SetParticle(ParticleName::boost, "image\\PaticleAssets\\circle_W.png", { 0.f,0.f,0.f }
    , {0.5f,0.5f,0.5f}, 0, 1, 15.f, 0.f, { 0.f,0.f,0.f }, { 90.f, 90.f, 90.f }, 0.1f, 0.05f, 1.f
    , {0.1f,0.1f}, {0.1f,0.1f}, {1.f,1.f}, {1.f,1.f,1.f,1.f}, { -0.02f, -0.02f, -0.1f, -0.01f });

    //��
    SetParticle(ParticleName::smoke, "image\\PaticleAssets\\cloudA.png", { 0.f,0.f,0.f }
    , { 0.f,0.f,0.f }, 0, 1, 20, 0.f, { 0.f,0.f,0.f }, { 0.f,0.f,0.f }, 0.f, 0.f, 1.f
    , { 0.5f,0.5f }, { 0.f,0.f }, { 1.f,1.f }, { 1.f,1.f,1.f,0.1f }, { 0.f,0.f,0.f,-0.005f });

    //���n
    SetParticle(ParticleName::grass, "image\\PaticleAssets\\flashA_W.png", { 0.f,0.f,0.f }
    , { 0.5f,0.5f,0.5f }, 0, 1, 20, 0.01f, { 0.f,1.f,0.f }, { 50.f,50.f,50.f }, 0.1f, 0.5f, 1.f
    , { 0.5f,0.5f }, { 0.5f,0.5f }, { 1.f,1.f }, { 0.1f,1.f,0.f,0.5f }, { 0.f,0.f,0.f,-0.01f });

    //���n
    SetParticle(ParticleName::dirt, "image\\PaticleAssets\\flashA_W.png", { 0.f,0.f,0.f }
    , { 0.5f,0.5f,0.5f }, 0, 1, 20, 0.005f, { 0.f,1.f,0.f }, { 50.f,50.f,50.f }, 0.1f, 0.5f, 1.f
    , { 0.75f,0.75f },{ 0.5f,0.5f },{ 1.07f,1.07f },{ 0.5f,0.3f,0.f,0.5f },{ 0.01f,0.01f,0.f,-0.02f });

    //�X�p�[�N�Ή�
    SetParticle(ParticleName::spark, "image\\PaticleAssets\\flashB_R.png", { 0.f, 0.f, 0.f }
    , { 0.15f, 0.15f, 0.f }, 0, 1, 2, 0.f, { 0.f,0.f,0.f }, { 0.f,0.f,0.f }, 0.05f, 0.01f, 1.f
    , { 1.25f,1.25f }, { 0.5f,0.5f }, { 1.01f,1.01f }, { 1.f,1.f,0.f,0.8f }, { 0.f,-0.03f,0.f,-0.02f });
        //���X�p�[�N
    SetParticle(ParticleName::spark, "image\\PaticleAssets\\flashB_R.png", { 0.f, 0.f, 0.f }
    , { 0.1f, 0.1f, 0.1f }, 0, 1, 20, 0.f, { 0.f,0.f,0.f }, { 20.f,20.f,20.f }, 0.2f, 0.5f, 1.f
    , { 0.5f,0.5f }, { 0.1f,0.1f }, { 0.95f,0.95f }, { 1.f,1.f,0.f,0.8f }, { 0.f,-0.03f,0.f,-0.02f });

    //�P�ʋ�
    SetParticle(ParticleName::gold, "image\\PaticleAssets\\circle_W.png", { 0.f, 0.f, 0.f }
    , { 0.15f, 0.15f, 0.15f }, 0, 1, 20, -0.01f, { 0.f,1.f,0.f }, { 60.f,60.f,60.f }, 0.2f, 0.05f, 1.f
    , { 1.f,1.f }, { 0.0f,0.0f }, { 1.01f,1.01f }, { 1.f,1.f,0.f,1.f }, { 0.f,0.f,0.02f,-0.05f });

    //�X
    SetParticle(ParticleName::ice, "image\\PaticleAssets\\flashC_W.png", { 0.f, 0.f, 0.f }
    , { 0.15f, 0.15f, 0.15f }, 0, 1, 20, 0.01f, { 0.f,1.f,0.f }, { 50.f,50.f,50.f }, 0.1f, 0.5f, 1.f
    , { 0.5f,0.5f }, { 0.0f,0.0f }, { 1.05f,1.05f }, { 0.0f,0.5f,1.f,1.0f }, { 0.05f,0.05f,0.f,-0.03f });

    //�����ړ�
    SetParticle(ParticleName::speed, "image\\PaticleAssets\\circle_W.png", { 0.f,0.f,0.f }
    , { 0.1f,0.1f,0.1f }, 0, 1, 20.f, 0.f, { 0.f,0.f,0.f }, { 50.f,50.f,50.f }, 0.2f, 0.1f, 0.98f
    , { 0.3f,0.3f }, { 0.0f,0.0f }, { 0.95f,0.95f }, { 5.f,1.f,1.f,1.f }, { 0.12f, 0.f, 0.f, -0.07f });
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
//�����Ȃ�
void ParticlePackage::ActParticle(Particle* pParticle, ParticleName pn, const XMFLOAT3& position)
{
    for (auto& itr : emitter_[pn])
    {
        itr.position = position;
        pParticle->Start(itr);
    }
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