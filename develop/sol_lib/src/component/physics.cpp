#include <DxLib.h>
#include "misc/assert_dx.hpp"
#include "misc/math/math.hpp"
#include "app/game_time.hpp"
#include "component/physics.hpp"
#include "misc/collider/collider_info_all.hpp"

using namespace dx_math;

Physics* Physics::instance_ = nullptr;

Physics& Physics::GetInstance()
{
    ASSERT(instance_ != nullptr, "this->create(); hasn't been called yet.");
    return *instance_;
}

void Physics::Create()
{
    ASSERT(DxLib_IsInit() != 0, "DxLib isn't initialize.");
    ASSERT(instance_ == nullptr, "instance has been created.");
    instance_ = new Physics();
}

void Physics::Destroy()
{
    ASSERT(instance_ != nullptr, "this->create(); hasn't been called yet.");
    delete instance_;
}

Physics::Physics()
{
}

Physics::~Physics()
{
}

void Physics::Entry(CollidableObject* collidable)
{	
    ASSERT(instance_, "haven't been created instance.");

    //! collidable が配列内に見つかれば back を行う
    bool found = std::find(collidables_.begin(), collidables_.end(), collidable) != collidables_.end();
    if (found) {
        ASSERT(found, "the specified object is already registered.");
        return;
    }
    
    collidables_.emplace_back(collidable);
}

void Physics::Exit(CollidableObject* collidable)
{
    //! collidable が配列内に見つかれば erase を行う
    auto count = std::erase_if(collidables_, 
                              [collidable](CollidableObject* target) { 
                              return target == collidable; }
    );

    if (count <= 0) {
        ASSERT(count == 0, "the specified object isn't registered.");
    }
}

void Physics::Update() noexcept
{
    /* HACK: EARLY RETURN;
    *  if (GameTime::GetTimeScaler() < 0.01f) return;
    */
    auto dt = GameTime::GetDeltaTime();
    if (dt < 0.0001f) return;

    //! 移動処理
    for (auto& object : collidables_) {
        if (!object->IsActive()) continue;
        if (object->GetColliderInfo()->GetCategory() == ColliderInfo::Category::kStatic) continue;

        //! Rigidbody 
        auto rd	= object->GetRigidbody();
        if (!rd) continue;

        VECTOR position = object->GetTransform()->GetLocalPosition();
        VECTOR velocity = rd->GetVelocity();

        //! 重力を Velocity に加える
        if (rd->OnGravity()) {
            //! Gravity: 1.f m/s^2
            velocity.y += kGravity * dt;

            if (!rd->IsGrounded()) {
                //! 自由落下の終端加速度
                velocity.y = std::clamp(velocity.y, kMaxFreefallSpeed, 0.f);
            }
        }

        //! 摩擦を加える
        //if (rd->IsGrounded()) {
        //    ApplyFriction(rd, dt);
        //    velocity = rd->GetVelocity();
        //}

        rd->SaveFuturePosition(position + velocity * dt);
    }

    //! 衝突チェック
    auto on_collide_info = CheckCollide();

    //! 座標修正
    FixPosition();

    //! 衝突通知
    for (auto& object : on_collide_info) {
        object.owner->OnCollide(*object.collider);
    }
}

