#pragma once
#include "Engine/GameObject.h"

namespace TextValue
{

}

//���������Ǘ�����N���X
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
    //�R���X�g���N�^
    TextPrinter(GameObject* parent);

    //�f�X�g���N�^
    ~TextPrinter();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};