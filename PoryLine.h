#pragma once
#include <list>
#include <DirectXMath.h>
#include <string>
#include "Engine/Direct3D.h"
#include "Engine/Texture.h"

using namespace DirectX;
using namespace std;

////コンスタントバッファー
//struct CONSTANT_BUFFER
//{
//	XMMATRIX	matWVP;
//	XMFLOAT4	color;
//};
//
////頂点情報
//struct VERTEX
//{
//	XMFLOAT3 position;
//	XMFLOAT3 uv;
//};

//コンスタントバッファー
struct ConstantBuffer
{
	XMMATRIX	matWVP;
	XMFLOAT4	color;
};

//頂点情報
struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT3 uv;
};


class PoryLine
{
	const float WIDTH_;		//太さ
	const int LENGTH_;		//長さ

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_;	//画像

	list<XMFLOAT3> positions_;	//過去length_回分の位置

public:
	//コンストラクタ
	PoryLine();

	//現在の位置を記憶させる
	//引数：pos	現在の位置
	void AddPosition(XMFLOAT3 pos);

	//ロード
	//引数：fileName	画像ファイル名
	//戻値：成功/失敗
	HRESULT Load(std::string fileName);

	//描画
	void Draw();

	//解放
	void Release();
};

