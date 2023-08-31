#include "Engine/Model.h"

#include "Ground.h"
#include "CheckPoint.h"

using std::string;
using std::to_string;

//�R���X�g���N�^
Ground::Ground(GameObject* parent)
    :GameObject(parent, "Ground")
    , chosenCircuit_(0)
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
        defaultStartRotate_ = 0;    //���Z�b�g���Ă���

        //�����葱�������I���Ȃ�
        while (true)
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
                            /*
                            //���K�����Ȃ��Ă����͂�
                            XMVECTOR vecZ = { 0.0f,0.0f,1.0f,0.0f };
                            XMVECTOR vecUp = { 0.0f,1.0f,0.0f,0.0f };


                            //�x�N�g������p�x���v�Z
                            defaultStartRotate_ = XMConvertToDegrees(acos(*XMVector3Dot(startVec, vecZ).m128_f32
                                / (*XMVector3Length(startVec).m128_f32 * *XMVector3Length(vecZ).m128_f32)));

                            //�O�ς��g��Ȃ���0 ~ 180�@�ɂȂ��Ă��܂�
                            XMVECTOR cro = XMVector3Cross(startVec, vecZ - startVec);
                            if (*XMVector3Dot(cro, vecUp).m128_f32 > 0.0f)
                                defaultStartRotate_ += 180;
                            */

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

//
void Ground::SetCircuitParts(CircuitUnion* pCU, std::string modelName, int modelType)
{
    if (pCU == nullptr)
        return;

    //�p�[�c
    CircuitParts cp(Model::Load(modelName), modelType);
    pCU->parts_.push_back(cp);
}

void Ground::MakeCircuit()
{
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
}