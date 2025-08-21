#pragma once
#include <cstdint>
#include <functional>
#include "object/game_object.hpp"

/// @remark ����
using ON_CLICK_CALLBACK = std::function<void()>;
using ON_HOVER_CALLBACK = std::function<void()>;

class Button : public GameObject
{
public:
	/// @brief �C���X�^���X������
	Button(const std::string& name, Vector2D_I pos, Vector2D_I size);

	/// @brief �C���X�^���X���
	~Button() override = default;

    /// @brief ������
    void Initialize() noexcept override;

    /// @brief �j������
    void Finalize()   noexcept override;

    /// @brief �X�V
    void Update()	  noexcept override;

    /// @brief �`��
    void Draw() const noexcept override;

    /// @brief�`���X�V
    void LastUpdate() noexcept override;

    /// @brief �{�^�����N���b�N���ꂽ�Ƃ�
    void OnClick()    noexcept;

    /// @brief �{�^�����z�o�[����
    void OnHover()     noexcept;

    /// @brief �z�o�[��Ԃ̐ݒ�
    void SetHover(bool is_hover = true) noexcept;

    /// @brief �z�o�[��Ԃ��擾
    bool IsHover() const noexcept { return is_hover_; };

    /// @brief �N���b�N���̏�����o�^
    void RegisterOnClickProcess(const ON_CLICK_CALLBACK& process) noexcept;

    /// @brief �z�o�[���̏�����o�^
    void RegisterOnHoverProcess(const ON_HOVER_CALLBACK& process) noexcept;

private:
    Vector2D_I center_pos_;         /// @brief �������W
    Vector2D_I size_;               /// @brief �T�C�Y

    ON_CLICK_CALLBACK on_click_;    /// @brief �N���b�N�����֐�
    ON_HOVER_CALLBACK on_hover_;    /// @brief �z�o�[�����֐�
    bool              is_click_;    /// @brief �{�^���N���b�N
    bool              is_hover_;    /// @brief �{�^���Z���N�g
};
