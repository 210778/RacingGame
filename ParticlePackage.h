#pragma once
#include "Engine/Particle.h"
//#include "Engine/GameObject.h"
class Particle;

namespace ParticlePackage
{
    //�������i�f�[�^�Z�b�g�j
    void Initialize();

    enum class ParticleName
    {
        boost,
        rainbow,
        smoke,
        grass,
        dirt,
        spark,
        gold,
        ice,
    };

    //�G�t�F�N�g�N���֐�
    
    /// <summary>
    /// �G�t�F�N�g�N��
    /// </summary>
    /// <param name="pParticle">�p�[�e�B�N���^�|�C���^</param>
    /// <param name="pn">�G�t�F�N�g�̖��O�^</param>
    /// <param name="position">�ʒu</param>
    /// <param name="direction">����</param>
    void ActParticle(Particle* pParticle, ParticleName pn, const XMFLOAT3& position
        , const XMFLOAT3& direction);
    /// <summary>
    /// �G�t�F�N�g�N��
    /// </summary>
    /// <param name="pParticle">�p�[�e�B�N���^�|�C���^</param>
    /// <param name="pn">�G�t�F�N�g�̖��O�^</param>
    /// <param name="position">�ʒu</param>
    /// <param name="direction">����</param>
    void ActParticle(Particle* pParticle, ParticleName pn, const XMFLOAT3& position
        , const XMVECTOR& direction);

    /// <summary>
    /// �G�t�F�N�g�N��
    /// </summary>
    /// <param name="pParticle">�p�[�e�B�N���^�|�C���^</param>
    /// <param name="pn">�G�t�F�N�g�̖��O�^</param>
    /// <param name="position">�ʒu</param>
    /// <param name="direction">����:�f�t�H���g</param>
    void ActParticle(Particle* pParticle, ParticleName pn, const XMFLOAT3& position);
};
