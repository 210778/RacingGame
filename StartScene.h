#pragma once
#include "Engine/GameObject.h"

class Text;

//�X�^�[�g��ʃV�[�����Ǘ�����N���X
class StartScene : public GameObject
{
	int hImageArrow_;	//���

	int hImageStart_;	//�X�^�[�g���
	int hImageLoad_;	//���[�h���

	std::vector<int> CircuitImage_;	//�R�[�X�̃C���[�W�摜

	//������
	std::string stringSelect_;
	std::string stringStart_;
	std::string stringLoad_;

	bool quitFlag_;	//�I�����邩�ǂ���

	//�����|�C���^
	Text* pTextCircuit_;
	Text* pTextCaption_;

	enum DataName
	{
		circuit = 0,
		population,
		vehicle,
		wheel,
		NameMax
	};

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
		//�ŏ��l����Ȃ��Ȃ�"<"���A�ő�l����Ȃ��Ȃ�">"��ǉ�����string�̃|�C���^�ɕԂ�
		void PrintArrowLR(std::string* str)
		{
			if (str == nullptr)
				return;

			*str = (index > minValue) ? "< " + *str : "  " + *str;
			*str = (index < maxValue) ? *str + " >" : *str + "  ";
		}
	};

	DataSelection circuitSelect_;

	std::map<int,DataSelection> dataSelection_;	//�I������f�[�^�̂܂Ƃ�
	DataSelection selectIndex_;	//�f�[�^�̍���


	DataSelection sceneIndex_;	//��ʕʂ̍���

	enum SceneName
	{
		title = 0,
		select,
		load,
		SceneMax
	};


	//���ڂ̐ݒ�ƕ\��
	void PrintParagraph(DataName dn, std::string str, int width, int height);

	//�\���p
	float captionWidthOperand_;
	int captionHeight_;
	int captionUpperHeight_;
	XMFLOAT2 sceneTitlePosition_;
	float countSpeed_;
	float arrwoBace_;
	float sinOperand_;
	float indexOperand_;
	float indexUpper_;
	float indexLastUpper_;

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