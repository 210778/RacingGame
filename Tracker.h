#pragma once
#include "Engine/GameObject.h"

//���������Ǘ�����N���X
class Tracker : public GameObject
{
    int hModel_;    //���f���ԍ�
    
    int life_;  //����

public:
    //�R���X�g���N�^
    Tracker(GameObject* parent);

    //�f�X�g���N�^
    ~Tracker();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    void SetLife(int seconds)
    {
        life_ = seconds * 60;
    }
};