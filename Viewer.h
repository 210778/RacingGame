#pragma once
#include "Engine/GameObject.h"

class Vehicle;

//���������Ǘ�����N���X
class Viewer : public GameObject
{
    Vehicle* pVehicle;

    float cameraLength_;//���f���̕\���p
    //�ړ��A��]�̑���
    float rotateSPD_;
    //�J������1��]���Ȃ����߂�
    float upLim_;
    float downLim_;
    //�Y�[���A�b�v�A�_�E���̌��E�Ƒ���
    float nearLim_;
    float farLim_;
    float zoomUp_;
    float zoomOut_;
    //�J�����p
    XMFLOAT3 camFlo_;
    float camX_;

    //�J�����̊����p
    float toFutureLength_;     //���݈ʒu���疢���ʒu�ֈړ����鑬�x(����)

    XMVECTOR camTarPlusVec_;    //�J�����̏œ_�ɒǉ�����x�N�g��

    Transform viewTransform_;   //�Ώۂ̃g�����X�t�H�[��
    XMVECTOR  viewAcceleration_; //�Ώۂ̉����x
    float     viewRotate_;       //�Ώۂ̒ǉ���]�l
    float     viewRotateAdd_;    //���ɂ�����l

public:
    //�R���X�g���N�^
    Viewer(GameObject* parent);

    //�f�X�g���N�^
    ~Viewer();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    /// <summary>
    /// ���_�����Z�b�g
    /// </summary>
    /// <param name="transform">������̂̃g�����X�t�H�[��</param>
    /// <param name="speed">�����x</param>
    /// <param name="handle">�ǉ��ŉ�]�������l</param>
    void SetViewValue(const Transform& transform,const XMVECTOR& speed, const float handle);

    void SetViewPosition();
};