#pragma once
#include "collidable_object.hpp"
#include "component/health.hpp"
#include "component/modeler.hpp"
#include "component/animator.hpp"

/// @brief �L�����N�^�[�I�u�W�F�N�g
class CharacterObject abstract : public CollidableObject
{
public:
    /// @remark �֎~�R���X�g���N�^
    CharacterObject() = delete;

    /// @brief �C���X�^���X��������
    CharacterObject(const std::string& name)
        : CollidableObject{ name, Weight::Normal, ColliderInfo::Type::Capsule, false }
        , health_(std::make_shared<Health>(*this, 1))
        , model_(nullptr)
        , animator_(nullptr)
    {
    }

    /// @brief �C���X�^���X�����
    virtual ~CharacterObject() override = default;

    /// @brief �I�u�W�F�N�g��������
    virtual void Initialize() noexcept override
    {
        CollidableObject::Initialize();
    }

    /// @brief �I�u�W�F�N�g�j������
    virtual void Finalize()   noexcept override
    {
        CollidableObject::Finalize();
    }

    /// @brief �I�u�W�F�N�g���X�V
    virtual void Update()	  noexcept override abstract;

    /// @brief �I�u�W�F�N�g��`��
    virtual void Draw() const noexcept override abstract;

    /// @brief �I�u�W�F�N�g�̍ŏI�X�V
    /// HACK: ���t���[���Ă΂Ȃ��݌v�ɂ���
    virtual void LastUpdate() noexcept override abstract;

    /// @brief �Փ˔���
    virtual void OnCollide(const CollidableObject& object) noexcept override abstract;

protected:
    /// @remark �̗͑���
    void SetHealth(uint32_t amount) noexcept { health_->UpgradeMaxHealth(amount); }
    void OnDamage (uint32_t amount) noexcept { health_->TakeDamage      (amount); }
    void OnHeal	  (uint32_t amount) noexcept { health_->TakeHeal        (amount); }

protected:
    std::shared_ptr<Health>   health_;      /// @brief �̗͊Ǘ��R���|�[�l���g
    std::shared_ptr<Modeler>  model_;       /// @brief ���f���Ǘ��R���|�[�l���g
    std::shared_ptr<Animator> animator_;    /// @brief �A�j���[�V�����Ǘ��R���|�[�l���g

};
