#include "Circuit.h"
#include "CheckPoint.h"

namespace Circuit
{
    //�R�[�X���i
    struct CircuitParts
    {
        int model_; //���f���ԍ�
        int type_;  //���
        //�����Ȃ�
        CircuitParts(){
            model_ = -1;
            type_ = 0;
        }
        //�R���X�g���N�^�i���f���ԍ��A��ށj
        CircuitParts(int m, int t){
            model_ = m;
            type_ = t;
        }
    };
    //�`�F�b�N�|�C���g�^
    struct CircuitCheckPoint
    {
        CheckPoint* pCP_;
        XMFLOAT3    CP_position_;
        float       CP_Radius_;
        CircuitCheckPoint(){
            pCP_ = nullptr;
            CP_position_ = { 0.0f,0.0f,0.0f };
            CP_Radius_ = 0.0f;
        }
        CircuitCheckPoint(CheckPoint* poi, const XMFLOAT3& pos, float rad){
            pCP_ = poi;
            CP_position_ = pos;
            CP_Radius_ = rad;
        }
    };
    //�R�[�X�P��
    struct CircuitUnion
    {
        std::vector<CircuitParts> parts_;   //�R�[�X�̃p�[�c����
        std::string  name_;                 //�R�[�X�̖��O
        int maxLap_;                        //�K�v����
        std::vector<CheckPoint*> checkPoint_;   //�`�F�b�N�|�C���g�̃|�C���^
        std::vector<CircuitCheckPoint> check_;
        std::vector<Transform> startTransform_; //�X�^�[�g�n�_�̃g�����X�t�H�[��(�ʒu�Ɖ�])
        //�����Ȃ��R���X�g���N�^
        CircuitUnion() {
            name_ = "circuit";
            maxLap_ = 1;
        };
        //���O�R���X�g���N�^
        CircuitUnion(std::string name) {
            name_ = name;
            maxLap_ = 1;
        };
        //���O,���񐔃R���X�g���N�^
        CircuitUnion(std::string name, int lap) {
            name_ = name;
            maxLap_ = lap;
        };
    };

    std::vector<CircuitUnion> circuits_;    //�R�[�X���ׂĂ̂܂Ƃ�

    int chosenCircuit_;             //�I�����ꂽ�R�[�X
    float defaultCheckpointSize_;   //�`�F�b�N�|�C���g�̑傫��
    float defaultStartRotate_;      //�X�^�[�g���̉�]
    float checkPointLimit_;         //�`�F�b�N�|�C���g��T�����E

    std::map<circuitType, std::string> circuitTypeMap_; //unordered_map �ł̓o�O��?

}

//������
void Circuit::Initialize()
{
    chosenCircuit_ = 0;
    defaultCheckpointSize_ = 60.f;
    defaultStartRotate_ = 0.f;
    checkPointLimit_ = 500.f;

    circuitTypeMap_[circuitType::abyss] = "abyss";
    circuitTypeMap_[circuitType::boost] = "boost";
    circuitTypeMap_[circuitType::dirt] = "dirt";
    circuitTypeMap_[circuitType::ice] = "ice";
    circuitTypeMap_[circuitType::other] = "other";
    circuitTypeMap_[circuitType::road] = "road";
    circuitTypeMap_[circuitType::turf] = "turf";
}