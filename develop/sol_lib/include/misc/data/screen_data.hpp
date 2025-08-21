#pragma once

namespace screen
{
#ifdef _RELEASE
    static constexpr float kWidthF      = 1920.0f;  /// @brief <float> ��ʉ��T�C�Y
    static constexpr float kHeightF     = 1080.0f;  /// @brief <float> ��ʏc�T�C�Y
    static constexpr int   kWidth       =    1920;  /// @brief ��ʉ��T�C�Y
    static constexpr int   kHeight      =    1080;  /// @brief ��ʏc�T�C�Y

    static constexpr float kHalfWidthF  =  960.0f;  /// @brief <float> ��ʉ��n�[�t�T�C�Y
    static constexpr float kHalfHeightF =  540.0f;  /// @brief <float> ��ʏc�n�[�t�T�C�Y
    static constexpr int   kHalfWidth   =     960;  /// @brief ��ʉ��n�[�t�T�C�Y
    static constexpr int   kHalfHeight  =     540;  /// @brief ��ʏc�n�[�t�T�C�Y

    static constexpr int   kWindowMode  =   false;
#else
    static constexpr float kWidthF      = 1280.0f;  /// @brief <float> ��ʉ��T�C�Y
    static constexpr float kHeightF     =  720.0f;  /// @brief <float> ��ʏc�T�C�Y
    static constexpr int   kWidth       =    1280;  /// @brief ��ʉ��T�C�Y
    static constexpr int   kHeight      =     720;  /// @brief ��ʏc�T�C�Y

    static constexpr float kHalfWidthF  =  640.0f;  /// @brief <float> ��ʉ��n�[�t�T�C�Y
    static constexpr float kHalfHeightF =  360.0f;  /// @brief <float> ��ʏc�n�[�t�T�C�Y
    static constexpr int   kHalfWidth   =     640;  /// @brief ��ʉ��n�[�t�T�C�Y
    static constexpr int   kHalfHeight  =     360;  /// @brief ��ʏc�n�[�t�T�C�Y

    static constexpr int   kWindowMode  =    true;
#endif
    static constexpr int   kColorBit    =      16;  /// @brief �J���[�r�b�g
}