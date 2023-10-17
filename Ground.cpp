#include "Engine/Model.h"
#include "Ground.h"
#include "CheckPoint.h"

using std::string;
using std::to_string;
using std::stoi;

//�R���X�g���N�^
Ground::Ground(GameObject* parent)
    :GameObject(parent, "Ground")
    , chosenCircuit_(0)
    , defaultCheckpointSize_(60.0f), defaultStartRotate_(0.0f)
    , checkPointLimit_(500)
{
    circuitTypeMap_[circuitType::abyss] = "abyss";
    circuitTypeMap_[circuitType::boost] = "boost";
    circuitTypeMap_[circuitType::dirt] = "dirt";
    circuitTypeMap_[circuitType::ice] = "ice";
    circuitTypeMap_[circuitType::other] = "other";
    circuitTypeMap_[circuitType::road] = "road";
    circuitTypeMap_[circuitType::turf] = "turf";
}

//�f�X�g���N�^
Ground::~Ground()
{
}

//������
void Ground::Initialize()
{
    //
    MakeCircuit();

    //�`�F�b�N�|�C���g�ݒu
    MakeCheckPoint();

    //�X�^�[�g�n�_�ݒu
    MakeStartPoint();
}

//�X�V
void Ground::Update()
{
    transform_.position_.x += 0;
}

//�`��
void Ground::Draw()
{
    if (circuits_.empty())
        return;

    for (int i = 0; i < circuits_[chosenCircuit_].parts_.size(); i++)
    {
        Model::SetTransform(circuits_[chosenCircuit_].parts_[i].model_, transform_);
        Model::Draw(circuits_[chosenCircuit_].parts_[i].model_);
    }
}

//�J��
void Ground::Release()
{
}

//�`�F�b�N�|�C���g��T���ăZ�b�g
void Ground::MakeCheckPoint()
{
    //�R�[�X�S��
    for (int circuit = 0; circuit < circuits_.size(); circuit++)
    {
        int last = 0;   //�ŐV�̃`�F�b�N�|�C���g�̔ԍ�

        //������Ȃ������E�ɂȂ�܂ŏI���Ȃ�
        for (int i = 0; i < checkPointLimit_; i++)
        {
            bool isSuccess = false;//����������

            if (last == 9)
                transform_.position_.y = 0;

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
void Ground::MakeStartPoint()
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

//���̃`�F�b�N�|�C���g�̈ʒu��Ԃ�
XMFLOAT3* Ground::NextCheckPointPosition(int point, int next)
{
    //���K������
    int number = (point + next - 1) % circuits_[chosenCircuit_].check_.size();
    if (number < 0)
        number += circuits_[chosenCircuit_].check_.size();

    XMFLOAT3 position = circuits_[chosenCircuit_].check_[number].CP_position_;
    return &position;
}

//
void Ground::SetCircuitParts(CircuitUnion* pCU, string modelName, circuitType modelType)
{
    if (pCU == nullptr)
        return;

    //�p�[�c
    int number = Model::Load(modelName);
    assert(number >= 0);

    CircuitParts cp(number, modelType);
    pCU->parts_.push_back(cp);
}

void Ground::MakeCircuit()
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
            }
        }
        //�ǉ�
        if (circuit.parts_.size() >= 1)
            circuits_.push_back(circuit);
    }

#if 0
    {
        //�R�[�X
        CircuitUnion circuit("circuit_1", 2);
        //�p�[�c
        SetCircuitParts(&circuit, "model\\circuit_1_R.fbx", road);
        SetCircuitParts(&circuit, "model\\circuit_1_G.fbx", turf);
        SetCircuitParts(&circuit, "model\\circuit_1_A.fbx", abyss);
        //�ǉ�
        circuits_.push_back(circuit);
    }
    {
        CircuitUnion circuit("circuit_2", 2);
        SetCircuitParts(&circuit, "model\\circuit_2_R.fbx", road);
        SetCircuitParts(&circuit, "model\\circuit_2_D.fbx", dirt);
        SetCircuitParts(&circuit, "model\\circuit_2_I.fbx", ice);
        SetCircuitParts(&circuit, "model\\circuit_1_A.fbx", abyss);
        circuits_.push_back(circuit);
    }

    {
        const int pathSize = MAX_PATH;
        char caption[pathSize];

        string defaultName = "***";
        int defaultValue = -1;
        string fileName = ".\\gameObject.ini";

        int i = 1;
        string appHead = "circuit_";
        string app = appHead + to_string(i);
        string nameKey = "name";
        string lapKey = "lap";


        //���O
        char circuitName[pathSize];
        GetPrivateProfileString(app.c_str(), nameKey.c_str(), defaultName.c_str(), circuitName, pathSize, fileName.c_str());
        string circuitStr = circuitName;
        //����
        int lap = GetPrivateProfileInt(app.c_str(), lapKey.c_str(), defaultValue, fileName.c_str());

        if (lap < 1)
        {
            return;//��~
        }

        char section[pathSize];
        DWORD result = GetPrivateProfileSection(app.c_str(), section, pathSize, fileName.c_str());

        if (result >= (pathSize - 2) || result <= 0)
        {
            return; // �G���[�ŏI�����܂��B
        }

        std::map<string, string> circuitMap;
        int start = 0;
        for (int i = 0; i < result; i++)
        {
            string key = "";
            string name = "";
            bool equal = false;

            for (int j = start; j < result; j++)
            {
                if (section[j] == '\0')
                {
                    start = j + 1;
                    break;
                }
                else if (section[j] == '=')
                {
                    if (!equal)
                    {
                        equal = true;
                    }
                }
                else
                {
                    if (equal)
                    {
                        name += section[j];
                    }
                    else
                    {
                        key += section[j];
                    }
                }
            }

            circuitMap[key] = name;
        }

        //OutputDebugString(str.c_str());

        CircuitUnion circuit(circuitStr, lap);
    }
#endif

}

//�I�񂾃R�[�X�̃`�F�b�N�|�C���g�����
void Ground::CreateChosenCircuit(int value)
{
    chosenCircuit_ = value;
    
    //���S
    if (circuits_.empty())
        return;
    if (chosenCircuit_ < 0 || chosenCircuit_ >= circuits_.size())
        chosenCircuit_ = 0;

    for (int i = 0; i < circuits_[chosenCircuit_].check_.size(); i++)
    {
        if (circuits_[chosenCircuit_].check_[i].pCP_ == nullptr)
        {
            circuits_[chosenCircuit_].check_[i].pCP_ = Instantiate<CheckPoint>(this);
            circuits_[chosenCircuit_].check_[i].pCP_->MakeSphereCollider(&circuits_[chosenCircuit_].check_[i].CP_position_
                , circuits_[chosenCircuit_].check_[i].CP_Radius_, i);
        }
    }
}