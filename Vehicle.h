#pragma once
#include <unordered_map>
#include "Engine/GameObject.h"


class Particle;
class Ground;
class VehicleWheel;
class RayCastData;

//�ԗ����Ǘ�����N���X
class Vehicle : public GameObject
{
protected:
    //���f���ԍ��n
        int hModel_;    //���f���ԍ�
        int hGroundModel_;  //�n�ʂ̃��f���ԍ�
        int hWheelModel_;

    //�d�v�x�N�g���n
        //�x�N�g���R��
        struct TriVector
        {
            XMVECTOR x, y, z;
            //�[���ŏ������R���X�g���N�^
            TriVector() {
                x = { 0.0f,0.0f,0.0f,0.0f };
                y = { 0.0f,0.0f,0.0f,0.0f };
                z = { 0.0f,0.0f,0.0f,0.0f };}
            //��������R���X�g���N�^
            TriVector(XMVECTOR vecX,XMVECTOR vecY,XMVECTOR vecZ) {
                x = vecX;
                y = vecX;
                z = vecX;}
            //�Z�b�^�[
            void Set(XMVECTOR vecX, XMVECTOR vecY, XMVECTOR vecZ) {
                x = vecX;
                y = vecY;
                z = vecZ;}
        };
        //�����x
        XMVECTOR acceleration_;
        //���ꂼ��̎��̒P�ʃx�N�g���@�萔
        TriVector worldVector_;
        //�ԗ����猩���e���̒P�ʃx�N�g��
        TriVector vehicleVector_;

    //��]�p�s��
    XMMATRIX matRotateX;
    XMMATRIX matRotateY;
    XMMATRIX matRotateZ;

    XMMATRIX matRotateX_R;
    XMMATRIX matRotateY_R;
    XMMATRIX matRotateZ_R;


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
        float iceFriction_;         //�X���C

        bool  slideFlag_;           //���s�A����
        float sideWheelFriction_;   //�������̖��C
        float sideSlideFriction_;   //�������󒆖��C
        float sideIceFriction_;     //�������X���C

        float landingFriction_;
        float sideFriction_;

        //�n�ʂ̃^�C�v�ɂ�門�C�̂܂Ƃ�
        struct GroundTypeValue
        {
            float acceleration;   //�����͂ɏ�Z�����l
            float landing;    //���s���C
            float side;       //�J�[�u���ɉ������ɉ�����

            //�����Ȃ��R���X�g���N�^
            GroundTypeValue(){
                acceleration = 1.0f;
                landing = 0.99f;
                side = 0.2f;
            }
            //��������R���X�g���N�^
            GroundTypeValue(float _acceleration, float _landing, float _side) {
                acceleration = _acceleration;
                landing = _landing;
                side = _side;
            }
        };
        std::unordered_map<int, GroundTypeValue> GroundTypeFriction_;    //�n�ʂ̃^�C�v�ɂ�門�C�̂܂Ƃ�

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
        int goalRanking_;   //�S�[���������̏��� �S�[���O��0
        int population_;    //���[�X�ɎQ�����Ă���l��
        unsigned long long goalTime_;//�S�[���������̌o�߃^�C��

    float mass_;            //�Ԃ̏d��
    float engineRotate_;    //�G���W����]��

    XMVECTOR frontVec_;//���ʂ̃x�N�g��

    int landingType_; //�n�ʂ̃^�C�v

    Particle* pParticle_;//�G�t�F�N�g�p

    Ground* pGround_;//�n�ʂ̃|�C���^�[

    VehicleWheel* pWheels_;//�^�C���̃|�C���^�[

    float wheelSpeedAdd_; //�����x�ɉ��Z����^�C���̉�]

    short accZDirection_; //�O�����ɐi��ł邩���������B�O�F+1, ��F-1

    float wheelParticleLength_;//�^�C���̃G�t�F�N�g����������x�N�g���̒���
    float wheelParticleLengthMax_;//�^�C���̃G�t�F�N�g����������x�N�g���̒����̏��

    std::string vehicleModelName_;  //�ԗ����f���̖��O
    std::string wheelModelName_;    //�^�C�����f���̖��O

    Transform startTransform_;      //�X�^�[�g��Ԃ̈ʒu�Ɖ�]
    Transform restartTransform_;    //�����������̈ʒu�Ɖ�]

    float slopeLimitAngle_; //�⓹�����Ƃ��̏���p�x�i���������猩��j���̒l�𒴂���ƃo�O�肪��

    float wallReflectionForce_;//�ǂɂԂ������Ƃ��̌�������l

    const short handleRight_;
    const short handleLeft_;

    float boostCapacityMax_;//�u�[�X�g�ł���ő�e��
    float boostCapacity_;//�u�[�X�g�ł��錻�ݎc��
    float boostSpending_;//�u�[�X�g���鎞�̏����
    float boostIncrease_;//�u�[�X�g�ł���c�ʂ̒ǉ��l

    bool isPlayer_; //�v���C���[�L�������ǂ���

