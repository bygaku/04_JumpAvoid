#include <DxLib.h>
#include "misc/assert_dx.hpp"
#include "component/physics.hpp"
#include "object/collidable_object.hpp"
#include "misc/collider/collider_info_all.hpp"

CollidableObject::CollidableObject(const std::string& name, Weight weight, ColliderInfo::Type collider_type, bool is_trigger)
    : GameObject{ name }
    , weight_(weight)
    , collider_info_(nullptr)
{
    rigidbody_ = std::make_shared<Rigidbody>(*transform_);
    CreateColliderInfo(collider_type, is_trigger);
}

CollidableObject::~CollidableObject()
{    
}

void CollidableObject::Initialize() noexcept
{
    Physics::GetInstance().Entry(this);
}

void CollidableObject::Finalize() noexcept
{
    Physics::GetInstance().Exit(this);
}

void CollidableObject::AddThroughTag(ObjectTag tag) noexcept
{
    bool found = std::find(through_tags_.begin(), through_tags_.end(), tag) != through_tags_.end();

    if (!found) {
        through_tags_.emplace_back(tag);
        return;
    }

    ASSERT(!found, "tag is already added list");
}

void CollidableObject::RemoveThroughTag(ObjectTag tag) noexcept
{
    auto it = std::find(through_tags_.begin(), through_tags_.end(), tag);

    if (it != through_tags_.end()) {
        through_tags_.erase(it);
        return;
    }

    ASSERT(it != through_tags_.end(), "tag isn't found list");
}

bool CollidableObject::IsThroughTarget(const CollidableObject* target) const noexcept
{
    return std::find(through_tags_.begin(), through_tags_.end(), target->GetTag()) != through_tags_.end();
}

std::shared_ptr<ColliderInfo> CollidableObject::CreateColliderInfo(ColliderInfo::Type collider_type, bool is_trigger)
{
    if (collider_info_ != nullptr) {
        return collider_info_;
    }

    //! ÉJÉeÉSÉäÇèdÇ≥Ç…äÓÇ√Ç¢Çƒê›íË
    uint32_t category = ColliderInfo::Category::kDynamic;

    if (weight_ == Weight::Static) {
        category = ColliderInfo::Category::kStatic;
    }
    if (is_trigger) {
        category |= ColliderInfo::Category::kTrigger;
    }

    switch (collider_type) {
    case ColliderInfo::Type::Segment:
        return collider_info_ = std::make_shared<ColliderInfoSegment>(is_trigger, category);
    case ColliderInfo::Type::Sphere:
        return collider_info_ = std::make_shared<ColliderInfoSphere> (is_trigger, category);
    case ColliderInfo::Type::Capsule:
        return collider_info_ = std::make_shared<ColliderInfoCapsule>(is_trigger, category);
    case ColliderInfo::Type::Box:
        return collider_info_ = std::make_shared<ColliderInfoBox>    (is_trigger, category);
    default:
        ASSERT(false, "Unknown collider type.");
        return nullptr;
    }
}

