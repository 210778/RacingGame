#pragma once
#include "Vehicle.h"

class Speedometer;
class Text;
class Sample;
class Viewer;
class ImagePrinter;

//���������Ǘ�����N���X
class VehiclePlayer : public Vehicle
{
    //�X�s�[�h���[�^�[
    Speedometer* pSpeedometer_;

    //�����V���[�Y
    Text* pTextSpeed_;  //����
    Text* pTextTime_;   //�o�ߎ���
    Text* pTextLap_;    //����
    Text* pTextRanking_;    //����
    Text* pTextAcceleration_; //�����x�\�� �f�o�b�O�p
    float km_hAdd;  //�����\�����Z
    int   flashIntervalUI_;   //�\�����_�ł���Ԋu
    bool  IsFlashUI_;         //UI��\�����邩�ǂ���
    Sample* pSample_;

    //�u�[�X�g�n
    int imageBoostMax_;
    int imageBoost_;

    //�摜�\���N���X
    ImagePrinter* pImagePrinter_;
    std::map<std::string, bool> isPrintedMap_;

public:
    //�R���X�g���N�^
    VehiclePlayer(GameObject* parent);

    //�R���X�g���N�^2
    VehiclePlayer(GameObject* pParent, std::string vehicleName, std::string wheelName);

    //�f�X�g���N�^
    ~VehiclePlayer();

    //�J��
    void Release() override;

    //�v���C���[����Ŏ��s����֐�  
    //UI�̏�����
    void PlayerUI_Initialize() override;
    //UI�̕\��
    void PlayerUI_Draw() override;
    //UI�̏��X�V
    void PlayerUI_Update() override;
    //�J�����̗p��
    void PlayerCamera_Initialize() override;
    //�J�����X�V
    void PlayerCamera_Update() override;

    //����
    void InputOperate() override;

#if 0
    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�����ɓ�������
    //�����FpTarget ������������
    void OnCollision(GameObject* pTarget) override;
#endif
};