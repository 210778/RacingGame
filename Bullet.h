#pragma once
#include "Engine/GameObject.h"

//���������Ǘ�����N���X
class Bullet : public GameObject
{
    int hModel_;    //���f���ԍ�

    XMFLOAT3 acceleration_; //�����x

    int life_;//����

public:
    //�R���X�g���N�^
    Bullet(GameObject* parent);

    //�f�X�g���N�^
    ~Bullet();

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

    //�����x���ʒu�ɉ��Z����B�A�b�v�f�[�g�̍ŏ��ɂ����Ƃ�
    void Accelerator();

    //�Z�b�^�[�ƃQ�b�^�[
    void SetSpeed(XMFLOAT3 speed)
    {
        acceleration_ = speed;
    }
    XMFLOAT3 GetSpeed()
    {
        return acceleration_;
    }
};