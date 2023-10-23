#pragma once
#include <map>
#include "Engine/GameObject.h"

namespace TextValue
{
    void SetStandby(int value);
}

//���������Ǘ�����N���X
class TextPrinter : public GameObject
{
    std::vector<int> hImage_;
    std::map<std::string, int> imageMap_;
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