std::vector<Physics::OnCollideInfo> Physics::CheckCollide() const noexcept
{
    std::vector<OnCollideInfo> on_collide_info;
    bool	do_check = true;
    int		count	 =	  0;	/// @brief チェック回数

    //! 衝突通知、座標修正
    while (do_check) {
        do_check = false;
        ++count;

        /// HACK: Playerからの距離を確認, 最適化すべし
        //! オブジェクト総当たり
        for (size_t i = 0; i < collidables_.size(); ++i) {
            for (size_t j = i + 1; j < collidables_.size(); ++j) {
                auto* obj_a = collidables_[i];
                auto* obj_b = collidables_[j];

                //! どちらも静止オブジェクトの場合, 判定しない
                if (obj_a->GetWeight() == CollidableObject::Weight::Static &&
                    obj_b->GetWeight() == CollidableObject::Weight::Static) {
                    continue;
                }

                //! 衝突処理
                if (IsCollide(obj_a, obj_b)) {
                    auto weight_a = obj_a->GetWeight();
                    auto weight_b = obj_b->GetWeight();

                    CollidableObject* higher = obj_a;		/// @brief 重さ：高
                    CollidableObject* lower = obj_b;		/// @brief 重さ：低

                    bool are_trigger = obj_a->GetColliderInfo()->IsTrigger() || obj_b->GetColliderInfo()->IsTrigger();

                    //! トリガーでなければ次目標位置修正
                    if (!are_trigger) {
                        if (weight_a < weight_b) {
                            higher = obj_b;
                            lower = obj_a;
                        }

                        FixFuturePosition(higher, lower);
                    }

                    /// @note 何度も呼ばれる可能性を考慮, 一旦遅延処理用.
                    bool has_higher_info = false;
                    bool has_lower_info = false;

                    //! 衝突通知情報の更新
                    for (const auto& object : on_collide_info) {
                        //! 既に通知リストに含まれている
                        if (object.owner == higher) has_higher_info = true;
                        if (object.owner == lower)  has_lower_info = true;
                    }

                    if (!has_higher_info) {
                        on_collide_info.push_back({ higher, lower });
                    }
                    if (!has_lower_info) {
                        on_collide_info.push_back({ lower, higher });
                    }

                    //! 一度でもヒット+補正したら衝突判定と補正やりなおし
                    if (!are_trigger) do_check = true;// 片方がトリガーならヒットとりなおさない
                }
            }
        }

        //! これ以上は判定を行わない
        if (count > kMaxHitCheck && do_check) {
#if _DEBUG
            printfDx("Loop exceeded the maximum number of evaluations.\n");
#endif
            break;
        }
    }
    return on_collide_info;
}

bool Physics::IsCollide(const CollidableObject* obj_a, const CollidableObject* obj_b) const noexcept
{
    //! まずカテゴリでフィルタリング
    if (!obj_a->GetColliderInfo()->CanCollideWith(obj_b->GetColliderInfo().get())) {
        return false;
    }

    //! スルータグのチェック
    if (obj_a->IsThroughTarget(obj_b) || obj_b->IsThroughTarget(obj_a)) {
        return false;
    }

    //! AABBで大まかな判定
    const AABB& aabb_a = obj_a->GetColliderInfo()->GetAABB();
    const AABB& aabb_b = obj_b->GetColliderInfo()->GetAABB();

    if (!aabb_a.Intersects(aabb_b)) {
        return false;
    }

    //! 詳細な衝突判定
    VECTOR hit_point;
    VECTOR hit_normal;
    float penetration;

    auto rb_a = obj_a->GetRigidbody();
    auto rb_b = obj_b->GetRigidbody();

    //! 次フレームの位置で判定
    VECTOR pos_a = rb_a ? rb_a->GetFuturePosition() : obj_a->GetTransform()->GetLocalPosition();
    VECTOR pos_b = rb_b ? rb_b->GetFuturePosition() : obj_b->GetTransform()->GetLocalPosition();

    return obj_a->GetColliderInfo()->TestCollision(
        obj_b->GetColliderInfo().get(),
        pos_a,
        pos_b,
        hit_point,
        hit_normal,
        penetration
    );
}

