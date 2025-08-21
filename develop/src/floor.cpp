#include <DxLib.h>
#include "floor.hpp"
#include "misc/collider/collider_info_all.hpp"

Floor::Floor(const VECTOR& position, const VECTOR& size)
    : CollidableObject("floor", Weight::Static, ColliderInfo::Type::Box, false)
    , size_(size)
    , texture_(-1)
{
    tag_   = ObjectTag::Floor;
    model_ = std::make_shared<Modeler>(*this);
    transform_->SetLocalPosition(position);
}

Floor::~Floor()
{
}

void Floor::Initialize() noexcept
{
    CollidableObject::Initialize();

    //! Transform
    transform_->SetLocalScale(0.01f);
    transform_->SetLocalRotation(Quaternion::FromEulerDegrees(0.f, 180.f, 0.f));

    // ボックスコライダーの設定
    auto box = std::dynamic_pointer_cast<ColliderInfoBox>(GetColliderInfo());
    if (box) {
        box->SetBox(transform_->GetLocalPosition(), size_ * 0.5);
    }

    //! Modeler
    if (model_) {
        model_->Initialize();
        model_->SetModelHandle("../resources/model/simple/road.mv1");
    }
}

void Floor::Finalize() noexcept
{
    CollidableObject::Finalize();
}

void Floor::Update() noexcept
{
    if (!IsActive()) return;

    // ColliderのAABB更新（初回のみ）
    if (GetColliderInfo() && GetColliderInfo()->NeedsUpdate()) {
        GetColliderInfo()->UpdateAABB(
            transform_->GetLocalPosition(),
            transform_->GetWorldMatrix()
        );
    }

    model_->SetMatrix(transform_->GetLocalMatrix());
}

void Floor::Draw() const noexcept
{
    if (!IsActive()) return;
    model_->Draw();

    VECTOR pos = transform_->GetLocalPosition();
    VECTOR half_size = VScale(size_, 0.5f);

    // 床の頂点座標
    VECTOR vertices[8] = {
        VAdd(pos, VGet(-half_size.x, -half_size.y, -half_size.z)),
        VAdd(pos, VGet(half_size.x, -half_size.y, -half_size.z)),
        VAdd(pos, VGet(half_size.x,  half_size.y, -half_size.z)),
        VAdd(pos, VGet(-half_size.x,  half_size.y, -half_size.z)),
        VAdd(pos, VGet(-half_size.x, -half_size.y,  half_size.z)),
        VAdd(pos, VGet(half_size.x, -half_size.y,  half_size.z)),
        VAdd(pos, VGet(half_size.x,  half_size.y,  half_size.z)),
        VAdd(pos, VGet(-half_size.x,  half_size.y,  half_size.z))
    };

    // 床の色
    unsigned int color = 0x00ffff;

    // 上面（Y+）を描画
    DrawTriangle3D(vertices[3], vertices[2], vertices[6], color, TRUE);
    DrawTriangle3D(vertices[3], vertices[6], vertices[7], color, TRUE);

    // エッジを描画（デバッグ用）
#ifdef _DEBUG
    unsigned int edge_color = 0xffffff;

    // 底面
    DrawLine3D(vertices[0], vertices[1], edge_color);
    DrawLine3D(vertices[1], vertices[2], edge_color);
    DrawLine3D(vertices[2], vertices[3], edge_color);
    DrawLine3D(vertices[3], vertices[0], edge_color);

    // 上面
    DrawLine3D(vertices[4], vertices[5], edge_color);
    DrawLine3D(vertices[5], vertices[6], edge_color);
    DrawLine3D(vertices[6], vertices[7], edge_color);
    DrawLine3D(vertices[7], vertices[4], edge_color);

    // 垂直線
    DrawLine3D(vertices[0], vertices[4], edge_color);
    DrawLine3D(vertices[1], vertices[5], edge_color);
    DrawLine3D(vertices[2], vertices[6], edge_color);
    DrawLine3D(vertices[3], vertices[7], edge_color);

    // AABBの描画
    if (GetColliderInfo()) {
        GetColliderInfo()->GetAABB().Debug(false);
    }
#endif
}

void Floor::LastUpdate() noexcept
{
    // 特に処理なし
}

void Floor::OnCollide(const CollidableObject& object) noexcept
{
    // 静的オブジェクトなので特に反応しない
}