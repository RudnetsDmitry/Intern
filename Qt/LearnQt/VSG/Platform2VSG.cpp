/// @file	Platform2VSG.cpp
///
/// @see	Platform2VSG.h
///
/// @author	RDN © Intern :)
///
/// @date    20.12.2025 18:15
///

#include "stdafx.h"
#include "Platform2VSG.h"

namespace model3d
{
	vsg::dvec3 PointToVec(CPoint3D const & pt)
	{
		return {pt.x, pt.y. pt.z};
	}
}
