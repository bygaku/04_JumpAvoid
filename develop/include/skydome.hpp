#pragma once
#include "object/game_object.hpp"
#include "Component/modeler.hpp"

class Skydome final : public GameObject
{
public:
    /// @brief インスタンスを初期化
    Skydome();

    /// @brief インスタンスを解放
    ~Skydome() override;

    /// @brief 初期化
    void Initialize() noexcept override;

    /// @brief 破棄
    void Finalize()   noexcept override;

    /// @brief 更新
    void Update()	  noexcept override;

    /// @brief 描画
    void Draw() const noexcept override;

    /// @brief 描画後更新
    void LastUpdate() noexcept override;

    /// @brief 親を設定
    void SetParent(std::shared_ptr<Transform> parent) noexcept;

    /// @brief 親を削除
    void RemoveParent() noexcept;

private:
    std::shared_ptr<Modeler> model_;

};
