#include "Engine/Text.h"
#include "Engine/Global.h"
#include "Engine/Image.h"
#include "TextPrinter.h"


namespace TextValue
{
	Text* pText = nullptr;
	int standbyCount_ = 0;

	void Initialize()
	{
		pText = new Text;
		pText->Initialize();
	}
};

//�R���X�g���N�^
TextPrinter::TextPrinter(GameObject* parent)
    :GameObject(parent, "TextPrinter")
{
}

//�f�X�g���N�^
TextPrinter::~TextPrinter()
{
}

//������
void TextPrinter::Initialize()
{
	TextValue::Initialize();

	int image_go = Image::Load("image\\count_go.png");
	assert(image_go >= 0);
	imageMap_["go"] = image_go;

	int image_1 = Image::Load("image\\count_1.png");
	assert(image_1 >= 0);
	imageMap_["1"] = image_1;

	int image_2 = Image::Load("image\\count_2.png");
	assert(image_2 >= 0);
	imageMap_["2"] = image_2;

	int image_3 = Image::Load("image\\count_3.png");
	assert(image_3 >= 0);
	imageMap_["3"] = image_3;
}

//�X�V
void TextPrinter::Update()
{
}

//�`��
void TextPrinter::Draw()
{
	//TextValue::pText->Draw(300, 300, TextValue::standbyCount_.c_str());

	switch (TextValue::standbyCount_)
	{
	default:
		break;
	case 0:
		Image::Draw(imageMap_["1"]);
		break;
	case 1:
		Image::Draw(imageMap_["2"]);
		break;
	case 2:
		Image::Draw(imageMap_["3"]);
		break;
	}

}

//�J��
void TextPrinter::Release()
{
}

void TextValue::SetStandby(int value)
{
	standbyCount_ = value;
}