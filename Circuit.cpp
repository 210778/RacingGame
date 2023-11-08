#include "Engine/Model.h"
#include "Circuit.h"
#include "CheckPoint.h"

using std::string;
using std::to_string;
using std::stoi;
using std::vector;

namespace Circuit
{
    Transform circuitTransform_;    //�R�[�X�̃g�����X�t�H�[��
    int chosenCircuit_;             //�I�����ꂽ�R�[�X
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
    //���������Ă���Ȃ��߂�
    if (!(circuits_.empty()))
    {
        return;
    }

    chosenCircuit_ = 0;
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

//�`��
void Circuit::Draw()
{
    if (circuits_.empty())
        return;

    for (int i = 0; i < circuits_[chosenCircuit_].parts_.size(); i++)
    {
        if (circuits_[chosenCircuit_].parts_[i].type_ == circuitType::abyss)
        {
            //�w�i�͉e�Ȃ�
            Model::SetTransform(circuits_[chosenCircuit_].parts_[i].model_, circuitTransform_);
            Model::Draw(circuits_[chosenCircuit_].parts_[i].model_, Direct3D::SHADER_SIMPLE3D);
        }
        else
        {
            Model::SetTransform(circuits_[chosenCircuit_].parts_[i].model_, circuitTransform_);
            Model::Draw(circuits_[chosenCircuit_].parts_[i].model_);
        }
    }
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

    for (int number = 0; number < checkPointLimit_; number++)
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

//�p�[�c�̃��f���ԍ����ăZ�b�g����
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

//�R�[�X��I��
void Circuit::SetChosenCircuit(int value)
{
    chosenCircuit_ = value;
}

//�I�񂾃R�[�X�̃`�F�b�N�|�C���g�����
void Circuit::CreateChosenCircuit(GameObject* pGO)
{
    //���S
    if (circuits_.empty())
        return;
    if (chosenCircuit_ < 0 || chosenCircuit_ >= circuits_.size())
        chosenCircuit_ = 0;

    for (int i = 0; i < circuits_[chosenCircuit_].check_.size(); i++)
    {
        circuits_[chosenCircuit_].check_[i].pCP_ = Instantiate<CheckPoint>(pGO);
        circuits_[chosenCircuit_].check_[i].pCP_->MakeSphereCollider(&circuits_[chosenCircuit_].check_[i].CP_position_
        , circuits_[chosenCircuit_].check_[i].CP_Radius_, i);
    }
}

//���̃`�F�b�N�|�C���g�̈ʒu��Ԃ�
XMFLOAT3* Circuit::GetNextCheckPointPosition(int point, int next)
{
    //���K������
    int number = (point + next - 1) % circuits_[chosenCircuit_].check_.size();
    if (number < 0)
        number += circuits_[chosenCircuit_].check_.size();

    XMFLOAT3 position = circuits_[chosenCircuit_].check_[number].CP_position_;
    return &position;
}

Circuit::CircuitUnion* Circuit::GetChosenCircuit()
{
    if (chosenCircuit_ >= 0 && chosenCircuit_ < circuits_.size())
    {
        return &circuits_[chosenCircuit_];
    }
    else
        return nullptr;
}