#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

#include "Bullet.h"
#include "Ground.h"

//�R���X�g���N�^
Bullet::Bullet(GameObject* parent)
    :GameObject(parent, "Bullet"), hModel_(-1),
    acceleration_(0.0f, 0.0f, 0.0f), life_(20)
{
    life_ *= 1;
}

//�f�X�g���N�^
Bullet::~Bullet()
{
}

//������
void Bullet::Initialize()
{
    hModel_ = Model::Load("model\\measure_pole.fbx");//model\\TestBullet.fbx
    assert(hModel_ >= 0);

    transform_.rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    transform_.scale_;// = XMFLOAT3(0.4f, 0.4f, 0.4f);
}

//�X�V
void Bullet::Update()
{
    Accelerator();

    life_--;
    if (life_ <= 0)
    {
        KillMe();
    }
/*
XMFLOAT3 �� XMVECTOR

XMFLOAT3 f;	//���������Ă�Ƃ���
XMVECTOR v = XMLoadFloat3(&f);

XMFLOAT3 �� XMVECTOR

XMVECTOR v; 	//���������Ă�Ƃ���
XMFLOAT3 f;
XMStoreFloat3(&f, v);
   
    XMVECTOR accVec = XMLoadFloat3(&acceleration_);
    transform_.rotate_ = XMFLOAT3(90.0f, 0.0f, 0.0f);
    XMVECTOR rotVec = XMLoadFloat3(&transform_.rotate_);
    XMStoreFloat3(&transform_.rotate_, accVec * rotVec);
    MessageBox(NULL, L"a", L" puzzleList_.size() ", MB_OK);
*/ 
#if 0
    Ground* pGround = (Ground*)FindObject("Ground"); //�X�e�[�W�I�u�W�F�N�g��T��
    int hGroundModel = pGround->GetModelHandle();    //���f���ԍ����擾
    RayCastData data;
    data.start = transform_.position_;  //���C�̔��ˈʒu
    data.start.y += 1;
    data.dir = acceleration_;           //���C�̕���
    Model::RayCast(hGroundModel, &data); //���C�𔭎�

    float length = 5.0f;    //�悭�킩��Ȃ�
    if (data.hit == true && data.dist < length)
    {
        //�����x���x�N�g����
        XMVECTOR accVec = XMLoadFloat3(&acceleration_);
        //�������o����
        //float* pPower = XMVector3Length(accVec).m128_f32;
        //�x�N�g���𓖂������@����
        accVec = data.reflection;
        //accVec = XMVector3Normalize(accVec);//���K��
        //������߂�
        //accVec *= *pPower;
        //�����x�ɓ����
        XMStoreFloat3(&acceleration_, accVec);
    }
#endif
    //��
#if 0
    XMVECTOR dirVec = XMLoadFloat3(&acceleration_);
    dirVec = XMVector3Normalize(dirVec);
    XMVECTOR headVec = { 0,1,0,0 };
    headVec = XMVector3Normalize(headVec);

    XMVECTOR crossed = XMVector3Cross(dirVec, headVec);

    float angle = acos(*XMVector3Dot(dirVec, headVec).m128_f32);

    transform_.rotate_.x = angle;
    transform_.rotate_.y = angle;
    transform_.rotate_.z = angle;


    //XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(30));	//�x����30�x��]������s��
    //v = XMVector3TransformCoord(v, m);	//�x�N�g�������s�񂍂ŕό`
#endif
#if 0
    //�x�N�g���̕��������� http://marupeke296.com/DXG_No16_AttitudeControl.html
    XMVECTOR upVec      = XMLoadFloat3(&acceleration_);
    XMVECTOR vecNormal  = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //�����x

    //XMVECTOR absUp = XMVectorAbs(upVec) * XMVectorAbs(vecNormal);
    float* upLength = XMVector3Length(upVec).m128_f32;
    float* normalLength = XMVector3Length(vecNormal).m128_f32;

    float *dot = XMVector3Dot(upVec, vecNormal / (*upLength * *normalLength)).m128_f32;
    float rot1 = acos(*dot);
    //��]�N�H�[�^�j�I���쐬
    XMVECTOR ax1 = XMVector3Cross(upVec, vecNormal);  //�p�������]���x�N�g��
    XMVECTOR qua1 = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    qua1 = XMQuaternionRotationAxis(ax1, rot1);//
    //�������킹��]
    XMVECTOR cq1;
    cq1 = XMQuaternionConjugate(qua1);//�����N�H�[�^�j�I��

    XMVECTOR odq = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); //���[�J���̕����N�H�[�^�j�I���iZ���j
    XMVECTOR res = XMQuaternionMultiply(cq1, odq);  //�����E�x�N�g���E���P
    res = XMQuaternionMultiply(res, qua1);

    XMFLOAT3 floRes;
    XMStoreFloat3(&floRes, res);
    XMVECTOR od = XMVectorSet(floRes.x, floRes.y, floRes.z, 0.0f);  //�p�����킹��]������x�N�g��

    XMVECTOR qua2   = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    XMVECTOR accVec = XMLoadFloat3(&acceleration_); //�����x

    float* odLength = XMVector3Length(od).m128_f32;
    float* accLength = XMVector3Length(accVec).m128_f32;
    //float* dot = XMVector3Dot(upVec, vecNormal / (*upLength * *normalLength)).m128_f32;

    float rot2 = acos(*(XMVector3Dot(od, accVec / (*odLength * *accLength)).m128_f32));
    qua2 = XMQuaternionRotationAxis(vecNormal, rot2);
    //�����N�H�[�^�j�I�������]�s����쐬
    XMVECTOR qua = XMQuaternionMultiply(qua1, qua2);

    XMMATRIX rotMat = XMMatrixRotationQuaternion(qua);
    XMVECTOR vecRot = XMLoadFloat3(&transform_.rotate_);
    vecRot = XMVector3TransformCoord(vecRot, rotMat);
    //�^��߂�
    XMStoreFloat3(&transform_.rotate_, vecRot);
