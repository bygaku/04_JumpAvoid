#pragma once
#include "collidable_object.hpp"
#include "component/health.hpp"
#include "component/modeler.hpp"
#include "component/animator.hpp"

/// @brief キャラクターオブジェクト
class CharacterObject abstract : public CollidableObject
{
public:
    /// @remark 禁止コンストラクタ
    CharacterObject() = delete;

    /// @brief インスタンスを初期化
    CharacterObject(const std::string& name)
        : CollidableObject{ name, Weight::Normal, ColliderInfo::Type::Capsule, false }
        , health_(std::make_shared<Health>(*this, 1))
        , model_(nullptr)
        , animator_(nullptr)
    {
    }

    /// @brief インスタンスを解放
    virtual ~CharacterObject() override = default;

    /// @brief オブジェクトを初期化
    virtual void Initialize() noexcept override
    {
        CollidableObject::Initialize();
    }

    /// @brief オブジェクト破棄処理
    virtual void Finalize()   noexcept override
    {
        CollidableObject::Finalize();
    }

    /// @brief オブジェクトを更新
    virtual void Update()	  noexcept override abstract;

    /// @brief オブジェクトを描画
    virtual void Draw() const noexcept override abstract;

    /// @brief オブジェクトの最終更新
    /// HACK: 毎フレーム呼ばない設計にする
    virtual void LastUpdate() noexcept override abstract;

    /// @brief 衝突判定
    virtual void OnCollide(const CollidableObject& object) noexcept override abstract;

protected:
    /// @remark 体力操作
    void SetHealth(uint32_t amount) noexcept { health_->UpgradeMaxHealth(amount); }
    void OnDamage (uint32_t amount) noexcept { health_->TakeDamage      (amount); }
    void OnHeal	  (uint32_t amount) noexcept { health_->TakeHeal        (amount); }

protected:
    std::shared_ptr<Health>   health_;      /// @brief 体力管理コンポーネント
    std::shared_ptr<Modeler>  model_;       /// @brief モデル管理コンポーネント
    std::shared_ptr<Animator> animator_;    /// @brief アニメーション管理コンポーネント

};
