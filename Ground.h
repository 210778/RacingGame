#pragma once
#include "Engine/GameObject.h"

class CheckPoint;

//���������Ǘ�����N���X
class Ground : public GameObject
{
    int hModel_;    //���f���ԍ�

    //std::vector<int> circuit;

    //�R�[�X���i
    struct CircuitParts
    {
        int model_; //���f���ԍ�
        int type_;  //���

        //�����Ȃ�
        CircuitParts()
        {
            model_ = -1;
            type_ = 0;
        }
        //�R���X�g���N�^�i���f���ԍ��A��ށj
        CircuitParts(int m, int t)
        {
            model_ = m;
            type_ = t;
        }
    };

    //�R�[�X�P��
    struct CircuitUnion
    {
        std::vector<CircuitParts> parts_;   //�R�[�X�̃p�[�c����
        std::string  name_;                 //�R�[�X�̖��O
        int maxLap_;                        //�K�v����
        std::vector<CheckPoint*> checkPoint_;   //�`�F�b�N�|�C���g�̃|�C���^
        std::vector<Transform> startTransform_;              //�X�^�[�g�n�_�̃g�����X�t�H�[��(�ʒu�Ɖ�])

        //�����Ȃ��R���X�g���N�^
        CircuitUnion(){
            name_ = "circuit";
            maxLap_ = 3;
        };
        //���O�R���X�g���N�^
        CircuitUnion(std::string name) {
            name_ = name;
            maxLap_ = 3;
        };
        //���O,���񐔃R���X�g���N�^
        CircuitUnion(std::string name,int lap) {
            name_ = name;
            maxLap_ = lap;
        };
    };

    std::vector<CircuitUnion> circuits_;    //�R�[�X���ׂĂ̂܂Ƃ�

    //�I�����ꂽ�R�[�X
    int chosenCircuit_;

    float defaultCheckpointSize_;   //�`�F�b�N�|�C���g�̑傫��

    float defaultStartRotate_;      //�X�^�[�g���̉�]
    
public:

    enum circuitType
    {
        road = 0,   //�ʏ퓹�H 
        turf,       //�Ő�
        abyss,      //�ޗ�
        other,      //�ǂ�ł��Ȃ�
        circuitMax  //�I�_
    };

    //�R���X�g���N�^
    Ground(GameObject* parent);

    //�f�X�g���N�^
    ~Ground();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;



    //�Z�b�^�[�ƃQ�b�^�[
    int GetModelHandle()
    { 
        return hModel_;
    }

    //�ԍ����Z�b�g���Ĕz��̒�����R�[�X�I��
    void SetChosenCircuit(int i) { chosenCircuit_ = i; }
    //�I�������R�[�X�̔ԍ����擾
    int  GetChosenCircuit() { return chosenCircuit_; }

    //�g�p���Ă���R�[�X�̃|�C���^ ���S�΍􂳂�Ă��āA���s������nullptr
    CircuitUnion* GetCircuitUnion()
    {
        if (chosenCircuit_ <= 0 && chosenCircuit_ < circuits_.size())
            return &circuits_[chosenCircuit_];
        else
            return nullptr;
    }

    //�`�F�b�N�|�C���g��T���ăZ�b�g
    void MakeCheckPoint();

    //�X�^�[�g�n�_��T���ăZ�b�g
    void MakeStartPoint();

    //���̃`�F�b�N�|�C���g�̈ʒu��Ԃ�
    //�������T�C�Y���I�[�o�[���Ă�΂O�Ԃ̈ʒu��Ԃ�
    XMFLOAT3 NextCheckPointPosition(int number);
};