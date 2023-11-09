#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Text.h"

#include "StartScene.h"
#include "Music.h"
#include "Circuit.h"
#include "VehicleGlobal.h"
#include "VehicleInput.h"

using std::string;
using std::to_string;
using std::stoi;

//�R���X�g���N�^
StartScene::StartScene(GameObject* parent)
	: GameObject(parent, "StartScene")
	, pTextCircuit_(nullptr), pTextCaption_(nullptr)
	, hImageArrow_(-1), hImageStart_(-1), hImageLoad_(-1)
	, stringSelect_(""), stringStart_(""), stringLoad_("")
	, quitFlag_(false)
	, captionWidthOperand_(0.142f), captionHeight_(150), captionUpperHeight_(50)
	, sceneTitlePosition_({ 30.0f,30.0f })
	, countSpeed_(0.1f), arrwoBace_(-0.85f), sinOperand_(0.02f)
	, indexOperand_(-0.137f), indexUpper_(0.58f), indexLastUpper_(1.0f)
{
}

//������
void StartScene::Initialize()
{
	//�R�[�X������
	Circuit::Initialize();
	//�ԗ��p�[�c�ǂݍ��ݏ�����
	VehicleGlobal::Initialize();

	//����
	pTextCircuit_ = new Text;
	pTextCircuit_->Initialize();
	pTextCaption_ = new Text;
	pTextCaption_->Initialize();

	//���
	hImageArrow_ = Image::Load("image\\arrow.png");
	assert(hImageArrow_ >= 0);
	//�^�C�g��
	hImageStart_ = Image::Load("image\\Racing game title.jpg");
	assert(hImageStart_ >= 0);
	//���[�h
	hImageLoad_ = Image::Load("image\\BackGround_K.jpg");
	assert(hImageLoad_ >= 0);
	//�V�[����
	sceneIndex_.SetDataSelection("scene", 0, 0, SceneName::SceneMax - 1);

	//�R�[�X�摜
	CircuitImage_.push_back(Image::Load("image\\count_1.png"));
	CircuitImage_.push_back(Image::Load("image\\count_2.png"));
	CircuitImage_.push_back(Image::Load("image\\count_3.png"));

	stringSelect_	= "[Select menu]";
	stringStart_	= "[Start]";
	stringLoad_		= "Now loading...";


	//����
	//�R�[�X
	dataSelection_[DataName::circuit].SetDataSelection("Circuit"
		, 0, 0, Circuit::GetCircuitNameArray()->size() - 1);

	//�Q���l��
	dataSelection_[DataName::population].SetDataSelection("Population"
		, Circuit::GetChosenCircuit()->startTransform_.size(), 1, Circuit::GetChosenCircuit()->startTransform_.size());

	//�ԗ�
	dataSelection_[DataName::vehicle].SetDataSelection("Vehicle"
		, 0, 0, VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::vehicle)->size() - 1);

	//�^�C��
	dataSelection_[DataName::wheel].SetDataSelection("Wheel"
		, 0, 0, VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::wheel)->size() - 1);

	//����
	selectIndex_.SetDataSelection("index", 0, 0, dataSelection_.size() - 1);

	//���荀��
	selectIndex_.maxValue += 1;
}

//�X�V
void StartScene::Update()
{
	//����
	VehicleInput::Update();

	//���y
	Music::Update();

	//�I��
	if (VehicleInput::GetInput(VehicleInput::Button::selectDecide))
	{
		//�I����ʂ��H
		if (sceneIndex_.index == SceneName::select)
		{
			//�V�[���J��
			if(selectIndex_.index >= selectIndex_.maxValue)
			{
				if (sceneIndex_.DataAddition(1))
					Music::Play(Music::MusicName::se_select);

				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->ChangeScene(SCENE_ID_PLAY);
			}
		}
		else
		{
			if (sceneIndex_.DataAddition(1))
				Music::Play(Music::MusicName::se_select);
		}
	}
	//�߂�
	if (VehicleInput::GetInput(VehicleInput::Button::selectReturn))
	{
		//�^�C�g�����H
		if (sceneIndex_.index == SceneName::title)
		{
			if (quitFlag_)
			{
				quitFlag_ = false;	//���b�Z�[�W�\��
				//Global::SetIsCloseWindow(true);	//�I��
			}
		}

		if (sceneIndex_.DataAddition(-1))
			Music::Play(Music::MusicName::se_select);
	}

	//�I����ʂȂ�
	if (sceneIndex_.index == SceneName::select)
	{
		if (VehicleInput::GetInput(VehicleInput::Button::selectLeft))
		{
			if (dataSelection_[selectIndex_.index].DataAddition(-1))
			{
				Music::Play(Music::MusicName::se_select2);
			}
			else
			{
				Music::Play(Music::MusicName::se_se_selectError);
			}
		}
		if (VehicleInput::GetInput(VehicleInput::Button::selectRight))
		{
			if (dataSelection_[selectIndex_.index].DataAddition(1))
				Music::Play(Music::MusicName::se_select2);
			else
				Music::Play(Music::MusicName::se_se_selectError);
		}

		if (VehicleInput::GetInput(VehicleInput::Button::selectUp))
		{
			if (selectIndex_.DataAddition(-1))
				Music::Play(Music::MusicName::se_select);
			else
				Music::Play(Music::MusicName::se_se_selectError);
		}
		if (VehicleInput::GetInput(VehicleInput::Button::selectDown))
		{
			if (selectIndex_.DataAddition(1))
				Music::Play(Music::MusicName::se_select);
			else
				Music::Play(Music::MusicName::se_se_selectError);
		}
	}

}

