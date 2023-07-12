#include "camera.h"
#include "Direct3D.h"
#include"Global.h"

namespace Camera
{
	//変数
	XMVECTOR position_;	//カメラの位置（視点）
	XMVECTOR target_;	//見る位置（焦点）
	XMMATRIX viewMatrix_;	//ビュー行列
	XMMATRIX projMatrix_;	//プロジェクション行列
	XMMATRIX billBoardMatrix_; //ビルボード行列
};

//初期化（プロジェクション行列作成）
void Camera::Initialize()
{
	position_ = XMVectorSet(0, 0, 0, 0);	//カメラの位置  XMVectorSet(3, 3, -10, 0);
	target_ = XMVectorSet(0, 0, 0, 0);	//カメラの焦点

	const float NEAR_CLIPPING = 0.1f;
	const float FAR_CLIPPING = 1600.0f;//800

	//const int WINDOW_WIDTH = 1280;				//ウィンドウの幅
	//const int WINDOW_HEIGHT = 720;				//ウィンドウの高さ

	//プロジェクション行列
	projMatrix_ = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,
		(FLOAT)Global::GetScreenWidth() / (FLOAT)Global::GetScreenHeight(),
		NEAR_CLIPPING,
		FAR_CLIPPING);
	/*
	この関数の引数は前から順番に
		1.画角（視野角）：XM_PIが3.14ラジアン＝180度。DIV4は÷4なのでXM_PIDIV4は45度を表す定数。
		2.アスペクト比：ウィンドウの(横, 縦)の比率。今数字を直接書いちゃってダサいので、後で何とかした。
		3.ニア（近）クリッピング面までの距離：カメラからこの値より近いものは映らない。
		4.ファー（遠）クリッピング面までの距離：カメラからこの値より遠いものは映らない。
	*/
}

//更新（ビュー行列作成）
void Camera::Update()
{
	//assert(!XMVector3Equal(EyeDirection, XMVectorZero()));
	//assert(!XMVector3IsInfinite(EyeDirection));
	//assert(!XMVector3Equal(UpDirection, XMVectorZero()));
	//assert(!XMVector3IsInfinite(UpDirection));

	//安全
	if (XMVector3Equal(position_, target_))
		return;

	//ビュー行列の作成
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));

	//ビルボード行列
	//（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
	//http://marupeke296.com/DXG_No11_ComeOnBillboard.html
	billBoardMatrix_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), target_- position_, XMVectorSet(0, 1, 0, 0));
	billBoardMatrix_ = XMMatrixInverse(nullptr, billBoardMatrix_);
}

//位置を設定
void Camera::SetPosition(XMVECTOR position)
{
	position_ = position;
}
void Camera::SetPosition(XMFLOAT3 position)
{
	position_ = XMLoadFloat3(&position);
}

//位置を取得
XMVECTOR Camera::GetPosition()
{
	return position_;
}
XMFLOAT3 Camera::GetPositionFloat()
{
	XMFLOAT3 answer;
	XMStoreFloat3(&answer, position_);
	return answer;
}

//焦点を設定
void Camera::SetTarget(XMVECTOR target)
{
	target_ = target;
}
void Camera::SetTarget(XMFLOAT3 target)
{
	target_ = XMLoadFloat3(&target);
}

//焦点を取得
XMVECTOR Camera::GetTarget()
{
	return target_;
}
XMFLOAT3 Camera::GetTargetFloat()
{
	XMFLOAT3 answer;
	XMStoreFloat3(&answer, target_);
	return answer;
}

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix() { return viewMatrix_; }

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix() { return projMatrix_; }

//ビルボード用回転行列を取得
XMMATRIX Camera::GetBillboardMatrix() { return billBoardMatrix_; }
