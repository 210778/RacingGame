#pragma once
#include "Engine/GameObject.h"

class Particle;
class Ground;
class VehicleWheel;

//�ԗ����Ǘ�����N���X
class Vehicle : public GameObject
{
protected:
    //���f���ԍ��n
        int hModel_;    //���f���ԍ�
        int hGroundModel_;  //�n�ʂ̃��f���ԍ�
        int hWheelModel_;

    //�d�v�x�N�g���n
        //�����x
        XMVECTOR acceleration_;
        //���ꂼ��̎��̒P�ʃx�N�g���@�萔
        const XMVECTOR VectorX_;    //X���P�ʃx�N�g��
        const XMVECTOR VectorY_;    //Y���P�ʃx�N�g��
        const XMVECTOR VectorZ_;    //Z���P�ʃx�N�g��
        //�ԗ����猩���e���̒P�ʃx�N�g��
        XMVECTOR vehicleVectorX_;   //�ԗ����猩��X���P�ʃx�N�g��
        XMVECTOR vehicleVectorY_;   //�ԗ����猩��Y���P�ʃx�N�g��
        XMVECTOR vehicleVectorZ_;   //�ԗ����猩��Z���P�ʃx�N�g��

    //�ړ��A��]�̑���
    float moveSPD_;
    float rotateSPD_;
    float jumpForce_;

    const float gravity_;  //�d�� �萔
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

    //���[�X�n
        unsigned long long time_; //�o�߃^�C��
        int pointCount_;    //�`�F�b�N�|�C���g�o�ߐ�
        int pointCountMax_; //�K�v�ȃ`�F�b�N�|�C���g�o�ߐ�
        int lapCount_;      //����
        int lapMax_;        //�K�v�Ȏ���
        bool goalFlag_;     //true�Ȃ�S�[�����
        int ranking_;       //�����̏���
        int population_;    //���[�X�ɎQ�����Ă���l��

    float mass_;     //�Ԃ̏d��
    float engineRotate_;    //�G���W����]��

    XMVECTOR frontVec_;//���ʂ̃x�N�g��

    int landingType_; //�n�ʂ̃^�C�v

    Particle* pParticle_;//�G�t�F�N�g�p

    Ground* pGround_;//�n�ʂ̃|�C���^�[

    VehicleWheel* pWheels_;//�^�C���̃|�C���^�[

    float wheelSpeedAdd_; //�����x�ɉ��Z����^�C���̉�]

    short accZDirection_; //�O�����ɐi��ł邩���������B�O�F+1, ��F-1

    float wheelParticleLength_;//�^�C���̃G�t�F�N�g����������x�N�g���̒���

    XMFLOAT3 startPosition_;    //�J�n���̈ʒu
    XMFLOAT3 startRotate_;      //�J�n���̉�]
    std::string vehicleModelName_;  //�ԗ����f���̖��O
    std::string wheelModelName_;    //�^�C�����f���̖��O

    Transform startTransform_;  //�X�^�[�g��Ԃ̈ʒu�Ɖ�]

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

    const short handleRight_;
    const short handleLeft_;

    //���C�L���X�g�̎�����
    enum Direction
    {
        front = 0,
        right = 1,
        rear = 2,
        left = 3,
    };

public:
    //�R���X�g���N�^
    Vehicle(GameObject* parent);

    //�p���p
    Vehicle(GameObject* parent, const std::string& name);

    //�f�X�g���N�^
    virtual ~Vehicle(); //���@�[�`����

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
    /// <returns>�Ȃɂ��ɓ��������ꍇtrue</returns>
    bool Landing(int hModel , int type);

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
    void SetVehicleSize(int hModel);

    //�ԗ��̑���A���͂̎�t
    virtual void InputOperate();



    //�n���h���̑���
    void HandleTurnLR(int LR);

    //�X�^�[�g�̈ʒu
    void SetStartTransform(Transform value) { startTransform_ = value; }

    //���ʔ���n�Z�b�^�[�E�Q�b�^�[
        //�`�F�b�N�|�C���g�ʉߐ����擾
        int GetPointCount() { return pointCount_; }
        //�K�v�ȃ`�F�b�N�|�C���g�o�ߐ����Z�b�g
        void SetPointCountMax(int value) { pointCountMax_ = value; }
        //���񐔂��擾
        int GetLapCount() { return lapCount_; }
        //�K�v�Ȏ��񐔂��擾
        void SetLapMax(int value) { lapMax_ = value; }
        //���̃`�F�b�N�|�C���g�̈ʒu���擾�@�������z����I�[�o�[���Ă���O�̈ʒu��Ԃ�
        XMFLOAT3* GetNextCheckPosition();
        //���̃`�F�b�N�|�C���g�܂ł̋������擾
        float GetNextCheckDistance();
        //���ʂ��Z�b�g
        void SetRanking(int value) { ranking_ = value; }
        //���[�X�Q���l�����Z�b�g
        void SetPopulation(int value) { population_ = value; }

    //�v���C���[����Ŏ��s����֐�  
        //UI�̏�����
        virtual void PlayerUI_Initialize();
        //UI�̕\��
        virtual void PlayerUI_Draw();
        //UI�̏��X�V
        virtual void PlayerUI_Update();
        //�J�����̗p��
        virtual void PlayerCamera_Initialize();
        //�G�t�F�N�g��\��(�d���Ȃ�̂�NPC�͂��Ȃ�)
        virtual void PlayerParticle();
};