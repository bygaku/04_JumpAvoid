#include <DxLib.h>
#include "app/game_time.hpp"
#include "component/rigidbody.hpp"

Rigidbody::Rigidbody(Transform& transform)
    : owner_transform_(transform)
    , material_(PhysicsMaterial::Default())
    , direction_(VGet(0.f, 0.f, 0.f))
    , velocity_ (VGet(0.f, 0.f, 0.f))
    , future_pos_(VGet(0.f, 0.f, 0.f))
    , mass_(1.f)
    , use_gravity_(true)
    , is_grounded_(false)
    , ground_normal_(VGet(0.f, 1.f, 0.f))
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Initialize() noexcept
{
    direction_ = VGet(0.f, 0.f, 0.f);
    velocity_  = VGet(0.f, 0.f, 0.f);
}

void Rigidbody::SetVelocity(const VECTOR& velocity) noexcept
{
    velocity_ = velocity;

    if (VSquareSize(velocity_) > 0.001f) {
        direction_ = VNorm(velocity_);
    }
}

#undef min
#undef max
void Rigidbody::SetMass(float mass) noexcept
{
    mass_ = std::max(mass, 0.01f);
}

void Rigidbody::SetUseGravity(bool gravity) noexcept
{
    use_gravity_ = gravity;
}

void Rigidbody::SetMaterial(const PhysicsMaterial& material) noexcept
{
    material_ = material;
}

void Rigidbody::SetGroundNormal(const VECTOR& normal) noexcept
{
    ground_normal_ = normal;
}

void Rigidbody::AddForce(const VECTOR& force_direction) noexcept
{
    VECTOR accel = force_direction * (1.0f / mass_);
    velocity_ += accel * GameTime::GetDeltaTime();

    if (VSquareSize(velocity_) > 0.001f) {
        direction_ = VNorm(velocity_);
    }
}

void Rigidbody::SaveFuturePosition(const VECTOR& future_pos) noexcept
{
    future_pos_ = future_pos;
}
