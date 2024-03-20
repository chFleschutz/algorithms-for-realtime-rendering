#ifndef ECS_HPP
#define ECS_HPP

#include "thirdparty/entt/single_include/entt/entt.hpp"

//!Abkürzung für Zugriff auf ECS aka Registry
class SORS_ECS
{
public:
    static entt::registry sRegistry;
};

#define ECS SORS_ECS::sRegistry

#endif // ECS_HPP
