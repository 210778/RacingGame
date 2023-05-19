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
        CircuitParts() {};
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
        std::vector<CircuitParts> parts_;    //�R�[�X�̃p�[�c����
        std::string  name_;     //�R�[�X�̖��O
        int maxLap_;            //�K�v����
        std::vector<XMFLOAT3> checkPointPosition_;

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

    //�`�F�b�N�|�C���g�̓����蔻��
    std::vector<CheckPoint*> checkPoint_;
    
public:

    enum circuitType
    {
        road = 0,   //�ʏ퓹�H 
        turf,       //�Ő�
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

    //�R�[�X�I��
    void SetChosenCircuit(int i) { chosenCircuit_ = i; }
    int  GetChosenCircuit() { return chosenCircuit_; }
    //�R�[�X��I�����āA�����蔻����Z�b�g�B���łɂ��铖���蔻��͏���(�����F�R�[�X�ԍ�)
    void SetChosenCircuitCheckPoint(int value);

    //�g�p���Ă���R�[�X�̃|�C���^
    CircuitUnion* GetCircuitUnion()
    {
        if (chosenCircuit_ <= 0 && chosenCircuit_ < circuits_.size())
            return &circuits_[chosenCircuit_];
        else
            return nullptr;
    }
};