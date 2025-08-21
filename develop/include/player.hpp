#pragma once
#include "object/character_object.hpp"

/// @brief �v���C���[
class Player : public CharacterObject
{
private:
    /// @brief �v���C���[���
    enum class State
    {
        kNone,
        kIdle,
        kRun,
        kPaceUp,
        kJumping,
        kFalling,
    };

public:
    /// @brief �R���X�g���N�^
    Player();

    /// @brief �f�X�g���N�^
    ~Player() override;

    /// @brief ������
    void Initialize()   noexcept override;

    /// @brief �j��
    void Finalize()     noexcept override;

    /// @brief �X�V
    void Update()       noexcept override;

    /// @brief �`��
    void Draw()   const noexcept override;

    /// @brief �ŏI�X�V
    void LastUpdate()   noexcept override;

    /// @brief �Փ˔���
    void OnCollide(const CollidableObject& object) noexcept override;

private:
    /// @brief �W�����v�ɂ��㏸����
    void OnJumping(VECTOR& velo);

    /// @brief ���~����
    void OnFalling(VECTOR& velo);

    /// @brief ���͂ɂ��X�e�[�g�̍X�V
    void UpdateStateFromInput(VECTOR& velo);

private:
    static constexpr float kJumpForce       = 24.f;     /// @brief �W�����v������ (km/h)
    static constexpr float kJumpSustained   = 0.7f;     /// @brief �W�����v�؋�� (km/frame)
    static constexpr float kMaxPressCount   = 0.5f;     /// @brief �W�����v���͍ő�l (seconds)
    static constexpr float kPlayerHeight    = 1.7f;     /// @brief �g��
    static constexpr float kPlayerWaist     = 0.3f;     /// @brief ����

private:
    State        state_;

    bool         is_jumping_;                           /// @brief �W�����v����
    bool         jump_pressed_;                         /// @brief �W�����v����
    float        count_press_time_;                     /// @brief ���͂̎��Ԃ��J�E���g

    unsigned int color_;                                /// @brief �J�v�Z���̐F: DEBUG�p
    float        gravity_ = -0.98f;
};