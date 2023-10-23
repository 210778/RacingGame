#pragma once
#include <map>
#include <string>
#include <vector>
#include "Engine/GameObject.h"

class CircuitCheckPoint;
class CheckPoint;
class CircuitUnion;
class GameObject;

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


    //������
    void Initialize();

    //�`��
    void Draw();

    std::vector<std::string>* GetCircuitNameArray();

    //�p�[�c�̃��f���ԍ����ăZ�b�g����
    void ResetCircuitModelHandle();

    //�R�[�X��I��
    void SetChosenCircuit(int value);

    //�I�񂾃R�[�X�̃`�F�b�N�|�C���g�����
    void CreateChosenCircuit(GameObject* pGO);

    /// <summary>
    /// ���̃`�F�b�N�|�C���g�̈ʒu
    /// </summary>
    /// <param name="point">�����̃`�F�b�N�|�C���g</param>
    /// <param name="next">�m�肽����������X���̃|�C���g�i�f�t�H���g�F�P�j</param>
    /// <returns>���̃`�F�b�N�|�C���g�̈ʒu�̃|�C���^�@�z��̒[�ɗ�����ŏ����琔����</returns>
    XMFLOAT3* GetNextCheckPointPosition(int point, int next = 1);

    //�R�[�X�z�񂩂�I�������R�[�X�̃|�C���^���擾
    CircuitUnion* GetChosenCircuit();
}

