#include <DxLib.h>
#include "app/game_time.hpp"
#include "app/input_manager.hpp"
#include "obstacle.hpp"
#include "misc/collider/collider_info_sphere.hpp"

Obstacle::Obstacle(const std::string& name)
    : CollidableObject{name, Weight::Static, ColliderInfo::Type::Sphere, false }
    , move_speed_(kInitalizeSpeed)
{
    //! �^�O��ݒ�
    tag_ = ObjectTag::Enemy;
    model_ = std::make_shared<Modeler>(*this);
}

Obstacle::~Obstacle()
{
}

void Obstacle::Initialize() noexcept
{
    //! Transform
    transform_->SetLocalPosition(VGet(0.f, 0.f, 0.f));
    transform_->SetLocalScale(0.01f);

    //! Rigidbody
    rigidbody_->SetUseGravity(false);
    rigidbody_->SetGrounded(false);

    //! Collider Sphere
    auto sphere_collider = std::dynamic_pointer_cast<ColliderInfoSphere>(GetColliderInfo());
    if (sphere_collider) {
        sphere_collider->SetSphere(
            VGet(0.f, 0.f, 0.f),
            1.0f
        );

        sphere_collider->SetNeedsUpdate(true);
    }

    if (model_) {
        model_->Initialize();
        model_->SetModelHandle("../resources/model/obstacle/spikyball.mv1");
    }

    //! Physics�̒ǉ���
    CollidableObject::Initialize();
}

void Obstacle::Finalize() noexcept
{
    CollidableObject::Finalize();
}

void Obstacle::Update() noexcept
{
    if (!IsActive()) return;

    //VECTOR current_pos = transform_->GetLocalPosition();
    //current_pos.z += move_speed_ * GameTime::GetDeltaTime();
    //transform_->SetLocalPosition(current_pos);

    //! �R���C�_�[�X�V
    GetColliderInfo()->UpdateAABB(
        transform_->GetLocalPosition(),
        transform_->GetWorldMatrix()
    );

    //! �e�R���|�[�l���g�X�V
    transform_->UpdateWorldTransform();
    model_->SetMatrix(transform_->GetWorldMatrix());
}

void Obstacle::Draw() const noexcept
{
    if (!IsActive()) return;
    model_->Draw();

#ifdef _DEBUG
    // �f�o�b�O���͏Փ˔���̋���`��
    auto sphere  = std::dynamic_pointer_cast<ColliderInfoSphere>(GetColliderInfo());
    float radius = sphere->GetRadius();

    auto& p = transform_->GetWorldPosition();
    printfDx("world... x: %.2f, y: %.2f, z: %.2f\n", p.x, p.y, p.z);
    printfDx("sphere... x: %.2f, y: %.2f, z: %.2f\n", sphere->GetWorldCenter().x, sphere->GetWorldCenter().y, sphere->GetWorldCenter().z);

    // ���C���[�t���[���̋���`��
    DrawSphere3D(sphere->GetWorldCenter(), radius, 8,
        GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif
}

void Obstacle::LastUpdate() noexcept
{
    if (IsOutOfScreen()) {
        GameObject::Deactivate();
    }
}

void Obstacle::OnCollide(const CollidableObject& object) noexcept
{
}

void Obstacle::ResetStartPosition()
{
}

bool Obstacle::IsOutOfScreen() const noexcept
{
    return transform_->GetLocalPosition().z > kDespawnZ;
}