void Physics::FixFuturePosition(CollidableObject* higher, CollidableObject* lower) const noexcept
{
    //! 当たり判定の種別ごとに修正方法を変更
    auto higher_type = higher->GetColliderInfo()->GetType();
    auto lower_type  =  lower->GetColliderInfo()->GetType();
    auto higher_rd	 = higher->GetRigidbody();
    auto lower_rd	 =  lower->GetRigidbody();

    //! 静的オブジェクトはRigidbodyを持たない可能性
    if (!lower_rd) return;

    //! 詳細な衝突情報を取得
    VECTOR hit_point;
    VECTOR hit_normal;
    float  penetration;

    VECTOR pos_higher = higher_rd ? higher_rd->GetFuturePosition() : higher->GetTransform()->GetLocalPosition();
    VECTOR pos_lower = lower_rd->GetFuturePosition();

    bool hit = higher->GetColliderInfo()->TestCollision(
        lower->GetColliderInfo().get(),
        pos_higher,
        pos_lower,
        hit_point,
        hit_normal,
        penetration
    );

    if (!hit) return;

    //! 重さに応じた押し出し比率を計算
    float weight_ratio = 1.0f;
    if (higher->GetWeight() != CollidableObject::Weight::Static) {
        float mass_higher = higher_rd ? higher_rd->GetMass() : 1000000.0f;  // 静的は非常に重い
        float mass_lower = lower_rd->GetMass();
        weight_ratio = mass_higher / (mass_higher + mass_lower);
    }

    //! 押し出しベクトル
    VECTOR push_vector = VScale(hit_normal, penetration);

    //! 低い重さのオブジェクトを押し出す
    VECTOR new_pos = pos_lower + VScale(push_vector, weight_ratio);
    lower_rd->SaveFuturePosition(new_pos);

    //! 高い重さのオブジェクトも少し押し戻す（静的でない場合）
    if (higher_rd && higher->GetWeight() != CollidableObject::Weight::Static) {
        VECTOR higher_new_pos = pos_higher - VScale(push_vector, 1.0f - weight_ratio);
        higher_rd->SaveFuturePosition(higher_new_pos);
    }

    //! 接地判定
    //! 法線が上向きで、下方向に移動していた場合は接地
    if (hit_normal.y > 0.7f && lower_rd->GetVelocity().y < 0) {
        lower_rd->SetGrounded(true);

        //! Y方向の速度をリセット
        VECTOR vel = lower_rd->GetVelocity();
        vel.y = 0.0f;
        lower_rd->SetVelocity(vel);
    }
}

void Physics::FixPosition() noexcept
{
    for (auto& object : collidables_) {
        auto rd = object->GetRigidbody();
        if (!rd) continue;

        //! 位置の差分から速度を逆算
        VECTOR position_diff = rd->GetFuturePosition() - object->GetTransform()->GetLocalPosition();

        float dt = GameTime::GetDeltaTime();
        if (dt > 0.0001f) {
            //! 移動量更新
            VECTOR fix_velocity = position_diff * (1.0f / dt);
            rd->SetVelocity(fix_velocity);
        }

        //! 座標更新
        object->GetTransform()->SetLocalPosition(rd->GetFuturePosition());
    }
}

void Physics::ApplyGravity() noexcept
{
}

void Physics::ApplyFriction(const std::shared_ptr<Rigidbody>& rb, float delta_time) noexcept
{
    VECTOR velocity		 = rb->GetVelocity();
    VECTOR ground_normal = rb->GetGroundNormal();

    //! 地面に平行な速度成分を計算
    auto normal_dot_velocity = VDot(velocity, ground_normal);
    auto velocity_parallel   = velocity - (ground_normal * normal_dot_velocity);
    auto speed				 = VSize(velocity_parallel);
    if (speed < 0.001f) return;

    //! 摩擦力の計算
    auto& material			   = rb->GetMaterial();
    auto  friction_coefficient = material.dynamic_friction;

    //! 静摩擦の判定
    if (speed < 0.1f) {
        friction_coefficient = material.static_friction;
    }

    //! 摩擦力 = μ * N（垂直抗力）
    auto normal_force   = rb->GetMass() * fabs(kGravity);
    auto friction_force = friction_coefficient * normal_force;

    //! 摩擦による減速
    auto friction_direction = VNorm(velocity_parallel) * -1.f;
    auto friction_accel     = friction_direction * (friction_force / rb->GetMass());

    //! 速度を更新（摩擦で完全に止まらないように制限）
    auto new_velocity_parallel = velocity_parallel + (friction_accel * delta_time);

    //! 摩擦で逆方向になった場合は停止
    if (VDot(new_velocity_parallel, velocity_parallel) < 0) {
        new_velocity_parallel = VGet(0.f, 0.f, 0.f);
    }

    //! 垂直成分と合成して速度を設定
    VECTOR velocity_normal = ground_normal * normal_dot_velocity;
    rb->SetVelocity(new_velocity_parallel + velocity_normal);
}
