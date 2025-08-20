#include "skydome.hpp"

Skydome::Skydome()
    : GameObject{ "skydome" }
    , model_(std::make_shared<Modeler>(*this))
{
    tag_ = ObjectTag::Skydome;
}

Skydome::~Skydome()
{
}

void Skydome::Initialize() noexcept
{
    transform_->SetLocalScale(0.05f);

    if (model_) {
        model_->SetModelHandle("../resources/model/skydome/my_skydome.mv1");
    }
}

void Skydome::Finalize() noexcept
{
}

void Skydome::Update() noexcept
{
    model_->SetMatrix(transform_->GetLocalMatrix());
}

void Skydome::Draw() const noexcept
{
    model_->Draw();
}

void Skydome::LastUpdate() noexcept
{
}

void Skydome::SetParent(std::shared_ptr<Transform> parent) noexcept
{
    transform_->SetParent(parent.get());
}

void Skydome::RemoveParent() noexcept
{
    transform_->SetParent(nullptr);
}
