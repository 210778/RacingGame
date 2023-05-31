#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

#include "Viewer.h"
#include "Vehicle.h"

//�R���X�g���N�^
Viewer::Viewer(GameObject* parent)
    :GameObject(parent, "Viewer"), pVehicle(nullptr)
    , rotateSPD_(1.0), upLim_(80.0), downLim_(90.0)
    , nearLim_(4.0f), farLim_(120.0f), zoomUp_(0.95f), zoomOut_(1.05f)
    , camFlo_(XMFLOAT3(0.0f, 10.0f, -20.0f)), camX(0.0f)
    , toFutureLength_(0.1f), camTarPlusVec({ 0.0f, 4.0f, 0.0f, 0.0f })
{
    /*
    , rotateSPD_(1.0), upLim_(55.0), downLim_(110.0)
    , nearLim_(4.0f), farLim_(120.0f), zoomUp_(0.95f), zoomOut_(1.05f)
    , camFlo_(XMFLOAT3(0.0f, 10.0f, -20.0f)), camX(0.0f)
    */
}

//�f�X�g���N�^
Viewer::~Viewer()
{
}

//������
void Viewer::Initialize()
{
    pVehicle = GetParent();
    assert(pVehicle != nullptr);
}

//�X�V
void Viewer::Update()
{
    //�e�̃g�����X�t�H�[�����擾
    XMFLOAT3 parentPos = pVehicle->GetPosition();
    XMFLOAT3 parentRot = pVehicle->GetRotate();

    //transform_.rotate_.X�̒l�ɍ��킹��X����]������s��
    XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians(parentRot.y));
    XMMATRIX matRotateX = XMMatrixRotationX(XMConvertToRadians(parentRot.x));
    //XMMATRIX matRotateZ = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.z));

    //���݈ʒu���x�N�g���ɂ��Ă���
    XMVECTOR vecPos = XMLoadFloat3(&parentPos);

    //�㉺��]
    if (Input::IsKey(DIK_F))
    {
        camX += rotateSPD_;
    }
    if (Input::IsKey(DIK_R))
    {
        camX -= rotateSPD_;
    }

    //����
    if (camX >= upLim_)
    {
        camX = upLim_ - 1.0f;
    }
    if (camX <= -downLim_)
    {
        camX = -downLim_ + 1.0f;
    }

    matRotateX += XMMatrixRotationX(XMConvertToRadians(camX));//�񂵂����ǉ��ŉ�]

    XMVECTOR vecCam = XMLoadFloat3(&camFlo_);    //������ɐL�т�x�N�g����p��
    vecCam = XMVector3TransformCoord(vecCam, matRotateX);   //����������ɍ��킹�ĉ�](1)
    vecCam = XMVector3TransformCoord(vecCam, matRotateY);   //����������ɍ��킹�ĉ�](2)
    
#if 0
    Camera::SetPosition(vecPos + vecCam);     //���݂̈ʒu�ƃx�N�g���𑫂��ăJ�����̈ʒu�ɂ���
    Camera::SetTarget(vecPos);                //�J�����̏œ_�̓v���C���[�̈ʒu
#else
    //�J�����̊���
    XMVECTOR futurePosition_ = vecPos + vecCam;  //�ڕW�̈ʒu
    XMVECTOR futureTarget_   = vecPos + camTarPlusVec;           //�ڕW�̏œ_ + �ǉ�

    XMVECTOR presentPosition = Camera::GetPosition();   //���݂̈ʒu
    XMVECTOR presentTarget   = Camera::GetTarget();     //���݂̏œ_

    XMVECTOR presentToFuturePosition = futurePosition_ - presentPosition;   //���݂��疢���̃x�N�g��
    XMVECTOR presentToFutureTarget   = futureTarget_ - presentTarget;

    presentToFuturePosition *= toFutureLength_;   //������Z������
    presentToFutureTarget   *= toFutureLength_;

    Camera::SetPosition(presentPosition + presentToFuturePosition); //���������݂̃x�N�g���ɑ����Ă���
    Camera::SetTarget(presentTarget + presentToFutureTarget);       //
#endif

    //�Y�[������
    float* pLength = XMVector3Length(vecCam).m128_f32;
    float length = *pLength;

    if (Input::IsKey(DIK_T))
    {
        if (length > nearLim_)
        {
            camFlo_.x *= zoomUp_;
            camFlo_.y *= zoomUp_;
            camFlo_.z *= zoomUp_;
        }
    }
    if (Input::IsKey(DIK_G))
    {
        if (length < farLim_)
        {
            camFlo_.x *= zoomOut_;
            camFlo_.y *= zoomOut_;
            camFlo_.z *= zoomOut_;
        }
    }

}

//�`��
void Viewer::Draw()
{
}

//�J��
void Viewer::Release()
{
}
