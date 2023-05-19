#pragma once
#include "Engine/GameObject.h"

//���������Ǘ�����N���X
class Speedometer : public GameObject
{
    std::vector<int> hImage_;

    float speed_;   //�󂯎�����X�s�[�h(0 ~ 360)

    //�g�p����摜�̎��
    enum
    {
        IMAGE_FRAME,
        IMAGE_NEEDLE,
        IMAGE_MAX
    }SpeedImage_;

public:
    //�R���X�g���N�^
    Speedometer(GameObject* parent);

    //�f�X�g���N�^
    ~Speedometer();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;


    //�Z�b�^�[
    void SetSpeed(int value)
    {
        speed_ = value;
    }
};