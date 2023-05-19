#pragma once
#include "Engine/GameObject.h"

//���������Ǘ�����N���X
class Tachometer : public GameObject
{
    int hModel_;    //���f���ԍ�
    int hImage_;

    
    int nowHp_; //���݂̂g�o
    int maxHp_; //�ő�g�o

    float stopRatio_;       //�ő�HP�ɑ΂���Œ肳�ꂽ�Q�[�W�̊���
    float moveRatio_;       //�ő�HP�ɑ΂���ω����Ă�Q�[�W�̊���
    const float SPEED;      //�o�[�̑��x

    //�g�p����摜�̎��
    enum
    {
        PICT_FRAME,
        PICT_GREEN,
        PICT_LGREEN,
        PICT_RED,
        PICT_MAX
    };

    //�摜�ԍ�
    int hPict_[PICT_MAX];

public:
    //�R���X�g���N�^
    Tachometer(GameObject* parent);

    //�f�X�g���N�^
    ~Tachometer();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    //�_���[�W���󂯂�
    //�����Fvalue  �_���[�W��
    void Damage(int value);

    //��
    //�����Fvalue  �񕜗�
    void Recovery(int value);
};