    float collideBoxValue_;//����ƂԂ������Ƃ��̊|����l

    bool isOperationInvalid_;   //����𖳌�������

    //�ԗ��̊e�T�C�Y
    struct
    {
        float toRight_ = 1.0f;  //�E�[�@�܂ł̋���
        float toLeft_ = 1.0f;   //���[�@�܂ł̋���
        float toFront_ = 1.0f;  //�O���@�܂ł̋���
        float toRear_ = 1.0f;   //����@�܂ł̋���
        float toTop_ = 1.0f;    //��[�@�܂ł̋���
        float toBottom_ = 1.0f; //���[�@�܂ł̋���
        float rightToLeft_ = 1.0f;  //�E�[�@����@���[�@�܂ł̋���
        float topToBottom_ = 1.0f;  //��[�@����@���[�@�܂ł̋���
        float frontToRear_ = 1.0f;  //�O���@����@����@�܂ł̋���
        float toFrontRight_ = 1.0f; //�E�΂ߑO�@�܂ł̋���
        float toFrontLeft_ = 1.0f;  //���΂ߑO�@�܂ł̋���
        float toRearRight_ = 1.0f;  //�E�΂ߌ��@�܂ł̋���
        float toRearLeft_ = 1.0f;   //���΂ߌ��@�܂ł̋���     
        float angleFrontRight_ = 0.0f; //�O������@�E�΂ߑO�@�܂ł̊p�x
        float angleFrontLeft_ = 0.0f;  //�O������@���΂ߑO�@�܂ł̊p�x
        float angleRearRight_ = 0.0f;  //�O������@�E�΂ߌ��@�܂ł̊p�x
        float angleRearLeft_ = 0.0f;   //�O������@���΂ߌ��@�܂ł̊p�x
        XMFLOAT3 wheelFR_;      //�E�΂ߑO�@�^�C���̈ʒu
        XMFLOAT3 wheelFL_;      //���΂ߑO�@�^�C���̈ʒu
        XMFLOAT3 wheelRR_;      //�E�΂ߌ��@�^�C���̈ʒu
        XMFLOAT3 wheelRL_;      //���΂ߌ��@�^�C���̈ʒu
        float wheelHeight_ = 0.1f;      //�^�C���̍���
        float wheelRemainder_ = 1.0f;   //�ԑ̂ƃ^�C���̈ʒu�̍����̍���
        float toWheelBottom_ = 1.0f;    //���S����^�C���̒�ӂ܂ł̍���
        float topToWheelBottom_ = 1.0f; //��[����^�C����ӂ܂ł̍���
        float centerRightToLeft_ = 1.0f;    //���E�̒����̒��S
        float centerTopToBottom_ = 1.0f;    //�㉺�̒����̒��S(�^�C���̍�������)
        float centerFrontToRear_ = 1.0f;    //�O��̒����̒��S
        float centerPositionRemainder_ = 1.0f; //�Փ˃{�b�N�X��Y���̒��S�܂ł̋���
    }Size;

    //�������
    struct
    {
        enum inputName
        {
            moveFront = 0,
            moveRear,
            handleRight,
            handleLeft,
            boost,

            jump,
            turnRight,
            turnLeft,
            moveRight,
            moveLeft,

            MAX
        };

        //���݂̒l�A�P�ߋ��̒l
        std::unordered_map<int,float> inputNow,inputAgo;

        //���݂̒l���ߋ��ɓn���ă[���ɂ���
        void Refresh()
        {
            inputAgo = inputNow;

            //���Z�b�g
            for (auto& itr : inputNow){
                itr.second = 0.0f;
            }
        }
        //���͉����Ă��āA�O�ł͉����ĂȂ���
        float IsDown(inputName in)
        {
            if (inputAgo[in] == false)
                return inputNow[in];

            return 0.0f;
        }
        //���͉����ĂȂ��āA�O�͉����Ă�����
        float IsUp(inputName in)
        {
            if (inputAgo[in] == false)
                return 0.0f;

            return inputNow[in];
        }

    }Operation;

    //NPC�̂��߂̃��C�������������ǂ����̏��܂Ƃ�
    struct RayCastHit
    {
        XMFLOAT3	dir;	//���C�̌���float3
        float       dist;	//�Փ˓_�܂ł̋���
        BOOL        hit;	//���C������������
        XMFLOAT3    end;	//�����ʒu

        enum Number
        {
            up,
            down,
            front,
            frontLeft,
            left,
            rearLeft,
            rear,
            rearRight,
            right,
            frontRight,
        };
    };
    std::unordered_map<int, RayCastHit> rayCastHit_;


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

    //�����x���ʒu�ɉ��Z����B�A�b�v�f�[�g�̍ŏ��ɂ����Ƃ� ���肾����
    void Accelerator();
    //�����x���擾
    XMVECTOR GetAcceleration() { return acceleration_; };

    //�����x�̐��� vector
    void SpeedLimit(XMVECTOR& speed, const float limit);
    //�����x�̐��� xmfloat3
    void SpeedLimit(XMFLOAT3& speed, const float limit);

