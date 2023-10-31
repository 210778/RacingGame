#pragma once
#include "Vehicle.h"

class Text;
class Sample;
class Viewer;

//���������Ǘ�����N���X
class VehiclePlayer : public Vehicle
{
    //����
    Text* pText_;    //����

    float km_hAdd;  //�����\�����Z
    int   flashIntervalUI_;   //�\�����_�ł���Ԋu
    bool  IsFlashUI_;         //UI��\�����邩�ǂ���
    Sample* pSample_;

    //�u�[�X�g�n
    int imageBoostMax_;
    int imageBoost_;
    Transform boostMaxTrans_;
    Transform boostTrans_;

    //�X�s�[�h���[�^�[
    int hImageSpeedFrame_;
    int hImageSpeedNeedle_;
    Transform SpeedmeterTransform_;

    unsigned long long drawTime_;//�`��Ɏg�����Ԃ̒l
    int standardTime_;  //�`��Ɏg��FPS�̒l

    //�J�E���g�_�E���\��
    struct ImageData
    {
        int imageHandle_;   //�摜�ԍ�
        Transform imageTransform_;  //�摜�g�����X�t�H�[��
        int imageAlpha_;  //�s�����x�@�@�����@0 ~ 255�@�s����

        int lifeTime_;   //�\�����邷�鎞��(�P�ʁF�t���[��)
        int life_;       //�\�����Ă��鎞��

        bool isPrint_;   //�\�����邩�ǂ���
        bool isAlreadyPrint_;   //���ɕ\���������ǂ���

        Transform changeTransform_; //�g�����X�t�H�[���̕ω�
        int changeAlpha_;   //�s�����x�̕ω�

        enum class ImageNumber
        {
            start,
            one,
            two,
            three,
            goal
        };
        ImageData()
        {
            imageHandle_ = -1;
            imageAlpha_ = 255;
            lifeTime_ = 0;
            life_ = 0;
            isPrint_ = false;
            isAlreadyPrint_ = false;
            changeAlpha_ = 0;
        }
        ImageData(int handle, Transform transform, int time
            , Transform cTransform, int alpha = 255, int cAlpha = 0)
        {
            imageHandle_ = handle;
            imageTransform_ = transform;
            imageAlpha_ = alpha;
            lifeTime_ = time;
            life_ = 0;
            isPrint_ = false;
            isAlreadyPrint_ = false;
            changeTransform_ = cTransform;
            changeAlpha_ = cAlpha;
        }
    }imageData_;
    //�摜���Z�b�g
    void SetImage(ImageData::ImageNumber in, int handle, Transform transform, int time
        , Transform cTransform, int alpha = 255, int cAlpha = 0);
    //�摜�`��
    void DrawImage(ImageData::ImageNumber in);
    //�摜�܂Ƃ�
    std::map<ImageData::ImageNumber, ImageData> imageMap_;

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

    //�X�s�[�h���[�^�[�`��
    void DrawSpeedmeter();
    //�����`��
    void DrawKmH();
    //�o�ߎ��ԕ`��
    void DrawElapsedTime();
    //���ʕ\��
    void DrawRanking();
    //�u�[�X�g�\��
    void DrawBoostGauge();
    //�f�o�b�O�F�����x�Ɖ�]�\��
    void DrawAccelerationRotate();
    //�J�E���g�_�E���`��
    void DrawStandbyCount();
};