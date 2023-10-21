#pragma once
#include "Ground.h"

namespace Circuit
{
    Ground pGround_ = nullptr;

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

    //番号をセットして配列の中からコース選択
    void SetChosenCircuit(int i) { chosenCircuit_ = i; }
    //選択したコースの番号を取得
    int  GetChosenCircuit() { return chosenCircuit_; }

    //使用しているコースのポインタ 安全対策されていて、失敗したらnullptr
    CircuitUnion* GetCircuitUnion()
    {
        if (chosenCircuit_ >= 0 && chosenCircuit_ < circuits_.size())
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

    /// <summary>
    /// 次のチェックポイントの位置
    /// </summary>
    /// <param name="point">自分のチェックポイント</param>
    /// <param name="next">知りたい自分からX個次のポイント（デフォルト：１）</param>
    /// <returns>次のチェックポイントの位置のポインタ　配列の端に来たら最初から数える</returns>
    XMFLOAT3* NextCheckPointPosition(int point, int next = 1);

    //
    void SetCircuitParts(CircuitUnion* pCU, std::string modelName, circuitType modelType);

    void MakeCircuit();

    //選んだコースのチェックポイントを作る
    void CreateChosenCircuit(int value);

private:
    int a;
}

