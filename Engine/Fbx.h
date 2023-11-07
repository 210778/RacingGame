#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <vector>
#include <string>
#include "Transform.h"
#include "Direct3D.h"

class FbxParts;

//レイキャスト用構造体
struct RayCastData
{
	XMFLOAT3	start;	//レイ発射位置
	XMFLOAT3	dir;	//レイの向きfloat3
	//XMVECTOR	dir;	//レイの向きベクトル
	float       dist;	//衝突点までの距離
	BOOL        hit;	//レイが当たったか

	XMFLOAT3 end;	//命中位置
	XMVECTOR normal;		//命中したポリゴンの法線ベクトル	下記含めてベクトル型にしとく
	XMVECTOR parallelism;	//命中したポリゴンと平行のベクトル
	XMVECTOR reflection;	//命中したときの反射方向のベクトル

	//引数なしコンストラクタ　距離はfloat最大距離で初期化
	RayCastData()
	{
		dist = FLT_MAX;
		hit = false;
	}
	//位置と方向つきコンストラクタ
	RayCastData(XMFLOAT3 startPosition, XMFLOAT3 direction)
	{
		start = startPosition;
		dir = direction;
		dist = FLT_MAX;
		hit = false;
	}
	//位置と方向ベクトルのコンストラクタ
	RayCastData(XMFLOAT3 startPosition, XMVECTOR directionVector)
	{
		RayCastData(startPosition, XMFLOAT3(XMVectorGetX(directionVector),
			XMVectorGetY(directionVector), XMVectorGetZ(directionVector)));
	}
};

//-----------------------------------------------------------
//　FBXファイルを扱うクラス
//　ほとんどの処理は各パーツごとにFbxPartsクラスで行う
//-----------------------------------------------------------
class Fbx
{
	//FbxPartクラスをフレンドクラスにする
	//FbxPartのprivateな関数にもアクセス可
	friend class FbxParts;



	//モデルの各パーツ（複数あるかも）
	std::vector<FbxParts*>	parts_;

	//FBXファイルを扱う機能の本体
	FbxManager* pFbxManager_;

	//FBXファイルのシーン（Mayaで作ったすべての物体）を扱う
	FbxScene*	pFbxScene_;


	// アニメーションのフレームレート
	FbxTime::EMode	_frameRate;

	//アニメーション速度
	float			_animSpeed;

	//アニメーションの最初と最後のフレーム
	int _startFrame, _endFrame;

	//ノードの中身を調べる
	//引数：pNode		調べるノード
	//引数：pPartsList	パーツのリスト
	void CheckNode(FbxNode* pNode, std::vector<FbxParts*> *pPartsList);




public:
	Fbx();
	~Fbx();

	//ロード
	//引数：fileName	ファイル名
	//戻値：成功したかどうか
	virtual HRESULT Load(std::string fileName);

	//描画
	//引数：World	ワールド行列
	void    Draw(Transform& transform, int frame);
	//描画
	void Draw(Transform& transform, int frame,Direct3D::SHADER_TYPE type);

	//解放
	void    Release();

	//任意のボーンの位置を取得
	//引数：boneName	取得したいボーンの位置
	//戻値：ボーンの位置
	XMFLOAT3 GetBonePosition(std::string boneName);

	/// <summary>
	/// 任意のボーンの位置を取得改
	/// </summary>
	/// <param name="position">取得したいボーンの位置が入れられるポインタ</param>
	/// <param name="boneName">取得したいボーンの名前</param>
	/// <returns>見つかったか、見つからなかったか</returns>
	bool GetBonePosition(XMFLOAT3* position, std::string boneName);

	//レイキャスト（レイを飛ばして当たり判定）
	//引数：data	必要なものをまとめたデータ
	void RayCast(RayCastData *data);

};

