#pragma once
#include <string>

//static struct ObjectTag
//{
//    std::string kBox     = "Box";
//    std::string kText    = "Text";
//    std::string kCamera  = "Camera";
//    std::string kPlayer  = "Player";
//    std::string kPlane   = "Plane";
//    std::string kScore   = "Score";
//}ObjectTag;

/// @brief オブジェクト判別タグ
enum class ObjectTag {
    None,
    Wall,           
    SystemWall,
    Floor,
    Skydome,
    Camera,
    Player,
    Enemy,
    Car,
    RockBlock,
    WoodBlock,
    Terrain
};