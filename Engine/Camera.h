#pragma once
#include "GameObject.h"

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	//初期化（プロジェクション行列作成）
	void Initialize();

	//更新（ビュー行列作成）
	void Update();

	//視点（カメラの位置）を設定
	void SetPosition(XMVECTOR position);
	//float
	void SetPosition(XMFLOAT3 position);
	//視点を取得
	XMVECTOR GetPosition();
	//float
	XMFLOAT3 GetPositionFloat();

	//焦点（見る位置）を設定
	void SetTarget(XMVECTOR target);
	//float
	void SetTarget(XMFLOAT3 target);
	//焦点を取得
	XMVECTOR GetTarget();
	//float
	XMFLOAT3 GetTargetFloat();

	//ビュー行列を取得
	XMMATRIX GetViewMatrix();

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();

	//ビルボード用回転行列を取得
	XMMATRIX GetBillboardMatrix();
};