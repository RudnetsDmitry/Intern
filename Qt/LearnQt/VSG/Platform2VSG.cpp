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
#include "3DStructures.h"

namespace model3d
{
	vsg::dvec3 PointToVec(CPoint3D const & pt)
	{
		return {pt.xy.x, pt.xy.y, pt.z};
	}

	CPoint3D VecToPoint(vsg::dvec3 const & pt)
	{
		return {pt.x, pt.y, pt.z};
	}
}
