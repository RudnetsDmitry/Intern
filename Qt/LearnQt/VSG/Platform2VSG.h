/// @file	Platform2VSG.h
///
/// @brief convert platform structures to vsg
///
/// @author	RDN © Intern :)
///
/// @date    20.12.2025 18:12
///

#pragma once
#include <maths/vec3.h>

namespace model3d
{
	struct CPoint3D;

	vsg::dvec3 PointToVec(CPoint3D const & pt);
	CPoint3D VecToPoint(vsg::dvec3 const & pt);
}
