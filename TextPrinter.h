#pragma once
#include "Engine/GameObject.h"

namespace TextValue
{

}

//◆◆◆を管理するクラス
class TextPrinter : public GameObject
{
    std::vector<int> hImage_;
    enum StartImageName
    {
        go = 0,
        one,
        two,
        three
    };

public:
    //コンストラクタ
    TextPrinter(GameObject* parent);

    //デストラクタ
    ~TextPrinter();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};