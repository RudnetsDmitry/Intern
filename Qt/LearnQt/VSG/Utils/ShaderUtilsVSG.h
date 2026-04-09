/// @file	ShaderUtilsVSG.h
///
/// @brief see PROJECT CHRONO - http://projectchrono.org
///
/// @author	RDN © Intern :)
///
/// @date    28.02.2026 10:46
///

#pragma once
#include <memory>
#include <optional>
#include <state/material.h>


namespace vsg
{
	class ShaderSet;
	class StateGroup;
}

namespace vsg3d {
	struct LineStippleInfo
	{
		uint32_t	lineStippleFactor;
		uint16_t	lineStipplePattern;
	};

	//vsg::ref_ptr<vsg::ShaderSet> createPbrShaderSet(vsg::ref_ptr<const vsg::Options> options);

	vsg::ref_ptr<vsg::StateGroup> createLineStateGroup(vsg::ref_ptr<const vsg::Options> options,
														VkPrimitiveTopology topology,
														float line_width,
														bool skipZbuffer = false,
														std::optional<LineStippleInfo> lineStipple = {});

	//vsg::ref_ptr<vsg::StateGroup> createPbrStateGroup(vsg::ref_ptr<const vsg::Options> options,
	//                                                  std::shared_ptr<ChVisualMaterial> material,
	//                                                  bool wireframe);
	//
	//vsg::ref_ptr<vsg::PbrMaterialValue> createPbrMaterialFromChronoMaterial(std::shared_ptr<ChVisualMaterial> chronoMat);
	//vsg::ref_ptr<vsg::PhongMaterialValue> createPhongMaterialFromChronoMaterial(
	//    std::shared_ptr<ChVisualMaterial> chronoMat);

}  // namespace vsg3d
