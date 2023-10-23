#pragma once
#include <map>
#include <string>
#include <vector>
#include "Engine/GameObject.h"

class CircuitCheckPoint;
class CheckPoint;
class CircuitUnion;
class GameObject;

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


    //初期化
    void Initialize();

    //描画
    void Draw();

    std::vector<std::string>* GetCircuitNameArray();

    //パーツのモデル番号を再セットする
    void ResetCircuitModelHandle();

    //コースを選ぶ
    void SetChosenCircuit(int value);

    //選んだコースのチェックポイントを作る
    void CreateChosenCircuit(GameObject* pGO);

    /// <summary>
    /// 次のチェックポイントの位置
    /// </summary>
    /// <param name="point">自分のチェックポイント</param>
    /// <param name="next">知りたい自分からX個次のポイント（デフォルト：１）</param>
    /// <returns>次のチェックポイントの位置のポインタ　配列の端に来たら最初から数える</returns>
    XMFLOAT3* GetNextCheckPointPosition(int point, int next = 1);

    //コース配列から選択したコースのポインタを取得
    CircuitUnion* GetChosenCircuit();
}

