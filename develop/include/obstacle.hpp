#pragma once
#include "object/collidable_object.hpp"

/// @brief ��Q��
class Obstacle : public CollidableObject
{
public:
    /// @brief �R���X�g���N�^
    Obstacle(const std::string& name);

    /// @brief �f�X�g���N�^
    ~Obstacle() override;

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
    static constexpr float kSpeed = 24.f;

private:
    unsigned int color_; /// @brief �J�v�Z���̐F: DEBUG�p

};