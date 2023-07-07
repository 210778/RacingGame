#pragma once
#include "Engine/GameObject.h"

//���������Ǘ�����N���X
class MeasurePole : public GameObject
{
    int hModel_;    //���f���ԍ�
public:
    //�R���X�g���N�^
    MeasurePole(GameObject* parent);

    //�f�X�g���N�^
    ~MeasurePole();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    /// <summary>
    /// ���C�L���X�g�̉���
    /// </summary>
    /// <param name="start">�X�^�[�g�ʒu</param>
    /// <param name="direction">���C�̕���</param>
    /// <param name="length">���肷�钷��</param>
    void ViewRayCast(const XMFLOAT3* start, const XMVECTOR* direction, const float length);
};