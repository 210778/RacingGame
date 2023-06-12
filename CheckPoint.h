#pragma once
#include "Engine/GameObject.h"

//���������Ǘ�����N���X
class CheckPoint : public GameObject
{
    int number_;    //�ԍ�
public:
    //�R���X�g���N�^
    CheckPoint(GameObject* parent);

    //�f�X�g���N�^
    ~CheckPoint();

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

    //�Z�b�^�[�ƃQ�b�^�[
    void SetNumber(int value) { number_ = value; };
    int GetNumber() { return number_; };

    //�����蔻������
    void MakeSphereCollider(XMFLOAT3 position, float size);

};