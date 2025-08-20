#pragma once
#include "object/game_object.hpp"
#include "Component/modeler.hpp"

class Skydome final : public GameObject
{
public:
    /// @brief �C���X�^���X��������
    Skydome();

    /// @brief �C���X�^���X�����
    ~Skydome() override;

    /// @brief ������
    void Initialize() noexcept override;

    /// @brief �j��
    void Finalize()   noexcept override;

    /// @brief �X�V
    void Update()	  noexcept override;

    /// @brief �`��
    void Draw() const noexcept override;

    /// @brief �`���X�V
    void LastUpdate() noexcept override;

    /// @brief �e��ݒ�
    void SetParent(std::shared_ptr<Transform> parent) noexcept;

    /// @brief �e���폜
    void RemoveParent() noexcept;

private:
    std::shared_ptr<Modeler> model_;

};
