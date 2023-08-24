#include "Engine/Model.h"

#include "Ground.h"
#include "CheckPoint.h"

using std::string;
using std::to_string;

//コンストラクタ
Ground::Ground(GameObject* parent)
    :GameObject(parent, "Ground"), hModel_(-1), chosenCircuit_(0)
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
    hModel_ = Model::Load("model\\Ground13-G.fbx");
    assert(hModel_ >= 0);

    //コース１
    CircuitUnion circuit_1("circuit_1",1);
    //パーツ
    CircuitParts circuit_1_R(Model::Load("model\\Ground17-R.fbx"), road);
    CircuitParts circuit_1_G(Model::Load("model\\Ground17-I.fbx"), ice);
    CircuitParts circuit_1_D(Model::Load("model\\Ground17-D.fbx"), dirt);
    CircuitParts circuit_1_B(Model::Load("model\\Ground17-Rainbow.fbx"), boost);
    CircuitParts circuit_1_A(Model::Load("model\\bottomWater.fbx"), abyss);
    //まとめ
    circuit_1.parts_.push_back(circuit_1_R);
    circuit_1.parts_.push_back(circuit_1_G);

    circuit_1.parts_.push_back(circuit_1_D);
    circuit_1.parts_.push_back(circuit_1_B);

    circuit_1.parts_.push_back(circuit_1_A);
    //追加
    circuits_.push_back(circuit_1);

    //コース2
    CircuitUnion circuit_2("circuit_2", 1);
    //パーツ
    CircuitParts circuit_2_R(Model::Load("model\\circuit_12_R.fbx"), road);
    circuit_2.parts_.push_back(circuit_2_R);

    CircuitParts circuit_2_G(Model::Load("model\\circuit_1_G.fbx"), turf);
    circuit_2.parts_.push_back(circuit_2_G);

    circuit_2.parts_.push_back(circuit_1_A);
    //追加
    circuits_.push_back(circuit_2);

    //コース3
    CircuitUnion circuit_3("circuit_3", 1);
    //パーツ
    CircuitParts circuit_3_R(Model::Load("model\\Ground16-R.fbx"), road);
    circuit_3.parts_.push_back(circuit_3_R);

    CircuitParts circuit_3_G(Model::Load("model\\Ground16-G.fbx"), turf);
    circuit_3.parts_.push_back(circuit_3_G);

    circuit_3.parts_.push_back(circuit_1_A);
    //追加
    circuits_.push_back(circuit_3);

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

                            //チェックポイント作る
                            CheckPoint *pCheckPoint = Instantiate<CheckPoint>(this);
                            pCheckPoint->MakeSphereCollider(&centerPos
                                , *XMVector3Length(outVec - centerVec).m128_f32
                                , last);
                            //プッシュバック
                            circuits_[circuit].checkPoint_.push_back(pCheckPoint);
                            last++;
                            break;
                        }
                    }

                    if (!outSuccess)
                    {
                        //内側は見つかったけど外側は見つからなかった
                        //チェックポイント作る
                        CheckPoint* pCheckPoint = Instantiate<CheckPoint>(this);
                        pCheckPoint->MakeSphereCollider(&centerPos
                            , defaultCheckpointSize_ , last);
                        //プッシュバック
                        circuits_[circuit].checkPoint_.push_back(pCheckPoint);
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
                            /*
                            //正規化しなくていいはず
                            XMVECTOR vecZ = { 0.0f,0.0f,1.0f,0.0f };
                            XMVECTOR vecUp = { 0.0f,1.0f,0.0f,0.0f };


                            //ベクトルから角度を計算
                            defaultStartRotate_ = XMConvertToDegrees(acos(*XMVector3Dot(startVec, vecZ).m128_f32
                                / (*XMVector3Length(startVec).m128_f32 * *XMVector3Length(vecZ).m128_f32)));

                            //外積を使わないと0 ~ 180　になってしまう
                            XMVECTOR cro = XMVector3Cross(startVec, vecZ - startVec);
                            if (*XMVector3Dot(cro, vecUp).m128_f32 > 0.0f)
                                defaultStartRotate_ += 180;
                            */

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