#pragma once
#include "Engine/GameObject.h"

//���������Ǘ�����N���X
class Background : public GameObject
{
    int hModel_;    //���f���ԍ�
public:
    //�R���X�g���N�^
    Background(GameObject* parent);

    //�f�X�g���N�^
    ~Background();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};