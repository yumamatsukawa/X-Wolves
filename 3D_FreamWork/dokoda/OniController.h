#pragma once
class Scene;
class GameObject;

enum class CameraMode
{
	TPS,
	Overhead,
};

GameObject* CreateCamera(Scene& scene, CameraMode mode);
GameObject* CreateOni(Scene& scene);