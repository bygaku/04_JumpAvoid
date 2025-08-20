#pragma once
#include "scene/scene.hpp"

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

    /// @brief 毎フレームデバッグ描画
    void Debug()	  noexcept;

    /// @brief 描画後更新
    void LastUpdate() noexcept;

    /// @brief シーンを変更
    void ChangeScene(const std::string& tag);

    /// @brief シーンを登録
    template<typename TScene>
    typename std::enable_if<std::is_base_of<Scene, TScene>::value, void>::type
    RegisterScene(const std::string& tag) {
        if (scenes_.find(tag) == scenes_.end()) {
            scenes_.emplace(tag, std::make_unique<TScene>(*this));

            if (scenes_.size() == 1) {
                scenes_[tag]->Initialize();
                current_scene_ = scenes_[tag];
            }
        }
    }

    /// @brief 指定シーンを取得
    std::shared_ptr<Scene> GetScene(const std::string& tag) const noexcept;

private:
    static bool  instantiated_;

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
    
    std::shared_ptr<Scene>                                  current_scene_;
    std::shared_ptr<Scene>                                  next_scene_;

};
