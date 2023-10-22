#include "Engine/Model.h"
#include "Circuit.h"
#include "CheckPoint.h"
#include "Ground.h"


using std::string;
using std::to_string;
using std::stoi;
using std::vector;

namespace Circuit
{
    //コース部品
    struct CircuitParts
    {
        std::string modelName_;  //モデル名
        int model_; //モデル番号
        int type_;  //種類

        //引数なし
        CircuitParts()
        {
            modelName_ = "";
            model_ = -1;
            type_ = 0;
        }
        //コンストラクタ（モデル名、モデル番号、種類）
        CircuitParts(std::string n, int m, int t)
        {
            modelName_ = n;
            model_ = m;
            type_ = t;
        }
    };

    //チェックポイント型
    struct CircuitCheckPoint
    {
        CheckPoint* pCP_;
        XMFLOAT3    CP_position_;
        float       CP_Radius_;
        CircuitCheckPoint()
        {
            pCP_ = nullptr;
            CP_position_ = { 0.0f,0.0f,0.0f };
            CP_Radius_ = 0.0f;
        }
        CircuitCheckPoint(CheckPoint* poi, const XMFLOAT3& pos, float rad)
        {
            pCP_ = poi;
            CP_position_ = pos;
            CP_Radius_ = rad;
        }
    };

    //コース１つ分
    struct CircuitUnion
    {
        std::vector<CircuitParts> parts_;   //コースのパーツたち
        std::string  name_;                 //コースの名前
        int maxLap_;                        //必要周回数
        std::vector<CheckPoint*> checkPoint_;   //チェックポイントのポインタ
        std::vector<CircuitCheckPoint> check_;
        std::vector<Transform> startTransform_;              //スタート地点のトランスフォーム(位置と回転)

        //引数なしコンストラクタ
        CircuitUnion() {
            name_ = "circuit";
            maxLap_ = 3;
        };
        //名前コンストラクタ
        CircuitUnion(std::string name) {
            name_ = name;
            maxLap_ = 3;
        };
        //名前,周回数コンストラクタ
        CircuitUnion(std::string name, int lap) {
            name_ = name;
            maxLap_ = lap;
        };
    };

    //コースの種類
    enum circuitType
    {
        road = 0,   //通常道路 
        turf,       //芝生
        abyss,      //奈落
        ice,        //氷
        dirt,       //砂地
        boost,      //加速床
        other,      //どれでもない
        circuitMax  //終点
    };

    float defaultCheckpointSize_;   //チェックポイントの大きさ
    float defaultStartRotate_;      //スタート時の回転
    float checkPointLimit_;         //チェックポイントを探す限界
    std::vector<string> circuitNameArr_;

    std::vector<CircuitUnion> circuits_;    //コースすべてのまとめ

    std::map<circuitType, std::string> circuitTypeMap_; //コースの種類と対応する文字列

	//コースファイル読み込み
	void MakeCircuit();

    //コースのパーツのモデルを読み込み
    void SetCircuitParts(CircuitUnion* pCU,const std::string& modelName,const circuitType& modelType);

    //チェックポイントを探してセット
    void MakeCheckPoint();

    //スタート地点設置
    void MakeStartPoint();
}

//初期化
void Circuit::Initialize()
{
    defaultCheckpointSize_ = 60.0f;
    defaultStartRotate_ = 0.0f;
    checkPointLimit_ = 500;

    circuitTypeMap_[circuitType::abyss] = "abyss";
    circuitTypeMap_[circuitType::boost] = "boost";
    circuitTypeMap_[circuitType::dirt] = "dirt";
    circuitTypeMap_[circuitType::ice] = "ice";
    circuitTypeMap_[circuitType::other] = "other";
    circuitTypeMap_[circuitType::road] = "road";
    circuitTypeMap_[circuitType::turf] = "turf";

    //コースファイル読み込み
    MakeCircuit();

    //チェックポイント設置
    MakeCheckPoint();

    //スタート地点設置
    MakeStartPoint();
}

