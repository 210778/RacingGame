#include "Engine/Model.h"
#include "Circuit.h"
#include "CheckPoint.h"
#include "Ground.h"


using std::string;
using std::to_string;
using std::stoi;
using std::vector;

namespace Circuit
{
    //�R�[�X���i
    struct CircuitParts
    {
        std::string modelName_;  //���f����
        int model_; //���f���ԍ�
        int type_;  //���

        //�����Ȃ�
        CircuitParts()
        {
            modelName_ = "";
            model_ = -1;
            type_ = 0;
        }
        //�R���X�g���N�^�i���f�����A���f���ԍ��A��ށj
        CircuitParts(std::string n, int m, int t)
        {
            modelName_ = n;
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
        CircuitCheckPoint()
        {
            pCP_ = nullptr;
            CP_position_ = { 0.0f,0.0f,0.0f };
            CP_Radius_ = 0.0f;
        }
        CircuitCheckPoint(CheckPoint* poi, const XMFLOAT3& pos, float rad)
        {
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
        std::vector<Transform> startTransform_;              //�X�^�[�g�n�_�̃g�����X�t�H�[��(�ʒu�Ɖ�])

        //�����Ȃ��R���X�g���N�^
        CircuitUnion() {
            name_ = "circuit";
            maxLap_ = 3;
        };
        //���O�R���X�g���N�^
        CircuitUnion(std::string name) {
            name_ = name;
            maxLap_ = 3;
        };
        //���O,���񐔃R���X�g���N�^
        CircuitUnion(std::string name, int lap) {
            name_ = name;
            maxLap_ = lap;
        };
    };

    //�R�[�X�̎��
    enum circuitType
    {
        road = 0,   //�ʏ퓹�H 
        turf,       //�Ő�
        abyss,      //�ޗ�
        ice,        //�X
        dirt,       //���n
        boost,      //������
        other,      //�ǂ�ł��Ȃ�
        circuitMax  //�I�_
    };

    float defaultCheckpointSize_;   //�`�F�b�N�|�C���g�̑傫��
    float defaultStartRotate_;      //�X�^�[�g���̉�]
    float checkPointLimit_;         //�`�F�b�N�|�C���g��T�����E
    std::vector<string> circuitNameArr_;

    std::vector<CircuitUnion> circuits_;    //�R�[�X���ׂĂ̂܂Ƃ�

    std::map<circuitType, std::string> circuitTypeMap_; //�R�[�X�̎�ނƑΉ����镶����

	//�R�[�X�t�@�C���ǂݍ���
	void MakeCircuit();

    //�R�[�X�̃p�[�c�̃��f����ǂݍ���
    void SetCircuitParts(CircuitUnion* pCU,const std::string& modelName,const circuitType& modelType);

    //�`�F�b�N�|�C���g��T���ăZ�b�g
    void MakeCheckPoint();

    //�X�^�[�g�n�_�ݒu
    void MakeStartPoint();
}

//������
void Circuit::Initialize()
{
    defaultCheckpointSize_ = 60.0f;
    defaultStartRotate_ = 0.0f;
    checkPointLimit_ = 500;

    circuitTypeMap_[circuitType::abyss] = "abyss";
    circuitTypeMap_[circuitType::boost] = "boost";
    circuitTypeMap_[circuitType::dirt] = "dirt";
    circuitTypeMap_[circuitType::ice] = "ice";
    circuitTypeMap_[circuitType::other] = "other";
    circuitTypeMap_[circuitType::road] = "road";
    circuitTypeMap_[circuitType::turf] = "turf";

    //�R�[�X�t�@�C���ǂݍ���
    MakeCircuit();

    //�`�F�b�N�|�C���g�ݒu
    MakeCheckPoint();

    //�X�^�[�g�n�_�ݒu
    MakeStartPoint();
}

void Circuit::MakeCircuit()
{
    const int pathSize = MAX_PATH;

    string fileName = ".\\gameObject.ini";

    string appHead = "circuit_";
    string nameKey = "name";
    string lapKey = "lap";
    string circuitModelExtension = ".fbx";
    string modelFilePath = Global::GetModelFileName();

    for (int number = 0; number < 100; number++)
    {
        string app = appHead + to_string(number);
        char section[pathSize];
        DWORD result = GetPrivateProfileSection(app.c_str(), section, pathSize, fileName.c_str());
        if (result >= (pathSize - 2) || result <= 0)
            continue; // �T�C�Y�I�[�o�[�G���[

        std::map<string, string> circuitMap;    //�T���L�[�ƒl������
        int start = 0;  //�J�n�ʒu
        for (int i = 0; i < result; i++)
        {
            string key = "";
            string name = "";
            bool equal = false;
            for (int j = start; j < result; j++)
            {
                if (section[j] == '\0')
                {
                    start = j + 1;  //�����ɗ����Ƀ��[�v�𔲂���̂͑z��O
                    break;
                }
                else if (section[j] == '=')
                {
                    if (!equal)
                        equal = true;
                }
                else
                {
                    if (equal)
                        name += section[j];
                    else
                        key += section[j];
                }
            }
            if (!(key.empty()) && !(name.empty()))
                circuitMap[key] = name; //�ǉ�
        }
        //����
        int lap = 1;
        if (circuitMap.count(lapKey) && stoi(circuitMap[lapKey]) >= 1)
            lap = stoi(circuitMap[lapKey]);

        //�X�e�[�W��
        string circuitName = "circuit";
        if (circuitMap.count(nameKey) && !(circuitMap[nameKey].empty()))
            circuitName = circuitMap[nameKey];

        //����
        CircuitUnion circuit(circuitName, lap);
        //�p�[�c�����邩�������Ēǉ�
        for (const auto& i : circuitTypeMap_)
        {
            //�R�[�X�̎�ނ������āAfbx�t�@�C�����H
            if (circuitMap.count(i.second) && circuitMap[i.second].find(circuitModelExtension) != std::string::npos)
            {
                string model = modelFilePath + circuitMap[i.second];
                SetCircuitParts(&circuit, model, i.first);

                //CircuitParts cp(modelFilePath + circuitMap[i.second], -1, i.first);
                //circuit.parts_.push_back(cp);
            }
        }
        //�ǉ�
        if (circuit.parts_.size() >= 1)
            circuits_.push_back(circuit);
    }
}

//�R�[�X�̃p�[�c�̃��f����ǂݍ���
void Circuit::SetCircuitParts(CircuitUnion* pCU,const string& modelName,const circuitType& modelType)
{
    if (pCU == nullptr)
        return;

    //�p�[�c
    int number = Model::Load(modelName);
    assert(number >= 0);

    CircuitParts cp(modelName, number, modelType);
    pCU->parts_.push_back(cp);
}

//�`�F�b�N�|�C���g��T���ăZ�b�g
void Circuit::MakeCheckPoint()
{
    //�R�[�X�S��
    for (int circuit = 0; circuit < circuits_.size(); circuit++)
    {
        int last = 0;   //�ŐV�̃`�F�b�N�|�C���g�̔ԍ�

        //������Ȃ������E�ɂȂ�܂ŏI���Ȃ�
        for (int i = 0; i < checkPointLimit_; i++)
        {
            bool isSuccess = false;//����������

            //�p�[�c
            for (int part = 0; part < circuits_[circuit].parts_.size(); part++)
            {
                //���f����
                string center = "checkpoint_";
                center += to_string(last);
                string out = center;
                center += "_center";
                out += "_out";

                XMFLOAT3 centerPos;
                //���S��T��
                if (Model::GetBonePosition(&centerPos, circuits_[circuit].parts_[part].model_, center))
                {
                    //��������
                    isSuccess = true;
                    bool outSuccess = false;
                    //�p�[�c
                    for (int part2 = 0; part2 < circuits_[circuit].parts_.size(); part2++)
                    {
                        //�O����T��
                        XMFLOAT3 outPos;
                        if (Model::GetBonePosition(&outPos, circuits_[circuit].parts_[part2].model_, out))
                        {
                            //��������
                            outSuccess = true;
                            XMVECTOR centerVec = XMLoadFloat3(&centerPos);
                            XMVECTOR outVec = XMLoadFloat3(&outPos);
                            //�V�K
                            CircuitCheckPoint ccp(nullptr, centerPos
                                , *XMVector3Length(outVec - centerVec).m128_f32);
                            ccp.CP_position_.x *= -1;  //maya�Ƃ̂���̉���
                            circuits_[circuit].check_.push_back(ccp);
                            last++;
                            break;
                        }
                    }
                    if (!outSuccess)
                    {
                        //�V�K
                        CircuitCheckPoint ccp(nullptr, centerPos, defaultCheckpointSize_);
                        ccp.CP_position_.x *= -1;  //maya�Ƃ̂���̉���
                        circuits_[circuit].check_.push_back(ccp);

                        last++;
                    }
                }
            }
            if (!isSuccess)
            {
                //����������Ȃ��Ȃ�
                break;
            }
        }
    }
}

//�X�^�[�g�n�_��T���ăZ�b�g
void Circuit::MakeStartPoint()
{
    //�R�[�X�S��
    for (int circuit = 0; circuit < circuits_.size(); circuit++)
    {
        int last = 0;   //�ŐV�̃X�^�[�g�ʒu�̔ԍ�
        defaultStartRotate_ = 0;    //���Z�b�g���Ă���
        //������Ȃ������E�ɂȂ�܂ŏI���Ȃ�
        for (int i = 0; i < checkPointLimit_; i++)
        {
            bool isSuccess = false;//����������
            //�p�[�c
            for (int part = 0; part < circuits_[circuit].parts_.size(); part++)
            {
                //���f����
                string start = "startpoint_";
                start += to_string(last);
                string dir = start + "_direction";
                XMFLOAT3 startPos;
                //���S��T��
                if (Model::GetBonePosition(&startPos, circuits_[circuit].parts_[part].model_, start))
                {
                    //��������
                    isSuccess = true;
                    bool dirSuccess = false;
                    //�p�[�c
                    for (int part2 = 0; part2 < circuits_[circuit].parts_.size(); part2++)
                    {
                        //������\���{�[����T��
                        XMFLOAT3 dirPos;
                        if (Model::GetBonePosition(&dirPos, circuits_[circuit].parts_[part2].model_, dir))
                        {
                            //��������
                            dirSuccess = true;
                            last++;
                            XMVECTOR startVec = XMVector3Normalize(XMLoadFloat3(&startPos) - XMLoadFloat3(&dirPos));
                            defaultStartRotate_ = Calculator::AngleBetweenVector(startVec, { 0.0f,0.0f,1.0f,0.0f });
                            //�X�V
                            Transform startTrans;
                            startTrans.position_ = startPos;
                            startTrans.position_.x *= -1;   //�������
                            startTrans.rotate_.y = defaultStartRotate_;
                            circuits_[circuit].startTransform_.push_back(startTrans);
                            break;
                        }
                    }
                    if (!dirSuccess)
                    {
                        //�ʒu�͌����������Ǖ����͌�����Ȃ�����
                        //�p�x�̌v�Z�͂��Ȃ�
                        last++;
                        //�X�V
                        Transform startTrans;
                        startTrans.position_ = startPos;
                        startTrans.position_.x *= -1;   //�������
                        startTrans.rotate_.y = defaultStartRotate_;
                        circuits_[circuit].startTransform_.push_back(startTrans);
                    }
                }
            }
            if (!isSuccess)
            {
                //����������Ȃ��Ȃ�
                break;
            }
        }
    }
}

std::vector<std::string>* Circuit::GetCircuitNameArray()
{
    circuitNameArr_.clear();

    for (auto& itr : circuits_)
    {
        circuitNameArr_.push_back(itr.name_);
    }

    return &circuitNameArr_;
}

void Circuit::GetCircuits(std::vector<Circuit::CircuitUnion>* circuitArray)
{
    circuitArray = &circuits_;
}

void Circuit::ResetCircuitModelHandle()
{
    for (auto& itr : circuits_)
    {
        for (auto& itr2 : itr.parts_)
        {
            //�p�[�c
            itr2.model_ = Model::Load(itr2.modelName_);
            assert(itr2.model_ >= 0);
        }
    }
}