/// @file	3DStructures.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    11.12.2025 22:36
///

#pragma once
#include <utility>
#include <vector>

struct CPoint3D
{
	double x = .0;
	double y = .0;
	double z = .0;
};

namespace model3d
{
	enum class SelectType
	{
		underCursor,
		selected,

		collisionObj1,
		collisionObj2,

		collisionNetwork
	};


	// Данные для всеменной отрисовки
	struct DrawTmpData
	{
		std::vector<std::pair<CPoint3D, CPoint3D>> m_forDimDistance;
	};

	// Параметры для настройки осей в панели 3D
	struct PDEXTGUICLS_EXT_CLASS AxisParams
	{
		int lineAxisLength = 100;
		int lineAxisWidth = 3;
		bool isAxisScaling = false;
		bool areAxisVisible = true;
		bool isHideNegativeAxis = false;
	};

	struct OrbitCameraManipulatorState
	{
		CPoint3D	m_eye;
		CPoint3D	m_lookat;
		CPoint3D	m_up;
		CPoint3D	m_rotate_center;
		CPoint3D	m_ray_pointer_direction;
		CPoint3D	m_ray_pointer_start;
		CPoint3D	m_pointer_intersection;
		double		m_model_screen[4][4];
		double		m_screen_model[4][4];
		CPoint3D	m_pan_point;
	};	
}
