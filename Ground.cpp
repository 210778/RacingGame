#include "Engine/Model.h"

#include "Ground.h"
#include "CheckPoint.h"

using std::string;
using std::to_string;

//コンストラクタ
Ground::Ground(GameObject* parent)
    :GameObject(parent, "Ground")
    , chosenCircuit_(0)
    , defaultCheckpointSize_(60.0f), defaultStartRotate_(0.0f)
{
}

//デストラクタ
Ground::~Ground()
{
}

//初期化
void Ground::Initialize()
{
    //
    MakeCircuit();

    //チェックポイント設置
    MakeCheckPoint();

    //スタート地点設置
    MakeStartPoint();
}

//更新
void Ground::Update()
{
}

//描画
void Ground::Draw()
{
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

//チェックポイントを探してセット
void Ground::MakeCheckPoint()
{
    //コース全体
    for (int circuit = 0; circuit < circuits_.size(); circuit++)
    {
        int last = 0;   //最新のチェックポイントの番号

        //見つかり続ける限り終わらない
        while (true)
        {
            bool isSuccess = false;//見つかったか

            if (last == 9)
                transform_.position_.y = 0;

            //パーツ
            for (int part = 0; part < circuits_[circuit].parts_.size(); part++)
            {
                //モデル名
                string center = "checkpoint_";
                center += to_string(last);
                string out = center;
                center += "_center";
                out += "_out";

                XMFLOAT3 centerPos;
                //中心を探す
                if (Model::GetBonePosition(&centerPos, circuits_[circuit].parts_[part].model_, center))
                {
                    //見つかった
                    isSuccess = true;
                    bool outSuccess = false;

                    //パーツ
                    for (int part2 = 0; part2 < circuits_[circuit].parts_.size(); part2++)
                    {
                        //外側を探す
                        XMFLOAT3 outPos;
                        if (Model::GetBonePosition(&outPos, circuits_[circuit].parts_[part2].model_, out))
                        {
                            //見つかった
                            outSuccess = true;

                            XMVECTOR centerVec = XMLoadFloat3(&centerPos);
                            XMVECTOR outVec = XMLoadFloat3(&outPos);

                            ////チェックポイント作る
                            //CheckPoint *pCheckPoint = Instantiate<CheckPoint>(this);
                            //pCheckPoint->MakeSphereCollider(&centerPos
                            //    , *XMVector3Length(outVec - centerVec).m128_f32
                            //    , last);
                            ////プッシュバック
                            //circuits_[circuit].checkPoint_.push_back(pCheckPoint);

                            //新規
                            CircuitCheckPoint ccp(nullptr, centerPos
                                , *XMVector3Length(outVec - centerVec).m128_f32);
                            ccp.CP_position_.x *= -1;  //mayaとのずれの解消
                            circuits_[circuit].check_.push_back(ccp);

                            last++;
                            break;
                        }
                    }

                    if (!outSuccess)
                    {
                        ////内側は見つかったけど外側は見つからなかった
                        ////チェックポイント作る
                        //CheckPoint* pCheckPoint = Instantiate<CheckPoint>(this);
                        //pCheckPoint->MakeSphereCollider(&centerPos
                        //    , defaultCheckpointSize_ , last);
                        ////プッシュバック
                        //circuits_[circuit].checkPoint_.push_back(pCheckPoint);

                        //新規
                        CircuitCheckPoint ccp(nullptr, centerPos, defaultCheckpointSize_);
                        ccp.CP_position_.x *= -1;  //mayaとのずれの解消
                        circuits_[circuit].check_.push_back(ccp);

                        last++;
                    }
                }

            }

            if (!isSuccess)
            {
                //何も見つからないなら
                break;
            }
        }
    }
}

//スタート地点を探してセット
void Ground::MakeStartPoint()
{
    //コース全体
    for (int circuit = 0; circuit < circuits_.size(); circuit++)
    {
        int last = 0;   //最新のスタート位置の番号
        defaultStartRotate_ = 0;    //リセットしておく

        //見つかり続ける限り終わらない
        while (true)
        {
            bool isSuccess = false;//見つかったか

            //パーツ
            for (int part = 0; part < circuits_[circuit].parts_.size(); part++)
            {
                //モデル名
                string start = "startpoint_";
                start += to_string(last);
                string dir = start + "_direction";

                XMFLOAT3 startPos;
                //中心を探す
                if (Model::GetBonePosition(&startPos, circuits_[circuit].parts_[part].model_, start))
                {
                    //見つかった
                    isSuccess = true;

                    bool dirSuccess = false;

                    //パーツ
                    for (int part2 = 0; part2 < circuits_[circuit].parts_.size(); part2++)
                    {
                        //方向を表すボーンを探す
                        XMFLOAT3 dirPos;
                        if (Model::GetBonePosition(&dirPos, circuits_[circuit].parts_[part2].model_, dir))
                        {
                            //見つかった
                            dirSuccess = true;
                            last++;
                            
                            XMVECTOR startVec = XMVector3Normalize(XMLoadFloat3(&startPos) - XMLoadFloat3(&dirPos));
  
                            defaultStartRotate_ = Calculator::AngleBetweenVector(startVec, { 0.0f,0.0f,1.0f,0.0f });

                            //更新
                            Transform startTrans;
                            startTrans.position_ = startPos;
                            startTrans.position_.x *= -1;   //ずれ解消
                            startTrans.rotate_.y = defaultStartRotate_;
                            circuits_[circuit].startTransform_.push_back(startTrans);
                            break;
                        }
                    }

                    if (!dirSuccess)
                    {
                        //位置は見つかったけど方向は見つからなかった
                        //角度の計算はしない
                        last++;
                        //更新
                        Transform startTrans;
                        startTrans.position_ = startPos;
                        startTrans.position_.x *= -1;   //ずれ解消
                        startTrans.rotate_.y = defaultStartRotate_;
                        circuits_[circuit].startTransform_.push_back(startTrans);
                    }
                }
            }

            if (!isSuccess)
            {
                //何も見つからないなら
                break;
            }
        }
    }
}

//次のチェックポイントの位置を返す
XMFLOAT3* Ground::NextCheckPointPosition(int point, int next)
{
    //正規化する
    int number = (point + next - 1) % circuits_[chosenCircuit_].check_.size();
    if (number < 0)
        number += circuits_[chosenCircuit_].check_.size();

    XMFLOAT3 position = circuits_[chosenCircuit_].check_[number].CP_position_;
    return &position;
}

//
void Ground::SetCircuitParts(CircuitUnion* pCU, std::string modelName, int modelType)
{
    if (pCU == nullptr)
        return;

    //パーツ
    CircuitParts cp(Model::Load(modelName), modelType);
    pCU->parts_.push_back(cp);
}

void Ground::MakeCircuit()
{
    {
        //コース
        CircuitUnion circuit("circuit_1", 2);
        //パーツ
        SetCircuitParts(&circuit, "model\\circuit_1_R.fbx", road);
        SetCircuitParts(&circuit, "model\\circuit_1_G.fbx", turf);
        SetCircuitParts(&circuit, "model\\circuit_1_A.fbx", abyss);
        //追加
        circuits_.push_back(circuit);
    }
    {
        CircuitUnion circuit("circuit_2", 2);
        SetCircuitParts(&circuit, "model\\circuit_2_R.fbx", road);
        SetCircuitParts(&circuit, "model\\circuit_2_D.fbx", dirt);
        SetCircuitParts(&circuit, "model\\circuit_2_I.fbx", ice);
        SetCircuitParts(&circuit, "model\\circuit_1_A.fbx", abyss);
        circuits_.push_back(circuit);
    }
}

//選んだコースのチェックポイントを作る
void Ground::CreateChosenCircuit(int value)
{
    chosenCircuit_ = value;
    
    //安全
    if (circuits_.empty())
        return;
    if (chosenCircuit_ < 0 || chosenCircuit_ >= circuits_.size())
        chosenCircuit_ = 0;

    for (int i = 0; i < circuits_[chosenCircuit_].check_.size(); i++)
    {
        if (circuits_[chosenCircuit_].check_[i].pCP_ == nullptr)
        {
            circuits_[chosenCircuit_].check_[i].pCP_ = Instantiate<CheckPoint>(this);
            circuits_[chosenCircuit_].check_[i].pCP_->MakeSphereCollider(&circuits_[chosenCircuit_].check_[i].CP_position_
                , circuits_[chosenCircuit_].check_[i].CP_Radius_, i);
        }
    }
}