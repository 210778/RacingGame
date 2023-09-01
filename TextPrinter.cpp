#include "Engine/Text.h"
#include "Engine/Global.h"
#include "Engine/Image.h"
#include "TextPrinter.h"


namespace TextValue
{
	Text* pText = nullptr;
	std::string standbyCount_ = "";

	void Initialize()
	{
		pText = new Text;
		pText->Initialize();
	}
};

//コンストラクタ
TextPrinter::TextPrinter(GameObject* parent)
    :GameObject(parent, "TextPrinter")
{
}

//デストラクタ
TextPrinter::~TextPrinter()
{
}

//初期化
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

//更新
void TextPrinter::Update()
{
}

//描画
void TextPrinter::Draw()
{
	TextValue::pText->Draw(300, 300, TextValue::standbyCount_.c_str());

	for (auto& itr : imageMap_)
	{
		if (rand() % 10 == 0)
		{
			Image::Draw(itr.second);
		}
	}

}

//開放
void TextPrinter::Release()
{
}

void TextValue::SetStandby(int value)
{
	standbyCount_ = std::to_string(value);
}