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

namespace model3d
{
	static constexpr double DOUBLE_EMPTY = std::numeric_limits<double>::max();

	struct CPoint2D
	{
		double x = DOUBLE_EMPTY;
		double y = DOUBLE_EMPTY;

		CPoint2D() = default;
		explicit CPoint2D(double v)
			: x(v)
			, y(v)
		{
		}

		bool IsValid() const
		{
			return x != DOUBLE_EMPTY && y != DOUBLE_EMPTY;
		}
	};

	struct CPoint3D
	{
		CPoint2D xy;
		double z = DOUBLE_EMPTY;

		CPoint3D() = default;
		explicit CPoint3D(double v)
			: xy(v)
			, z(v)
		{
		}

		bool IsValid() const
		{
			return xy.IsValid() && z != DOUBLE_EMPTY;
		}
	};

	struct CRect3D
	{
		CPoint3D min = CPoint3D(std::numeric_limits<double>::max());
		CPoint3D max = CPoint3D(std::numeric_limits<double>::min());

		bool IsValid() const
		{
			return min.x < max.x ;
		}
	};

	struct CSphere
	{
		CPoint3D org;
		double r = -1.0;

		bool IsValid() const
		{
			return org.IsValid() && r > .0;
		}
	};

	/*VkPolygonMode { VK_POLYGON_MODE_FILL = 0, VK_POLYGON_MODE_LINE = 1, VK_POLYGON_MODE_POINT = 2,VK_POLYGON_MODE_FILL_RECTANGLE_NV = 1000153000};*/
	enum class PolygonMode : std::int8_t
	{
		FILL,
		LINE,
		POINT,
		RECTANGLE
	};

	/*enum VkCullModeFlagBits {VK_CULL_MODE_NONE = 0,VK_CULL_MODE_FRONT_BIT = 0x00000001,VK_CULL_MODE_BACK_BIT = 0x00000002,VK_CULL_MODE_FRONT_AND_BACK = 0x00000003}*/
	enum class CullMode : std::int8_t
	{
		NONE,
		FRONT,
		BACK,
		FRONT_AND_BACK
	};

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
	struct AxisParams
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
