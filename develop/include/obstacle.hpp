#pragma once
#include "object/collidable_object.hpp"
#include "component/modeler.hpp"

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

    /// @brief ���W������
    void ResetStartPosition();

    /// @brief ��ʊO�ɏo��������
    /// @return ��ʊO�Ȃ� true
    bool IsOutOfScreen() const noexcept;

    /// @brief �ړ����x��ݒ�
    void SetMoveSpeed(float speed) noexcept { move_speed_ = speed; }
private:
    static constexpr float kInitalizeSpeed = 5.f;
    static constexpr float kDespawnZ       = 5.0f;
    static constexpr float kSpawnZ         = -20.0f;

private:
    unsigned int             color_; /// @brief �J�v�Z���̐F: DEBUG�p
    std::shared_ptr<Modeler> model_;           ///< ���f���Ǘ�

    float move_speed_;               ///< �ړ����x�iz�����j
};