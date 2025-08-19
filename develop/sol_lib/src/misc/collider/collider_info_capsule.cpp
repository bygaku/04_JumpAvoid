#include "misc/collider/collider_info_capsule.hpp"
#include "misc/collider/collision_dispatcher.hpp"

ColliderInfoCapsule::ColliderInfoCapsule(bool is_trigger, uint32_t category, uint32_t mask)
    : ColliderInfo(Type::Capsule, is_trigger, category, mask)
{
}

ColliderInfoCapsule::ColliderInfoCapsule(const VECTOR& local_start, const VECTOR& local_end,
    float radius, bool is_trigger,
    uint32_t category, uint32_t mask)
    : ColliderInfo(Type::Capsule, is_trigger, category, mask)
{
}

void ColliderInfoCapsule::UpdateAABB(const VECTOR& position, const MATRIX& rotation)
{
    //! ���[�J�����W�����[���h���W�ɕϊ�
    capsule_.world_start_ = VAdd(position, VTransform(capsule_.local_start_, rotation));
    capsule_.world_end_   = VAdd(position, VTransform(capsule_.local_end_,   rotation));

    //! AABB���v�Z
    aabb_ = AABB::FromCapsule(capsule_.world_start_, capsule_.world_end_, capsule_.radius_);

    //! �X�V�t���O���N���A
    SetNeedsUpdate(false);
}

bool ColliderInfoCapsule::TestCollision(const ColliderInfo* other, const VECTOR& pos_a, const VECTOR& pos_b, VECTOR& hit_point, VECTOR& hit_normal, float& penetration) const
{
    // CollisionDispatcher�ɏ������Ϗ�
    return CollisionDispatcher::GetInstance().TestCollision(
        this, other, pos_a, pos_b, hit_point, hit_normal, penetration
    );
}

void ColliderInfoCapsule::SetCapsule(const VECTOR& local_start, const VECTOR& local_end, float radius)
{
    capsule_.local_start_   = local_start;
    capsule_.local_end_     = local_end;
    capsule_.radius_        = radius;
    SetNeedsUpdate(true);
}

float ColliderInfoCapsule::GetHeight() const noexcept
{
    return VSize(capsule_.local_end_ - capsule_.local_start_);
}

float ColliderInfoCapsule::GetTotalHeight() const noexcept
{
    return GetHeight() + capsule_.radius_ * 2.0f;
}