    //�n���h���p�x�̐����̂��߂�
    //�i�ύX�������p�x�A�ő�p�x�j
    //�ύX�������p�x�̐�Βl���ő�p�x�̐�Βl�𒴂��Ă�����ۂߍ���
    void AngleLimit(float& angle, const float limit);

    //�^�C������]������Ƃ��̕���
    void TurnWheel();

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
    /// <returns>�Ȃɂ��ɓ��������ꍇtrue</returns>
    bool CollideWall(int hModel, int type);

    /// <summary>
    /// �^�C����p��
    /// </summary>
    /// <param name="hModel">���f���ԍ�</param>
    void MakeWheels(int hModel);

    //�^�C���̍����Z�b�^�[ //�ԑ̂Ƃ̍������v�Z
    void SetWheelHeight(float height);

    int hSound_ = -1;


    //���f���̑傫���Z�b�^�[
    void SetVehicleSize(int hModel);

    //�ԗ��̑���A���͂̎�t
    virtual void InputOperate();

    //�n���h���̑���
    void HandleTurnLR(int LR);

    //�X�^�[�g�̈ʒu
    void SetStartTransform(const Transform& value) { startTransform_ = value; }

    //�x�N�g������]�s��ŉ�] (ZXY��)
    void VectorRotateMatrixZXY(XMVECTOR& vec);

    //�x�N�g������]�s���"�t"��] (ZXY��)
    void VectorRotateMatrixZXY_R(XMVECTOR& vec);

    //�n�ʁA�ǂ̎ԗ��̎p���𓝍�����
    void VehicleRotateTotal(std::vector<XMFLOAT3>* rotate);

    /// <summary>
    /// �⓹�ɉ����Ďԗ�����](X�AZ��)
    /// </summary>
    /// <param name="normal">�⓹�̖@���i���K���ρj</param>
    /// <param name="limitAngle">��]�����e����⓹�̊p�x(���������猩��)</param>
    /// <returns>���E�p�x�ȓ��Ȃ��]����true �����łȂ��Ȃ牽������false</returns>
    bool VehicleRotateSlope(const XMVECTOR& normal,const float limitAngle);

    /// <summary>
    /// NPC�p���C�L���X�g�Ƀf�[�^�Z�b�g
    /// </summary>
    /// <param name="number">map�̃L�[</param>
    /// <param name="rcd">���C�L���X�g�f�[�^</param>
    void SetRayCastHit(int number, const RayCastData& rcd);

    /// <summary>
    /// BoundingOrientedBox�̏Փ˔���
    /// </summary>
    /// <param name="pVehicle">���������ԗ��N���X�̃|�C���^</param>
    void CollideBoundingBox(Vehicle* pVehicle);

    //������͂̔��f
    void InputReceive(const XMVECTOR& vecX, const XMVECTOR& vecZ);

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
        //�o�E���f�B���O�{�b�N�X�̑傫���̃Q�b�^�[
        XMFLOAT3 GetBoundingBoxExtents() { return XMFLOAT3{ Size.centerRightToLeft_
                                                          , Size.centerTopToBottom_
                                                          , Size.centerFrontToRear_ }; }
        //�o�E���f�B���O�{�b�N�X�̒��S�̈ʒu�̃Q�b�^�[
        XMFLOAT3 GetBoundingBoxCenter() {
            return XMFLOAT3{ transform_.position_.x
                            ,transform_.position_.y + Size.centerPositionRemainder_
                            ,transform_.position_.z };
        }
        //�ԗ��̎��ʂ̃Q�b�^�[
        float GetMass() { return mass_; };
        //�ԗ��̎��ʂ̃Z�b�^�[
        void SetMass(float value) { mass_ = value; };
        //�S�[�����Ă�Ȃ�true�ƃ|�C���^�ɏ��ʂ��A���ĂȂ��Ȃ�false��Ԃ��ĂȂɂ����Ȃ�
        bool GetIsGoalRanking(int* pRank)
        {
            if (goalFlag_)
            {
                *pRank = goalRanking_;
                return true;
            }
            return false;
        }
        //�o�ߎ��Ԃ̃Z�b�^�[
        void SetTime(unsigned long long time) { time_ = time; }
        //����𖳌������邩�ǂ����̃Z�b�^�[
        void SetOperationInvalid(bool value) { isOperationInvalid_ = value; }
        

    //�v���C���[����Ŏ��s����֐�  
        //UI�̏�����
        virtual void PlayerUI_Initialize();
        //UI�̕\��
        virtual void PlayerUI_Draw();
        //UI�̏��X�V
        virtual void PlayerUI_Update();
        //�J�����̗p��
        virtual void PlayerCamera_Initialize();
        //�J�����X�V
        virtual void PlayerCamera_Update();
        //�G�t�F�N�g��\��(�d���Ȃ�̂�NPC�͂��Ȃ�)
        virtual void PlayerParticle();
};