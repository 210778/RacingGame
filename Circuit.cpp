#include "Circuit.h"
#include "CheckPoint.h"

namespace Circuit
{
    //コース部品
    struct CircuitParts
    {
        int model_; //モデル番号
        int type_;  //種類
        //引数なし
        CircuitParts(){
            model_ = -1;
            type_ = 0;
        }
        //コンストラクタ（モデル番号、種類）
        CircuitParts(int m, int t){
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
        CircuitCheckPoint(){
            pCP_ = nullptr;
            CP_position_ = { 0.0f,0.0f,0.0f };
            CP_Radius_ = 0.0f;
        }
        CircuitCheckPoint(CheckPoint* poi, const XMFLOAT3& pos, float rad){
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
        std::vector<Transform> startTransform_; //スタート地点のトランスフォーム(位置と回転)
        //引数なしコンストラクタ
        CircuitUnion() {
            name_ = "circuit";
            maxLap_ = 1;
        };
        //名前コンストラクタ
        CircuitUnion(std::string name) {
            name_ = name;
            maxLap_ = 1;
        };
        //名前,周回数コンストラクタ
        CircuitUnion(std::string name, int lap) {
            name_ = name;
            maxLap_ = lap;
        };
    };

    std::vector<CircuitUnion> circuits_;    //コースすべてのまとめ

    int chosenCircuit_;             //選択されたコース
    float defaultCheckpointSize_;   //チェックポイントの大きさ
    float defaultStartRotate_;      //スタート時の回転
    float checkPointLimit_;         //チェックポイントを探す限界

    std::map<circuitType, std::string> circuitTypeMap_; //unordered_map ではバグる?

}

//初期化
void Circuit::Initialize()
{
    chosenCircuit_ = 0;
    defaultCheckpointSize_ = 60.f;
    defaultStartRotate_ = 0.f;
    checkPointLimit_ = 500.f;

    circuitTypeMap_[circuitType::abyss] = "abyss";
    circuitTypeMap_[circuitType::boost] = "boost";
    circuitTypeMap_[circuitType::dirt] = "dirt";
    circuitTypeMap_[circuitType::ice] = "ice";
    circuitTypeMap_[circuitType::other] = "other";
    circuitTypeMap_[circuitType::road] = "road";
    circuitTypeMap_[circuitType::turf] = "turf";
}