//�`��
void StartScene::Draw()
{
	sceneIndex_.DataClamp();
	if (sceneIndex_.index == SceneName::title)
	{
		Image::SetTransform(hImageArrow_, transform_);
		Image::Draw(hImageStart_);
	}
	else if (sceneIndex_.index == SceneName::select)
	{
		//�w�i�R�[�X�摜
		if (dataSelection_[DataName::circuit].index >= 0 && dataSelection_[DataName::circuit].index < CircuitImage_.size())
		{
			Image::SetTransform(CircuitImage_[dataSelection_[DataName::circuit].index], transform_);
			Image::Draw(CircuitImage_[dataSelection_[DataName::circuit].index]);
		}

		pTextCircuit_->Draw(sceneTitlePosition_.x, sceneTitlePosition_.y, stringSelect_.c_str());

		int width = Global::GetScreenWidth() * captionWidthOperand_;
		int count = 0;

		//�R�[�X
		PrintParagraph(DataName::circuit, Circuit::GetCircuitNameArray()->at(dataSelection_[DataName::circuit].index)
			, width, captionHeight_ + captionUpperHeight_ * count++);
		Circuit::SetChosenCircuit(dataSelection_[DataName::circuit].index);	//�I��

		//�l��
		dataSelection_[DataName::population].maxValue = Circuit::GetChosenCircuit()->startTransform_.size();
		PrintParagraph(DataName::population, to_string(dataSelection_[DataName::population].index), width, captionHeight_ + captionUpperHeight_ * count++);
		VehicleGlobal::SetChosenPopulation(dataSelection_[DataName::population].index);	//�I��

		//�ԗ�
		PrintParagraph(DataName::vehicle, VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::vehicle)->
			at(dataSelection_[DataName::vehicle].index).first, width, captionHeight_ + captionUpperHeight_ * count++);
		VehicleGlobal::SetChosenVehicleName(VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::vehicle)->
			at(dataSelection_[DataName::vehicle].index).second);	//�I��

		//�^�C��
		PrintParagraph(DataName::wheel, VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::wheel)->
			at(dataSelection_[DataName::wheel].index).first, width, captionHeight_ + captionUpperHeight_ * count++);
		VehicleGlobal::SetChosenWheelName(VehicleGlobal::GetVehicleNameVector(VehicleGlobal::PartName::wheel)->
			at(dataSelection_[DataName::wheel].index).second);	//�I��

		//����
		//�����������ꂽ�ʒu
		pTextCircuit_->Draw(width, captionHeight_ + captionUpperHeight_ * ++count, stringStart_.c_str());

		{
			//�b���J�E���g
			static float count = 0.0f;
			count += countSpeed_;

			float bace = arrwoBace_;
			float pos = (sin(count) * sinOperand_) + bace;

			Transform arrowTrans;

			//�I���̍������Ō�Ȃ痣�ꂽ�ʒu�ɂ���
			float height = (selectIndex_.index * indexOperand_);
			if (selectIndex_.index >= selectIndex_.maxValue)
				height = ((selectIndex_.index + indexLastUpper_) * indexOperand_);

			arrowTrans.position_ = { pos , height + indexUpper_, 0.0f };

			Image::SetTransform(hImageArrow_, arrowTrans);
			Image::Draw(hImageArrow_);
		}

	}
	else if (sceneIndex_.index == SceneName::load)
	{
		Image::SetTransform(hImageArrow_, transform_);
		Image::Draw(hImageLoad_);

		pTextCircuit_->Draw(sceneTitlePosition_.x, sceneTitlePosition_.y, stringLoad_.c_str());
	}

}

//�J��
void StartScene::Release()
{
	pTextCircuit_->Release();
	pTextCaption_->Release();
}

//���ڂ̐ݒ�ƕ\��
void StartScene::PrintParagraph(DataName dn, std::string str, int width, int height)
{
	dataSelection_[dn].DataClamp();
	string nameStr = str;

	//���\��
	dataSelection_[dn].PrintArrowLR(&nameStr);

	nameStr = dataSelection_[dn].title + " : " + nameStr;

	pTextCircuit_->Draw(width, height, nameStr.c_str());
}