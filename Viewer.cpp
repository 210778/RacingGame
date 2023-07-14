#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

#include "Viewer.h"
#include "Vehicle.h"

//コンストラクタ
Viewer::Viewer(GameObject* parent)
    :GameObject(parent, "Viewer"), pVehicle(nullptr)
    , rotateSPD_(1.0), upLim_(80.0), downLim_(90.0)
    , nearLim_(4.0f), farLim_(120.0f), zoomUp_(0.95f), zoomOut_(1.05f)
    , camFlo_(XMFLOAT3(0.0f, 10.0f, -20.0f)), camX_(0.0f)
    , toFutureLength_(0.15f)/*0.1f*/, camTarPlusVec_({0.0f, 0.0f, 0.0f, 0.0f})// camTarPlusVec({ 0.0f, 4.0f, 0.0f, 0.0f })
    , viewRotate_(0.0f), viewRotateAdd_(0.25f)
{
}

//デストラクタ
Viewer::~Viewer()
{
}

//初期化
void Viewer::Initialize()
{
    pVehicle = (Vehicle*)GetParent();
    assert(pVehicle != nullptr);
}

//更新
void Viewer::Update()
{
    //親のトランスフォームを取得
    //XMFLOAT3 parentPos = pVehicle->GetPosition();
    //XMFLOAT3 parentRot = pVehicle->GetRotate();
    XMFLOAT3 parentPos = viewTransform_.position_;
    XMFLOAT3 parentRot = viewTransform_.rotate_;

    //加速度取得
    XMFLOAT3 vehicleAcc;
    XMStoreFloat3(&vehicleAcc, pVehicle->GetAcceleration());

    //transform_.rotate_.Xの値に合わせてX軸回転させる行列
    XMMATRIX matRotateZ = XMMatrixRotationZ(XMConvertToRadians(parentRot.z));
    XMMATRIX matRotateX = XMMatrixRotationX(XMConvertToRadians(parentRot.x));
    XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians(parentRot.y));    

    //現在位置をベクトルにしておく
    XMVECTOR vecPos = XMLoadFloat3(&parentPos);

    matRotateX += XMMatrixRotationX(XMConvertToRadians(camX_));//回した分追加で回転

    XMVECTOR vecCam = XMLoadFloat3(&camFlo_);    //後ろ上方に伸びるベクトルを用意
    vecCam = XMVector3TransformCoord(vecCam, matRotateZ);   //それを向きに合わせて回転(1)
    vecCam = XMVector3TransformCoord(vecCam, matRotateX);   //それを向きに合わせて回転(2)
    vecCam = XMVector3TransformCoord(vecCam, matRotateY);   //それを向きに合わせて回転(3)
    
    //ハンドルの値ぶん回転
    vecCam = XMVector3TransformCoord(vecCam
        , XMMatrixRotationY(XMConvertToRadians(viewRotate_ * viewRotateAdd_)));

#if 0
    Camera::SetPosition(vecPos + vecCam);     //現在の位置とベクトルを足してカメラの位置にする
    Camera::SetTarget(vecPos);                //カメラの焦点はプレイヤーの位置
#else
    //カメラの慣性
    XMVECTOR futurePosition_ = vecPos + vecCam;  //目標の位置
    XMVECTOR futureTarget_   = vecPos + camTarPlusVec_;           //目標の焦点 + 追加

    XMVECTOR presentPosition = Camera::GetPosition();   //現在の位置
    XMVECTOR presentTarget   = Camera::GetTarget();     //現在の焦点

    XMVECTOR presentToFuturePosition = futurePosition_ - presentPosition;   //現在から未来のベクトル
    XMVECTOR presentToFutureTarget   = futureTarget_ - presentTarget;

    presentToFuturePosition *= toFutureLength_;   //長さを短くする
    presentToFutureTarget   *= toFutureLength_;

    Camera::SetPosition(presentPosition + presentToFuturePosition); //少しずつ現在のベクトルに足していく
    Camera::SetTarget(presentTarget + presentToFutureTarget);       //
#endif

    //上下回転
    if (Input::IsKey(DIK_F))
    {
        camX_ += rotateSPD_;
    }
    if (Input::IsKey(DIK_R))
    {
        camX_ -= rotateSPD_;
    }

    //制限
    if (camX_ >= upLim_)
    {
        camX_ = upLim_ - 1.0f;
    }
    if (camX_ <= -downLim_)
    {
        camX_ = -downLim_ + 1.0f;
    }

    //ズーム調節
    float length = *XMVector3Length(vecCam).m128_f32;

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

//描画
void Viewer::Draw()
{
}

//開放
void Viewer::Release()
{
}

//視点情報をセット
void Viewer::SetViewValue(const Transform& transform, const XMVECTOR& speed, const float handle)
{
    viewTransform_      = transform;
    viewAcceleration_   = speed;
    viewRotate_         = handle;
}

void Viewer::SetViewPosition()
{
    ;
}