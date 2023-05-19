#pragma once
#include "Engine/GameObject.h"

class Sample;
class Tachometer;
class Course;
class Speedometer;
class Text;
//class CircuitUnion;
class Particle;

//���������Ǘ�����N���X
class Vehicle : public GameObject
{
    int hModel_;    //���f���ԍ�
    int hGroundModel_;  //�n�ʂ̃��f���ԍ�


    //XMFLOAT3 acceleration_; //�����x
    XMVECTOR acceleration_; //�����x

    //���f���̑傫��
    XMFLOAT3 vehicleSize_;
    XMFLOAT3 vehicleSizeHalf_;
    float vehicleSizeOblique_;//�΂�

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

    //�e�X�g�@�f�o�b�O�p
    Sample* pMarker;
    Tachometer* pTachometer;
    Speedometer* pSpeedometer;

    //�����V���[�Y
    Text* pTextSpeed_;  //����
    Text* pTextTime_;   //�o�ߎ���
    Text* pTextLap_;    //����
    Text* pTextEngine_; //�G���W����]��

    unsigned long long time_; //�o�߃^�C��

    int pointCount_;
    int lapCount_;
    bool goalFlag_;
    int hImage_;

    //�ǉ��ϐ�
    float torque_;   //�g���N
    float mass_;     //�Ԃ̏d��
    float engineRotate_;    //�G���W����]��
    bool clutchFlag_;   //�N���b�`���q�����Ă邩

    //�T�[�L�b�g�p
    //CircuitUnion* pCircuitUnion;

    XMVECTOR frontVec_;//���ʂ̃x�N�g��

    int landingType_; //�n�ʂ̃^�C�v

    Particle* pParticle;

public:
    //�R���X�g���N�^
    Vehicle(GameObject* parent);

    //�f�X�g���N�^
    ~Vehicle(); //���������z���ă����[�`������ ??

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

    //�܂Ƃ߂��ق�����������
    void VehicleCollide();

    //�ڒn
    void Landing();

    //�ǂƂ̐ڐG
    void CollideWall();
};