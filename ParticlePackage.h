#pragma once
#include "Engine/Particle.h"
//#include "Engine/GameObject.h"
class Particle;

namespace ParticlePackage
{
    //初期化（データセット）
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

    //エフェクト起動関数
    
    /// <summary>
    /// エフェクト起動
    /// </summary>
    /// <param name="pParticle">パーティクル型ポインタ</param>
    /// <param name="pn">エフェクトの名前型</param>
    /// <param name="position">位置</param>
    /// <param name="direction">方向</param>
    void ActParticle(Particle* pParticle, ParticleName pn, const XMFLOAT3& position
        , const XMFLOAT3& direction);
    /// <summary>
    /// エフェクト起動
    /// </summary>
    /// <param name="pParticle">パーティクル型ポインタ</param>
    /// <param name="pn">エフェクトの名前型</param>
    /// <param name="position">位置</param>
    /// <param name="direction">方向</param>
    void ActParticle(Particle* pParticle, ParticleName pn, const XMFLOAT3& position
        , const XMVECTOR& direction);

    /// <summary>
    /// エフェクト起動
    /// </summary>
    /// <param name="pParticle">パーティクル型ポインタ</param>
    /// <param name="pn">エフェクトの名前型</param>
    /// <param name="position">位置</param>
    /// <param name="direction">方向:デフォルト</param>
    void ActParticle(Particle* pParticle, ParticleName pn, const XMFLOAT3& position);
};
