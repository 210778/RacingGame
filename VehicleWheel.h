#pragma once
#include "Engine/GameObject.h"

class Vehicle;

//�ԗ��̃^�C�����Ǘ�����N���X
//�^�C���̐���4����ł܂Ƃ߂ĊǗ�����
class VehicleWheel : public GameObject
{
    int hModel_;    //���f���ԍ�
    Vehicle* pVehicle_;  //�������������Ă�ԗ��̃|�C���^�[

    float rotateSpeedX_;    //�����x�ɍ��킹�ĉ�]������^�C����X��
    float handleRotateY_;   //�n���h���p�x�ɍ��킹�ĉ�]������^�C����Y��

    struct
    {
        float wheelHeight_; //�^�C���̍���f
        XMFLOAT3 posFR_;  //�E�΂ߑO�@�^�C���̈ʒu
        XMFLOAT3 posFL_;  //���΂ߑO�@�^�C���̈ʒu
        XMFLOAT3 posRR_;  //�E�΂ߌ��@�^�C���̈ʒu
        XMFLOAT3 posRL_;  //���΂ߌ��@�^�C���̈ʒu

        XMVECTOR cToFR_; //��������E�O�@�܂ł̃x�N�g��
        XMVECTOR cToFL_; //�������獶�O�@�܂ł̃x�N�g��
        XMVECTOR cToRR_; //��������E���@�܂ł̃x�N�g��
        XMVECTOR cToRL_; //�������獶���@�܂ł̃x�N�g��

        XMVECTOR centerToLeft_;     //���f���̒��S���獶�܂ł̃x�N�g��
        XMVECTOR centerToRight_;    //���f���̒��S����E�܂ł̃x�N�g��
        XMVECTOR centerToBottom_;   //���f���̒��S�����܂ł̃x�N�g��
    }Wheels;

public:
    //�R���X�g���N�^
    VehicleWheel(GameObject* parent);

    //�f�X�g���N�^
    ~VehicleWheel();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    /// <summary>
    /// �^�C���̏�񂢂낢��̃Z�b�^�[�@���@�^�C���̍������v�Z���Ďԗ��ɋ�����
    /// </summary>
    /// <param name="pVehicle">�������ԗ��̃|�C���^</param>
    /// <param name="hModel">���f���ԍ�</param>
    /// <param name="PosFL">���O�@�^�C���̈ʒu</param>
    /// <param name="PosFR">�E�O�@�^�C���̈ʒu</param>
    /// <param name="PosRL">�����@�^�C���̈ʒu</param>
    /// <param name="PosRR">�E���@�^�C���̈ʒu</param>
    void SetVehicleWheel(Vehicle* pVehicle, int hModel
        , XMFLOAT3 PosFL, XMFLOAT3 PosFR, XMFLOAT3 PosRL, XMFLOAT3 PosRR);

    //�^�C���̍������v�Z���Ďԗ��ɋ�����
    void calculateHeight();

    /// <summary>
    /// �^�C���̉�]���x�ƃn���h���p�x���Z�b�g
    /// </summary>
    /// <param name="speed">�^�C����X����]���x�i�p���x�j</param>
    /// <param name="rotate">�^�C����Y����]�p�x�i�n���h���p�x�ɍ��킹��j</param>
    void SetWheelSpeedRotate(float speed, float rotate)
    {
        rotateSpeedX_ = speed;
        handleRotateY_ = rotate;
    }


};