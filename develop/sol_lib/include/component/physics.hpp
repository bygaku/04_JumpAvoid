#pragma once
#include <vector>
#include "object/collidable_object.hpp"

/// @brief �����E�Փ˔��肷��I�u�W�F�N�g��o�^�A�����ړ��E�Փ˂�ʒm
class Physics final
{
public:
    /// @brief �C���X�^���X���擾
    static Physics& GetInstance();

    /// @brief �C���X�^���X���쐬
    static void Create();
    
    /// @brief �C���X�^���X�����
    static void Destroy();

    /// @brief �Փ˃I�u�W�F�N�g�̓o�^
    void Entry(CollidableObject* collidable);

    /// @brief �Փ˃I�u�W�F�N�g�̉���
    void Exit(CollidableObject* collidable);

    /// @brief �o�^�I�u�W�F�N�g�̏Փ˒ʒm�A�����ړ�
    void Update() noexcept;
   
private:
    static constexpr float kGravity          = -9.81f;   /// @brief �d�� ( 1.f m/s )
    static constexpr float kMaxFreefallSpeed = -100.f;   /// @brief ���R�����̏I�[�����x

private:
    /// @brief �C���X�^���X��������
    Physics();

    /// @brief �C���X�^���X�����
    ~Physics();

private:
    std::vector<CollidableObject*> collidables_;        /// @brief �o�^���ꂽ Object �̔z��

private:
    static const int kBeforeFixInfoColor    = 0x0000ff;	/// @brief �␳�O���F
    static const int kAimInfoColor          = 0x0000aa;	/// @brief �␳�O�\����F
    static const int kAfterFixInfoColor     = 0xff00ff;	/// @brief �␳����F
    static const int kMaxHitCheck           = 5000;     /// @brief �ő唻���

    // OnCollide �̒x���ʒm�̂��߂̃f�[�^
    struct OnCollideInfo final
    {
        CollidableObject* owner;
        CollidableObject* collider;

        /// @brief CollidableObject �� OnCollide ���Ăяo��
        void OnCollide() const noexcept {
            owner->OnCollide(*collider); 
        }
    };

    /// @brief ���ׂĂ� CollidableObject �𑍓�����Ŋm�F
    std::vector<OnCollideInfo> CheckCollide()                                      const noexcept;

    /// @brief CollidableObject ���������Ă��邩����
    bool IsCollide(const CollidableObject* obj_a, const CollidableObject* obj_b)   const noexcept;

    /// @brief ���t���[���̍��W��␳
    void FixFuturePosition(CollidableObject* higher, CollidableObject* lower)      const noexcept;

    /// @brief ���W��␳
    void FixPosition() noexcept;

    ///// @brief �d�͂�K�p
    //void ApplyGravity() noexcept;

    ///// @brief ���C��K�p
    //void ApplyFriction(const std::shared_ptr<Rigidbody>& rb, float delta_time) noexcept;

private:
    static Physics* instance_;

};
