#pragma once
#include "scene/scene.hpp"

class SceneManager final
{
public:
    /// @brief �C���X�^���X��������
    SceneManager();

    /// @brief �C���X�^���X�����
    ~SceneManager();
                             
    /// @brief ���t���[���X�V
    void Update()	  noexcept;

    /// @brief ���t���[���`��
    void Draw()		  noexcept;

    /// @brief ���t���[���f�o�b�O�`��
    void Debug()	  noexcept;

    /// @brief �`���X�V
    void LastUpdate() noexcept;

    /// @brief �V�[����ύX
    void ChangeScene(const std::string& tag);

    /// @brief �V�[����o�^
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

    /// @brief �w��V�[�����擾
    std::shared_ptr<Scene> GetScene(const std::string& tag) const noexcept;

private:
    static bool  instantiated_;

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
    
    std::shared_ptr<Scene>                                  current_scene_;
    std::shared_ptr<Scene>                                  next_scene_;

};
