#pragma once
#include <map>
#include "Engine/GameObject.h"

class Text;

//�X�^�[�g��ʃV�[�����Ǘ�����N���X
class StartScene : public GameObject
{
	int hImageArrow_;

	//����
	Text* pTextCircuit_;

	struct DataSelection
	{
		std::string title;
		int index;
		int minValue;
		int maxValue;
		DataSelection(){
			title = "";
			index = 0;
			minValue = 0;
			maxValue = 0;
		}
		DataSelection(std::string name,int ind,int minV,int maxV){
			title = name;
			index = ind;
			minValue = minV;
			maxValue = maxV;
		}
		void SetDataSelection(std::string name, int ind, int minV, int maxV) {
			title = name;
			index = ind;
			minValue = minV;
			maxValue = maxV;
		}
		//index ���ŏ��l�����Ȃ�ŏ��l�ɁA�ő�l�I�[�o�[�Ȃ�ő�l�ɂ���
		void DataClamp(){
			index = min(max(index, minValue), maxValue);
		}
		//index��value�����Z����B�ő�l�A�ŏ��l�͈͂Ɏ��܂��Ă�����true�B��������Ȃ��Ȃ�false
		bool DataAddition(int value){
			index += value;
			int check = index;
			DataClamp();

			if (check == index)
				return true;
			else
				return false;
		};
	};

	DataSelection circuitSelect_;

	std::map<int,DataSelection> dataSelection_;	//�I������f�[�^�̂܂Ƃ�
	DataSelection selectIndex_;	//�f�[�^�̍���

	enum DataName
	{
		circuit = 0,
		population,
		vehicle,
		wheel,
		NameMax
	};

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	StartScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};