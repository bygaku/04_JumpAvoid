#pragma once
#include "game_object.hpp"
#include "misc/collider/collider_data.hpp"
#include "component/rigidbody.hpp"

/// @brief �����I�u�W�F�N�g���N���X
class CollidableObject abstract : public GameObject
{
public:
    /// @brief �ʒu�␳����
    enum class Weight : int	{
        Bouncy,     // Ball
        Low,		// Trash Can 
        Normal,		// Player, Enemy
        High,		// Car
        Static		// Wall, Map
    };

public:
    /// @remark �֎~�R���X�g���N�^
    CollidableObject() = delete;

    /// @brief �C���X�^���X��������
    CollidableObject(const std::string& name, Weight weight, ColliderInfo::Type collider_type, bool is_trigger);

    /// @brief �C���X�^���X�����
    virtual ~CollidableObject() override;

    /// @brief �I�u�W�F�N�g��������
    virtual void Initialize()   noexcept override;

    /// @brief �I�u�W�F�N�g�j������
    virtual void Finalize()     noexcept override;

    /// @brief �I�u�W�F�N�g���X�V
    virtual void Update()	    noexcept abstract;

    /// @brief �I�u�W�F�N�g��`��
    virtual void Draw()   const noexcept abstract;

    /// @brief �I�u�W�F�N�g�̍ŏI�X�V
    /// HACK: ���t���[���Ă΂Ȃ��݌v�ɂ���
    virtual void LastUpdate()   noexcept abstract;

    /// @brief �Փ˔���
    virtual void OnCollide(const CollidableObject& object) noexcept abstract;

    /// @brief ���ʂ��擾
    Weight GetWeight() const noexcept { return weight_; }

    /// @brief �����蔻��𖳎��i�X���[�j����^�O�̒ǉ�
    void AddThroughTag	 (ObjectTag tag)					   noexcept;
 
    /// @brief �����蔻��𖳎��i�X���[�j����^�O�̍폜
    void RemoveThroughTag(ObjectTag tag)					   noexcept;

    /// @brief �����蔻��𖳎��i�X���[�j����Ώۂ��ǂ���
    bool IsThroughTarget(const CollidableObject* target) const noexcept;

    /// @brief Rigidbody ���擾
    std::shared_ptr<Rigidbody> 
              GetRigidbody()    const noexcept { return     rigidbody_; }

    /// @brief �����蔻��f�[�^���擾
    std::shared_ptr<ColliderInfo>
              GetColliderInfo() const noexcept { return collider_info_; }

private:
    /// @brief �����蔻��f�[�^���쐬
    std::shared_ptr<ColliderInfo> CreateColliderInfo(ColliderInfo::Type collider_type, bool is_trigger);

protected:
    std::shared_ptr<Rigidbody>    rigidbody_;	    /// @brief �����f�[�^
    Weight                        weight_;          /// @brief �����̎���
    float                         speed_;           /// @brief �X�s�[�h

private:
    std::shared_ptr<ColliderInfo> collider_info_;	/// @brief �����蔻��f�[�^
    std::vector<ObjectTag>        through_tags_;    /// @brief �����蔻��𖳎�����^�O���X�g

//    friend Physics;
//
//private: /// @remark Physics�݂̂������^��ϐ�
//    VECTOR future_pos_;         /// @brief �ړ���̃|�W�V����

};
