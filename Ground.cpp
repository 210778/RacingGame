#include "Engine/Model.h"

#include "Ground.h"
#include "CheckPoint.h"

using std::string;
using std::to_string;

//�R���X�g���N�^
Ground::Ground(GameObject* parent)
    :GameObject(parent, "Ground"), hModel_(-1), chosenCircuit_(0)
    , defaultCheckpointSize_(60.0f), defaultStartRotate_(0.0f)
{
}

//�f�X�g���N�^
Ground::~Ground()
{
}

//������
void Ground::Initialize()
{
    hModel_ = Model::Load("model\\Ground13-G.fbx");
    assert(hModel_ >= 0);

    //�R�[�X�P
    CircuitUnion circuit_1("circuit_1",1);
    //�p�[�c
    CircuitParts circuit_1_R(Model::Load("model\\Ground15-R.fbx"), road);
    CircuitParts circuit_1_G(Model::Load("model\\Ground15-G.fbx"), turf);
    CircuitParts circuit_1_A(Model::Load("model\\bottomWater.fbx"), abyss);
    //�܂Ƃ�
    circuit_1.parts_.push_back(circuit_1_R);
    circuit_1.parts_.push_back(circuit_1_G);
    circuit_1.parts_.push_back(circuit_1_A);
    //�ǉ�
    circuits_.push_back(circuit_1);

    //�`�F�b�N�|�C���g�ݒu
    MakeCheckPoint();

    //�X�^�[�g�n�_�ݒu
    MakeStartPoint();
}

//�X�V
void Ground::Update()
{
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

void Ground::SetChosenCircuitCheckPoint(int value)
{
#if 0
    SetChosenCircuit(value);
    //���[�v
    for (int i = 0; i < circuits_[chosenCircuit_].checkPointPosition_.size(); i++)
    {
        CheckPoint* pCheckPoint = Instantiate<CheckPoint>(this);
        pCheckPoint->SetPosition(circuits_[chosenCircuit_].checkPointPosition_[i]);
        pCheckPoint->SetNumber(i);
        //checkPoint_.push_back(pCheckPoint);
    }
#endif
}

//�`�F�b�N�|�C���g��T���ăZ�b�g
void Ground::MakeCheckPoint()
{
    //�R�[�X�S��
    for (int circuit = 0; circuit < circuits_.size(); circuit++)
    {
        int last = 0;   //�ŐV�̃`�F�b�N�|�C���g�̔ԍ�

        //�����葱�������I���Ȃ�
        while (true)
        {
            bool isSuccess = false;//����������

            if (last == 9)
                transform_.position_.y = 0;

            //�p�[�c
            for (int part = 0; part < circuits_[circuit].parts_.size(); part++)
            {
                //���f����
                string center = "checkpoint";
                if (last < 10) center += "0";
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

                            //�`�F�b�N�|�C���g���
                            CheckPoint *pCheckPoint = Instantiate<CheckPoint>(this);
                            pCheckPoint->MakeSphereCollider(&centerPos
                                , *XMVector3Length(outVec - centerVec).m128_f32
                                , last);
                            //�v�b�V���o�b�N
                            circuits_[circuit].checkPoint_.push_back(pCheckPoint);
                            last++;
                            break;
                        }
                    }

                    if (!outSuccess)
                    {
                        //�����͌����������ǊO���͌�����Ȃ�����
                        //�`�F�b�N�|�C���g���
                        CheckPoint* pCheckPoint = Instantiate<CheckPoint>(this);
                        pCheckPoint->MakeSphereCollider(&centerPos
                            , defaultCheckpointSize_ , last);
                        //�v�b�V���o�b�N
                        circuits_[circuit].checkPoint_.push_back(pCheckPoint);
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

        //�����葱�������I���Ȃ�
        while (true)
        {
            bool isSuccess = false;//����������
            defaultStartRotate_ = 0;    //���Z�b�g���Ă���

            //�p�[�c
            for (int part = 0; part < circuits_[circuit].parts_.size(); part++)
            {
                //���f����
                string start = "startposition_";
                if (last < 10) start += "0";
                start += to_string(last);
                string dir = start += "direction";

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

                            XMVECTOR startVec = XMLoadFloat3(&startPos);
                            XMVECTOR dirVec = XMLoadFloat3(&dirPos);

                            //�����₱�����H
                            startVec = XMVector3Normalize(dirVec - startVec);
                            dirVec = { 0.0f,0.0f,1.0f,0.0f };

                            //�x�N�g������p�x���v�Z
                            defaultStartRotate_ = cos(*XMVector3Dot(startVec, dirVec).m128_f32
                                / (*XMVector3Length(startVec).m128_f32 * *XMVector3Length(dirVec).m128_f32));

                            //�X�V
                            Transform startTrans;
                            startTrans.position_ = startPos;
                            startTrans.rotate_.y = defaultStartRotate_;
                            circuits_[circuit].startTransform_ = startTrans;
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
                        startTrans.rotate_.y = defaultStartRotate_;
                        circuits_[circuit].startTransform_ = startTrans;
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