#pragma once
#include "app/application.hpp"
#include "scene_data/scene_data.hpp"

class Scene;

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

#ifdef _DEBUG
    /// @brief ���t���[���f�o�b�O�`��
    void Debug()	  noexcept;
#endif

    /// @brief �`���X�V
    void LastUpdate() noexcept;

    /// @brief �V�[����ύX
    void ChangeScene(const SceneTag& tag);

    /// @brief �w��V�[�����擾
    std::shared_ptr<Scene> GetScene(const SceneTag& tag) const noexcept;

private:
    static bool  instantiated_;

private:
    std::unordered_map<SceneTag, std::shared_ptr<Scene>> scenes_;
    std::shared_ptr<Scene>                               current_scene_;
    std::shared_ptr<Scene>                               next_scene_;

};
