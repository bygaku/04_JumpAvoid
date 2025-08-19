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
    //! ローカル座標をワールド座標に変換
    capsule_.world_start_ = VAdd(position, VTransform(capsule_.local_start_, rotation));
    capsule_.world_end_   = VAdd(position, VTransform(capsule_.local_end_,   rotation));

    //! AABBを計算
    aabb_ = AABB::FromCapsule(capsule_.world_start_, capsule_.world_end_, capsule_.radius_);

    //! 更新フラグをクリア
    SetNeedsUpdate(false);
}

bool ColliderInfoCapsule::TestCollision(const ColliderInfo* other, const VECTOR& pos_a, const VECTOR& pos_b, VECTOR& hit_point, VECTOR& hit_normal, float& penetration) const
{
    // CollisionDispatcherに処理を委譲
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
