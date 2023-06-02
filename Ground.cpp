#include "Engine/Model.h"

#include "Ground.h"
#include "CheckPoint.h"

//コンストラクタ
Ground::Ground(GameObject* parent)
    :GameObject(parent, "Ground"), hModel_(-1), chosenCircuit_(0)
{
}

//デストラクタ
Ground::~Ground()
{
}

//初期化
void Ground::Initialize()
{
    hModel_ = Model::Load("model\\Ground13-G.fbx");//Ground6
    assert(hModel_ >= 0);

    //位置や大きさを変えると多分バグる
    //transform_.position_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    transform_.scale_   = XMFLOAT3(1.0f, 1.0f, 1.0f);

    CheckPoint* pCheckPoint = Instantiate<CheckPoint>(this);
    pCheckPoint->SetPosition(0.0f, 0.0f, -200.0f);
    pCheckPoint->SetNumber(1);
    checkPoint_.push_back(pCheckPoint);

    CheckPoint* pCheckPoint2 = Instantiate<CheckPoint>(this);
    pCheckPoint2->SetPosition(0, 0, -100);
    pCheckPoint2->SetNumber(2);

    CheckPoint* pCheckPoint3 = Instantiate<CheckPoint>(this);
    pCheckPoint3->SetPosition(350, 0, 200);
    pCheckPoint3->SetNumber(3);

    //CheckPoint* pCheckPoint4 = Instantiate<CheckPoint>(this);
    //pCheckPoint4->SetPosition(250, 40, 100);

    //サーキットのモデル

    //サーキット１
    //パーツ
    CircuitParts circuit_1_R(Model::Load("model\\Ground13-R.fbx"), road);
    CircuitParts circuit_1_G(Model::Load("model\\Ground13-G.fbx"), turf);
    //まとめ
    CircuitUnion circuit_1("circuit_1",2);
    circuit_1.parts_.push_back(circuit_1_R);
    circuit_1.parts_.push_back(circuit_1_G);
    //チェックポイント
    circuit_1.checkPointPosition_.push_back({ 0.0f, 0.0f, -200.0f });
    circuit_1.checkPointPosition_.push_back({ 0.0f, 0.0f, -100.0f });
    circuit_1.checkPointPosition_.push_back({ 350.0f, 0.0f, 200.0f });

    //追加
    circuits_.push_back(circuit_1);
}

//更新
void Ground::Update()
{
    transform_.position_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

//描画
void Ground::Draw()
{
    //Model::SetTransform(hModel_, transform_);
    //Model::Draw(hModel_);

    if (circuits_.empty())
        return;

    for (int i = 0; i < circuits_[chosenCircuit_].parts_.size(); i++)
    {
        Model::SetTransform(circuits_[chosenCircuit_].parts_[i].model_, transform_);
        Model::Draw(circuits_[chosenCircuit_].parts_[i].model_);
    }
}

//開放
void Ground::Release()
{
}

void Ground::SetChosenCircuitCheckPoint(int value)
{
    SetChosenCircuit(value);
    //ループ
    for (int i = 0; i < circuits_[chosenCircuit_].checkPointPosition_.size(); i++)
    {
        CheckPoint* pCheckPoint = Instantiate<CheckPoint>(this);
        pCheckPoint->SetPosition(circuits_[chosenCircuit_].checkPointPosition_[i]);
        pCheckPoint->SetNumber(i);
        //checkPoint_.push_back(pCheckPoint);
    }

}