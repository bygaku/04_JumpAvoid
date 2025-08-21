#pragma once
#include "object/collidable_object.hpp"
#include "component/modeler.hpp"

/// @brief ���I�u�W�F�N�g�i�ÓI�j
class Floor final : public CollidableObject
{
public:
    /// @brief �C���X�^���X��������
    Floor(const VECTOR& position, const VECTOR& size);

    /// @brief �C���X�^���X�����
    ~Floor() override;

    /// @brief �I�u�W�F�N�g��������
    void Initialize()   noexcept override;

    /// @brief �I�u�W�F�N�g�j������
    void Finalize()     noexcept override;

    /// @brief �I�u�W�F�N�g���X�V
    void Update()       noexcept override;

    /// @brief �I�u�W�F�N�g��`��
    void Draw()   const noexcept override;

    /// @brief �I�u�W�F�N�g�̍ŏI�X�V
    void LastUpdate()   noexcept override;

    /// @brief �Փ˔���
    void OnCollide(const CollidableObject& object) noexcept override;

private:
    VECTOR size_;                       /// @brief ���̃T�C�Y
    int    texture_;                    /// @brief �e�N�X�`���n���h��

    std::shared_ptr<Modeler> model_;    /// @brief ���f���Ǘ��R���|�[�l���g

};