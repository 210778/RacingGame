#pragma once
#include "Engine/GameObject.h"

class Course;
class Speedometer;
class Text;
class Particle;
class Ground;
class VehicleWheel;

//�ԗ����Ǘ�����N���X
class Vehicle : public GameObject
{
protected:
    int hModel_;    //���f���ԍ�
    int hGroundModel_;  //�n�ʂ̃��f���ԍ�

    XMVECTOR acceleration_; //�����x

    //�ړ��A��]�̑���
    float moveSPD_;
    float rotateSPD_;
    float jumpForce_;

    float gravity_;  //�d��
    float speedLimit_;  //�����x�̐���

    //�n���h���֌W
    float handleRotate_;    //�n���h���̉�]�i�P�ʁF�x�j90�K�` -90�K���炢�H
    float handleRotateMax_; //�n���h���̌��E
    bool handleFlag_;       //�n���h���𓮂����Ă����� true
    //��]����Ƃ��̒���
    float turnAdjust_;
    float driveAdjust_;
    float handleAdjust_;

    float slideHandleRotateAdd_; //����Ƃ��̃n���h�����x�̒ǉ�
    float slideHandleAngleLimitAdd_; //����Ƃ��̃n���h���p�x�̒ǉ�

    float wheelSpeed_;  //�^�C���̉�]�@�n�ʂɂ��ƌ����������ɉ����x�����Z�@�A�N�Z���ő�����
    XMVECTOR wheelDirection_; //�g�����͕s��

    //float frictionForce_;   //���C
    //�n��E��
    bool landingFlag_;          //���n�A��
    float wheelFriction_;       //�^�C�����C
    float airFriction_;         //��C���C
    float turfFriction_;        //�Ő����C

    bool  slideFlag_;           //���s�A����
    float sideFriction_;        //�������̖��C
    float sideSlideFriction_;   //�������󒆖��C

    //�e�یn
    int coolTime_;  //�e�ۗp
    float bulletPower_;
    int heatAdd_;   //�N�[���^�C���ɒǉ�

    //���C�L���X�g�n
    float rayStartHeight;   //��Ƀ��C�L���X�g����Ƃ��̍���

    //�X�s�[�h���[�^�[
    Speedometer* pSpeedometer;

    //�����V���[�Y
    Text* pTextSpeed_;  //����
    Text* pTextTime_;   //�o�ߎ���
    Text* pTextLap_;    //����
    Text* pTextEngine_; //�G���W����]��

    unsigned long long time_; //�o�߃^�C��

    int pointCount_;    //�`�F�b�N�|�C���g�o�ߐ�
    int lapCount_;      //����
    int lapMax_;        //�K�v����
    bool goalFlag_;     //true�Ȃ�S�[�����
    int hImage_;        

    float mass_;     //�Ԃ̏d��
    float engineRotate_;    //�G���W����]��

    //�T�[�L�b�g�p
    //CircuitUnion* pCircuitUnion;

    XMVECTOR frontVec_;//���ʂ̃x�N�g��

    int landingType_; //�n�ʂ̃^�C�v

    Particle* pParticle_;//�G�t�F�N�g�p

    Ground* pGround_;//�n�ʂ̃|�C���^�[

    VehicleWheel* pWheels_;//�^�C���̃|�C���^�[

    float wheelSpeedAdd_; //�����x�ɉ��Z����^�C���̉�]

    short accZDirection_; //�O�����ɐi��ł邩���������B�O�F+1, ��F-1

    float wheelParticleLength_;

    //�ԗ��̊e�T�C�Y
    struct
    {
        float toRight_;   //�E�[�@�܂ł̋���
        float toLeft_;    //���[�@�܂ł̋���
        float toFront_;   //�O���@�܂ł̋���
        float toRear_;    //����@�܂ł̋���
        float toTop_;     //��[�@�܂ł̋���
        float toCenter_;  //��[�܂ł̋����̂P�^�Q
        float rightToLeft_;     //�E�[�@����@���[�@�܂ł̋���
        float frontToRear_;     //�O���@����@����@�܂ł̋���
        float toFrontRight_;  //�E�΂ߑO�@�܂ł̋���
        float toFrontLeft_;   //���΂ߑO�@�܂ł̋���
        float toRearRight_;   //�E�΂ߌ��@�܂ł̋���
        float toRearLeft_;    //���΂ߌ��@�܂ł̋���
        XMFLOAT3 wheelFR_;  //�E�΂ߑO�@�^�C���̈ʒu
        XMFLOAT3 wheelFL_;  //���΂ߑO�@�^�C���̈ʒu
        XMFLOAT3 wheelRR_;  //�E�΂ߌ��@�^�C���̈ʒu
        XMFLOAT3 wheelRL_;  //���΂ߌ��@�^�C���̈ʒu
        float wheelHeight_; //�^�C���̍���
    }Size;

public:
    //�R���X�g���N�^
    Vehicle(GameObject* parent);

    //�f�X�g���N�^
    virtual ~Vehicle(); //���������z���ă��@�[�`����

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

    //�����x�̐��� vector
    void SpeedLimit(XMVECTOR& speed, const float limit);
    //�����x�̐��� xmfloat3
    void SpeedLimit(XMFLOAT3& speed, const float limit);

    //�n���h���p�x�̐����̂��߂�
    //�i�ύX�������p�x�A�ő�p�x�j
    //�ύX�������p�x�̐�Βl���ő�p�x�̐�Βl�𒴂��Ă�����ۂߍ���
    void AngleLimit(float& angle, const float limit);

    //�ǂ⏰�Ƃ̐ڐG
    void VehicleCollide();

    /// <summary>
    /// �������邩���ׁA������ڒn
    /// </summary>
    /// <param name="hModel">���f���ԍ�</param>
    /// <param name="type">���f���̎��</param>
    void Landing(int hModel , int type);

    /// <summary>
    /// �ǂƂ̐ڐG
    /// </summary>
    /// <param name="hModel">���f���ԍ�</param>
    void CollideWall(int hModel, int type);

    /// <summary>
    /// �^�C����p��
    /// </summary>
    /// <param name="hModel">���f���ԍ�</param>
    void MakeWheels(int hModel);

    //�^�C���̍����Z�b�^�[
    void SetWheelHeight(float height) { Size.wheelHeight_ = height; }
    //���f���̑傫���Z�b�^�[
    void SetVehicleSize(int hModel,std::string modelName);

    //�ԗ��̑���A���͂̎�t
    void InputOperate();

    enum
    {
        right =  1,
        left  = -1
    }handleLR_;

    //�n���h���̑���
    void HandleTurnLR(int LR);

    template <class V>
    V* VehicleInstantiate(GameObject* pParent)
    {
        V* pNewObject = new V(pParent);
        if (pParent != nullptr)
        {
            pParent->PushBackChild(pNewObject);
        }
        pNewObject->Initialize();
        return pNewObject;
    }

};