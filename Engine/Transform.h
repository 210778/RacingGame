#pragma once
#include <DirectXMath.h>

using namespace DirectX;


//-----------------------------------------------------------
//位置、向き、拡大率などを管理するクラス
//-----------------------------------------------------------
class Transform
{
public:
	XMMATRIX matTranslate_;	//移動行列
	XMMATRIX matRotate_;	//回転行列	
	XMMATRIX matScale_;		//拡大行列
	XMFLOAT3 position_;		//位置
	XMFLOAT3 rotate_;		//向き
	XMFLOAT3 scale_;		//拡大率
	Transform * pParent_;	//親オブジェクトの情報

	//コンストラクタ
	Transform();

	//デストラクタ
	~Transform();

	//各行列の計算
	//引数：なし
	//戻値：なし
	void Calclation();

	//ワールド行列を取得
	//引数：なし
	//戻値：その時点でのワールド行列
	XMMATRIX GetWorldMatrix();

	//float3 + float3
	static XMFLOAT3 Float3Add(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
	}
	//float3 * float3
	static XMFLOAT3 Float3Times(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3(a.x * b.x, a.y * b.y, a.z * b.z);
	}
	//float3 * float
	static XMFLOAT3 Float3Times(const XMFLOAT3& a, float b)
	{
		return XMFLOAT3(a.x * b, a.y * b, a.z * b);
	}
	//float * float3
	static XMFLOAT3 Float3Times(float a, const XMFLOAT3& b)
	{
		return XMFLOAT3(a * b.x, a * b.y, a * b.z);
	}
};

