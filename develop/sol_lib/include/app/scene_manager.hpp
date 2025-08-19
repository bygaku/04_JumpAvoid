#pragma once
#include "app/application.hpp"
#include "scene_data/scene_data.hpp"

class Scene;

class SceneManager final
{
public:
    /// @brief インスタンスを初期化
    SceneManager();

    /// @brief インスタンスを解放
    ~SceneManager();
                             
    /// @brief 毎フレーム更新
    void Update()	  noexcept;

    /// @brief 毎フレーム描画
    void Draw()		  noexcept;

#ifdef _DEBUG
    /// @brief 毎フレームデバッグ描画
    void Debug()	  noexcept;
#endif

    /// @brief 描画後更新
    void LastUpdate() noexcept;

    /// @brief シーンを変更
    void ChangeScene(const SceneTag& tag);

    /// @brief 指定シーンを取得
    std::shared_ptr<Scene> GetScene(const SceneTag& tag) const noexcept;

private:
    static bool  instantiated_;

private:
    std::unordered_map<SceneTag, std::shared_ptr<Scene>> scenes_;
    std::shared_ptr<Scene>                               current_scene_;
    std::shared_ptr<Scene>                               next_scene_;

};
