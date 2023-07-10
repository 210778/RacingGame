#pragma once
#include "Engine/GameObject.h"

class Vehicle;

//���������Ǘ�����N���X
class Viewer : public GameObject
{
    //Vehicle* pVehicle;
    Vehicle* pVehicle;

    float cameraLength_;//���f���̕\���p
    //�ړ��A��]�̑���
    //float moveSPD_;
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
    float camX;

    //�J�����̊����p
    float toFutureLength_;     //���݈ʒu���疢���ʒu�ֈړ����鑬�x(����)

    XMVECTOR camTarPlusVec;    //�J�����̏œ_�ɒǉ�����x�N�g��

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
};