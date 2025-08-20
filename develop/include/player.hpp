#pragma once
#include "object/character_object.hpp"

/// @brief �v���C���[
class Player : public CharacterObject
{
private:
    /// @brief �A�j���[�V�������
    enum class Animation
    {
        kIdle,
        kRun,
        kPaceUp,
        kJumpUp,
        kJumpDown,
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
    /// @brief �W�����v����
    void HandleJump();

    /// @brief ���͏���
    void UpdateInput();

private:
    static constexpr float kJumpInitialPower = 2.f;    /// @brief �W�����v����
    static constexpr float kPlayerHeight     = 1.7f;    /// @brief �g��
    static constexpr float kPlayerWaist      = 0.3f;    /// @brief ����

private:
    bool         is_jumping_;                           /// @brief �W�����v����
    bool         jump_input_;                           /// @brief �W�����v����
    unsigned int color_;                                /// @brief �J�v�Z���̐F: DEBUG�p

};