#pragma once
class Scene;
class GameObject;

enum class CameraMode
{
	TPS,
	Overhead,
};

GameObject* CreateCamera(Scene& scene);
GameObject* CreateOni(Scene& scene);