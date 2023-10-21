#pragma once
#include "Ground.h"

namespace Circuit
{
    Ground pGround_ = nullptr;

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

    //�ԍ����Z�b�g���Ĕz��̒�����R�[�X�I��
    void SetChosenCircuit(int i) { chosenCircuit_ = i; }
    //�I�������R�[�X�̔ԍ����擾
    int  GetChosenCircuit() { return chosenCircuit_; }

    //�g�p���Ă���R�[�X�̃|�C���^ ���S�΍􂳂�Ă��āA���s������nullptr
    CircuitUnion* GetCircuitUnion()
    {
        if (chosenCircuit_ >= 0 && chosenCircuit_ < circuits_.size())
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

    /// <summary>
    /// ���̃`�F�b�N�|�C���g�̈ʒu
    /// </summary>
    /// <param name="point">�����̃`�F�b�N�|�C���g</param>
    /// <param name="next">�m�肽����������X���̃|�C���g�i�f�t�H���g�F�P�j</param>
    /// <returns>���̃`�F�b�N�|�C���g�̈ʒu�̃|�C���^�@�z��̒[�ɗ�����ŏ����琔����</returns>
    XMFLOAT3* NextCheckPointPosition(int point, int next = 1);

    //
    void SetCircuitParts(CircuitUnion* pCU, std::string modelName, circuitType modelType);

    void MakeCircuit();

    //�I�񂾃R�[�X�̃`�F�b�N�|�C���g�����
    void CreateChosenCircuit(int value);

private:
    int a;
}

