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
        CircuitParts()
        {
            model_ = -1;
            type_ = 0;
        }
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
        std::vector<CircuitParts> parts_;   //コースのパーツたち
        std::string  name_;                 //コースの名前
        int maxLap_;                        //必要周回数
        std::vector<CheckPoint*> checkPoint_;   //チェックポイントのポインタ
        std::vector<Transform> startTransform_;              //スタート地点のトランスフォーム(位置と回転)

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

    float defaultCheckpointSize_;   //チェックポイントの大きさ

    float defaultStartRotate_;      //スタート時の回転
    
public:

    enum circuitType
    {
        road = 0,   //通常道路 
        turf,       //芝生
        abyss,      //奈落
        other,      //どれでもない
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

    //番号をセットして配列の中からコース選択
    void SetChosenCircuit(int i) { chosenCircuit_ = i; }
    //選択したコースの番号を取得
    int  GetChosenCircuit() { return chosenCircuit_; }

    //使用しているコースのポインタ 安全対策されていて、失敗したらnullptr
    CircuitUnion* GetCircuitUnion()
    {
        if (chosenCircuit_ <= 0 && chosenCircuit_ < circuits_.size())
            return &circuits_[chosenCircuit_];
        else
            return nullptr;
    }

    //チェックポイントを探してセット
    void MakeCheckPoint();

    //スタート地点を探してセット
    void MakeStartPoint();

    //次のチェックポイントの位置を返す
    //ただしサイズがオーバーしてれば０番の位置を返す
    XMFLOAT3 NextCheckPointPosition(int number);
};