void Circuit::MakeCircuit()
{
    const int pathSize = MAX_PATH;

    string fileName = ".\\gameObject.ini";

    string appHead = "circuit_";
    string nameKey = "name";
    string lapKey = "lap";
    string circuitModelExtension = ".fbx";
    string modelFilePath = Global::GetModelFileName();

    for (int number = 0; number < 100; number++)
    {
        string app = appHead + to_string(number);
        char section[pathSize];
        DWORD result = GetPrivateProfileSection(app.c_str(), section, pathSize, fileName.c_str());
        if (result >= (pathSize - 2) || result <= 0)
            continue; // サイズオーバーエラー

        std::map<string, string> circuitMap;    //探索キーと値を入れる
        int start = 0;  //開始位置
        for (int i = 0; i < result; i++)
        {
            string key = "";
            string name = "";
            bool equal = false;
            for (int j = start; j < result; j++)
            {
                if (section[j] == '\0')
                {
                    start = j + 1;  //ここに来ずにループを抜けるのは想定外
                    break;
                }
                else if (section[j] == '=')
                {
                    if (!equal)
                        equal = true;
                }
                else
                {
                    if (equal)
                        name += section[j];
                    else
                        key += section[j];
                }
            }
            if (!(key.empty()) && !(name.empty()))
                circuitMap[key] = name; //追加
        }
        //周回数
        int lap = 1;
        if (circuitMap.count(lapKey) && stoi(circuitMap[lapKey]) >= 1)
            lap = stoi(circuitMap[lapKey]);

        //ステージ名
        string circuitName = "circuit";
        if (circuitMap.count(nameKey) && !(circuitMap[nameKey].empty()))
            circuitName = circuitMap[nameKey];

        //実体
        CircuitUnion circuit(circuitName, lap);
        //パーツがあるか検索して追加
        for (const auto& i : circuitTypeMap_)
        {
            //コースの種類があって、fbxファイルか？
            if (circuitMap.count(i.second) && circuitMap[i.second].find(circuitModelExtension) != std::string::npos)
            {
                string model = modelFilePath + circuitMap[i.second];
                SetCircuitParts(&circuit, model, i.first);

                //CircuitParts cp(modelFilePath + circuitMap[i.second], -1, i.first);
                //circuit.parts_.push_back(cp);
            }
        }
        //追加
        if (circuit.parts_.size() >= 1)
            circuits_.push_back(circuit);
    }
}

//コースのパーツのモデルを読み込み
void Circuit::SetCircuitParts(CircuitUnion* pCU,const string& modelName,const circuitType& modelType)
{
    if (pCU == nullptr)
        return;

    //パーツ
    int number = Model::Load(modelName);
    assert(number >= 0);

    CircuitParts cp(modelName, number, modelType);
    pCU->parts_.push_back(cp);
}

//チェックポイントを探してセット
void Circuit::MakeCheckPoint()
{
    //コース全体
    for (int circuit = 0; circuit < circuits_.size(); circuit++)
    {
        int last = 0;   //最新のチェックポイントの番号

        //見つからないか限界になるまで終わらない
        for (int i = 0; i < checkPointLimit_; i++)
        {
            bool isSuccess = false;//見つかったか

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
void Circuit::MakeStartPoint()
{
    //コース全体
    for (int circuit = 0; circuit < circuits_.size(); circuit++)
    {
        int last = 0;   //最新のスタート位置の番号
        defaultStartRotate_ = 0;    //リセットしておく
        //見つからないか限界になるまで終わらない
        for (int i = 0; i < checkPointLimit_; i++)
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

std::vector<std::string>* Circuit::GetCircuitNameArray()
{
    circuitNameArr_.clear();

    for (auto& itr : circuits_)
    {
        circuitNameArr_.push_back(itr.name_);
    }

    return &circuitNameArr_;
}

void Circuit::GetCircuits(std::vector<Circuit::CircuitUnion>* circuitArray)
{
    circuitArray = &circuits_;
}

void Circuit::ResetCircuitModelHandle()
{
    for (auto& itr : circuits_)
    {
        for (auto& itr2 : itr.parts_)
        {
            //パーツ
            itr2.model_ = Model::Load(itr2.modelName_);
            assert(itr2.model_ >= 0);
        }
    }
}