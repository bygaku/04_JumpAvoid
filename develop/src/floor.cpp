#include <DxLib.h>
#include "floor.hpp"
#include "misc/collider/collider_info_all.hpp"

Floor::Floor(const VECTOR& position, const VECTOR& size)
    : CollidableObject("floor", Weight::Static, ColliderInfo::Type::Box, false)
    , size_(size)
    , texture_(-1)
{
    // �^�O��ݒ�
    tag_ = ObjectTag::Floor;

    // �����ʒu��ݒ�
    transform_->SetLocalPosition(position);
}

Floor::~Floor()
{
}

void Floor::Initialize() noexcept
{
    CollidableObject::Initialize();

    // �{�b�N�X�R���C�_�[�̐ݒ�
    auto box = std::dynamic_pointer_cast<ColliderInfoBox>(GetColliderInfo());
    if (box) {
        box->SetBox(transform_->GetLocalPosition(), size_ * 0.5);
    }
}

void Floor::Finalize() noexcept
{
    CollidableObject::Finalize();
}

void Floor::Update() noexcept
{
    if (!IsActive()) return;

    // �ÓI�I�u�W�F�N�g�Ȃ̂œ��ɍX�V�����͂Ȃ�

    // Collider��AABB�X�V�i����̂݁j
    if (GetColliderInfo() && GetColliderInfo()->NeedsUpdate()) {
        GetColliderInfo()->UpdateAABB(
            transform_->GetLocalPosition(),
            transform_->GetWorldMatrix()
        );
    }
}

void Floor::Draw() const noexcept
{
    if (!IsActive()) return;

    VECTOR pos = transform_->GetLocalPosition();
    VECTOR half_size = VScale(size_, 0.5f);

    // ���̒��_���W
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

    // ���̐F
    unsigned int color = 0x00ffff;

    // ��ʁiY+�j��`��
    DrawTriangle3D(vertices[3], vertices[2], vertices[6], color, TRUE);
    DrawTriangle3D(vertices[3], vertices[6], vertices[7], color, TRUE);

    // �G�b�W��`��i�f�o�b�O�p�j
#ifdef _DEBUG
    unsigned int edge_color = 0xffffff;

    // ���
    DrawLine3D(vertices[0], vertices[1], edge_color);
    DrawLine3D(vertices[1], vertices[2], edge_color);
    DrawLine3D(vertices[2], vertices[3], edge_color);
    DrawLine3D(vertices[3], vertices[0], edge_color);

    // ���
    DrawLine3D(vertices[4], vertices[5], edge_color);
    DrawLine3D(vertices[5], vertices[6], edge_color);
    DrawLine3D(vertices[6], vertices[7], edge_color);
    DrawLine3D(vertices[7], vertices[4], edge_color);

    // ������
    DrawLine3D(vertices[0], vertices[4], edge_color);
    DrawLine3D(vertices[1], vertices[5], edge_color);
    DrawLine3D(vertices[2], vertices[6], edge_color);
    DrawLine3D(vertices[3], vertices[7], edge_color);

    // AABB�̕`��
    if (GetColliderInfo()) {
        GetColliderInfo()->GetAABB().Debug(false);
    }
#endif
}

void Floor::LastUpdate() noexcept
{
    // ���ɏ����Ȃ�
}

void Floor::OnCollide(const CollidableObject& object) noexcept
{
    // �ÓI�I�u�W�F�N�g�Ȃ̂œ��ɔ������Ȃ�
}