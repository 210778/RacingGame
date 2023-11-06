#pragma once
#include "Engine/GameObject.h"

//���������Ǘ�����N���X
class Sample : public GameObject
{
    int hModel_;    //���f���ԍ�
    XMFLOAT3 modelScale_;

public:
    //�R���X�g���N�^
    Sample(GameObject* parent);

    //�f�X�g���N�^
    ~Sample();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    //�����ɓ�������
   //�����FpTarget ������������
    void OnCollision(GameObject* pTarget) override;
};