#elif 0
    //�x�N�g���̕���������
    XMVECTOR posVec = XMLoadFloat3(&transform_.position_);
    XMVECTOR dirVec = XMLoadFloat3(&acceleration_);
    XMVECTOR upVec = { 90.0f,0.0f,0.0f,0.0f };

    XMMATRIX lookMat = XMMatrixLookAtRH(posVec, dirVec, upVec);
    XMVECTOR rotVec = { 90.0f,0.0f,0.0f,0.0f };
    rotVec = XMVector3TransformCoord(rotVec, lookMat);	//�x�N�g�������s�񂍂ŕό`
    XMStoreFloat3(&transform_.rotate_, rotVec);
#endif
    //
#if 0
    //������ς���
    XMVECTOR Z_Vec = { 0,0,1,0 };
    Z_Vec = XMVector3Normalize(Z_Vec); //���K��
    XMVECTOR SeekVec = XMLoadFloat3(&animal.statePos_[seek].goal);
    SeekVec = SeekVec - XMLoadFloat3(&animal.transform_.position_);
    SeekVec = XMVector3Normalize(SeekVec); //���K��
    float angle = *XMVector3AngleBetweenNormals(Z_Vec, SeekVec).m128_f32;
    angle = XMConvertToDegrees(angle);
    //
    if (animal.statePos_[seek].way > 0)
    {
        if (animal.transform_.position_.x > animal.statePos_[seek].goal.x)
        {
            angle = 360 - angle;
        }
    }
    else
    {
        if (animal.transform_.position_.x <= animal.statePos_[seek].goal.x)
        {
            angle -= 180;
        }
        else
        {
            angle = 180 - angle;
        }
    }
    animal.transform_.rotate_.y = angle;
#endif
}

//�����ɓ�������
void Bullet::OnCollision(GameObject* pTarget)
{
    //���������Ƃ��̏���
}

//�`��
void Bullet::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Bullet::Release()
{
}

void Bullet::Accelerator()
{
    transform_.position_.x += acceleration_.x * 0.5f;
    transform_.position_.y += acceleration_.y * 0.5f;
    transform_.position_.z += acceleration_.z * 0.5f;
}