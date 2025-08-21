#include "button.hpp"

Button::Button(const std::string& name, Vector2D_I pos, Vector2D_I size)
    : GameObject{ name }
    , center_pos_(pos)
    , size_(size)
    , on_click_(nullptr)
    , on_hover_(nullptr)
    , is_click_(false)
    , is_hover_(false)
{
}

void Button::Initialize() noexcept
{
}

void Button::Finalize() noexcept
{
}

void Button::Update() noexcept
{
    if (is_hover_) {
        OnHover();
    }

    if (is_click_) {
        OnClick();
    }
}

void Button::Draw() const noexcept
{
    DrawBox(center_pos_.x_ - (size_.x_ / 2),
            center_pos_.y_ - (size_.y_ / 2),
            center_pos_.x_ + (size_.x_ / 2),
            center_pos_.y_ + (size_.y_ / 2),
            0xffffff,
            true);

    if (is_hover_) {
        printfDx("%s: hover!!\n", GetName().c_str());
        printfDx("\n");
    }
}

void Button::LastUpdate() noexcept
{
}

void Button::RegisterOnClickProcess(const ON_CLICK_CALLBACK& process) noexcept
{
    on_click_ = process;
}

void Button::RegisterOnHoverProcess(const ON_HOVER_CALLBACK& process) noexcept
{
    on_hover_ = process;
}

void Button::OnClick() noexcept
{
    if (on_click_) {
        on_click_();
    }
}

void Button::OnHover() noexcept
{
    if (on_hover_) {
        on_hover_();
    }
}

void Button::SetHover(bool is_hover) noexcept
{
    is_hover_ = is_hover;
}
