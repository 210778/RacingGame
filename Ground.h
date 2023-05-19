#pragma once
#include "Engine/GameObject.h"

class CheckPoint;

//◆◆◆を管理するクラス
class Ground : public GameObject
{
    int hModel_;    //モデル番号

    //std::vector<int> circuit;

    //コース部品
    struct CircuitParts
    {
        int model_; //モデル番号
        int type_;  //種類

        //引数なし
        CircuitParts() {};
        //コンストラクタ（モデル番号、種類）
        CircuitParts(int m, int t)
        {
            model_ = m;
            type_ = t;
        }
    };

    //コース１つ分
    struct CircuitUnion
    {
        std::vector<CircuitParts> parts_;    //コースのパーツたち
        std::string  name_;     //コースの名前
        int maxLap_;            //必要周回数
        std::vector<XMFLOAT3> checkPointPosition_;

        //引数なしコンストラクタ
        CircuitUnion(){
            name_ = "circuit";
            maxLap_ = 3;
        };
        //名前コンストラクタ
        CircuitUnion(std::string name) {
            name_ = name;
            maxLap_ = 3;
        };
        //名前,周回数コンストラクタ
        CircuitUnion(std::string name,int lap) {
            name_ = name;
            maxLap_ = lap;
        };
    };

    std::vector<CircuitUnion> circuits_;    //コースすべてのまとめ


    //選択されたコース
    int chosenCircuit_;

    //チェックポイントの当たり判定
    std::vector<CheckPoint*> checkPoint_;
    
public:

    enum circuitType
    {
        road = 0,   //通常道路 
        turf,       //芝生
        circuitMax  //終点
    };

    //コンストラクタ
    Ground(GameObject* parent);

    //デストラクタ
    ~Ground();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;



    //セッターとゲッター
    int GetModelHandle()
    { 
        return hModel_;
    }

    //コース選択
    void SetChosenCircuit(int i) { chosenCircuit_ = i; }
    int  GetChosenCircuit() { return chosenCircuit_; }
    //コースを選択して、当たり判定をセット。すでにある当たり判定は消去(引数：コース番号)
    void SetChosenCircuitCheckPoint(int value);

    //使用しているコースのポインタ
    CircuitUnion* GetCircuitUnion()
    {
        if (chosenCircuit_ <= 0 && chosenCircuit_ < circuits_.size())
            return &circuits_[chosenCircuit_];
        else
            return nullptr;
    }
};