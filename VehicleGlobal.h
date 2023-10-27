#pragma once
#include "Engine/GameObject.h"

namespace VehicleGlobal
{
	enum PartName
	{
		vehicle = 0,
		wheel,
	};

	//������
	void Initialize();

	//�t�@�C������ԗ��ƃ^�C���̖��O�ǂݍ���
	void LoadVehicleObject();


	//�Z�b�^�[�ƃQ�b�^�[
	void SetChosenPopulation(int value);
	int GetChosenPopulation();
	void SetChosenVehicleName(std::string str);
	std::string GetChosenVehicleName();
	void SetChosenWheelName(std::string str);
	std::string GetChosenWheelName();

	std::map<PartName, std::map<std::string, std::string>>* GetVehicleObjectVector();

	/// <summary>
	/// �p�[�c�̖��O�ƃ��f�������i�[�����x�N�^�[�ւ̃|�C���^��Ԃ�
	/// </summary>
	/// <param name="pn">�ԗ���vehicle , �^�C����wheel</param>
	/// <returns>vector[pair[�ԗ��p�[�c�̖��O�A�t�@�C���̖��O]] </returns>
	std::vector<std::pair<std::string, std::string>>* GetVehicleNameVector(PartName pn);
};

