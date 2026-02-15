/// @file	SolidModelWindow.cpp
///
/// @see	SolidModelWindow.h
///
/// @author	RDN © Intern :)
///
/// @date    15.02.2026 12:51
///

#include "stdafx.h"
#include "SolidModelWindow.h"
#include "VsgQtWindow.h"

#include <vsg/maths/transform.h>
#include <vsg/utils/Builder.h>
#include <vsg/app/Trackball.h>

#include <QtWidgets/QToolBar>
#include <utils/ComputeBounds.h>

namespace
{
	vsg::ref_ptr<vsg::Node> createArrow(vsg::vec3 pos, vsg::vec3 dir, vsg::vec4 color)
	{
		vsg::ref_ptr<vsg::Group> arrow = vsg::Group::create();

		vsg::Builder builder;
		vsg::GeometryInfo geomInfo;
		vsg::StateInfo stateInfo;

		geomInfo.color = vsg::vec4{ 1, 1, 1, 1 };
		geomInfo.position = pos;
		geomInfo.transform = vsg::translate(0.0f, 0.0f, 0.5f);

		// If we don't point in the z-direction, then rotate the arrow
		vsg::vec3 axis = vsg::cross(vsg::vec3{ 0, 0, 1 }, dir);
		if (vsg::length(axis) > 0.0001)
		{
			float angle = acos(vsg::dot(vsg::vec3{ 0, 0, 1 }, dir));
			geomInfo.transform = vsg::rotate(angle, axis) * geomInfo.transform;
		}

		auto axisTransform = geomInfo.transform;
		geomInfo.transform = geomInfo.transform * vsg::scale(0.1f, 0.1f, 1.0f);

		// Rotate geomInfo from pos in the direction of dir
		auto node = builder.createCylinder(geomInfo, stateInfo);
		arrow->addChild(node);

		// The cone
		geomInfo.color = color;
		// This would have been cleaner with a pre_translate transform
		geomInfo.transform = vsg::scale(0.3f, 0.3f, 0.3f) * axisTransform * vsg::translate(0.0f, 0.0f, 1.0f / 0.3f);
		node = builder.createCone(geomInfo, stateInfo);
		arrow->addChild(node);

		return arrow;
	}

	// Create three arrows of the coordinate axes
	vsg::ref_ptr<vsg::Node> createGizmo()
	{
		vsg::ref_ptr<vsg::Group> gizmo = vsg::Group::create();

		auto org = vsg::vec3{ 0, 0, 0 };

		gizmo->addChild(createArrow(org, vsg::vec3{ 1, 0, 0 }, vsg::vec4{ 1, 0, 0, 1 }));
		gizmo->addChild(createArrow(org, vsg::vec3{ 0, 1, 0 }, vsg::vec4{ 0, 1, 0, 1 }));
		gizmo->addChild(createArrow(org, vsg::vec3{ 0, 0, 1 }, vsg::vec4{ 0, 0, 1, 1 }));

		vsg::Builder builder;
		vsg::GeometryInfo geomInfo;
		vsg::StateInfo stateInfo;
		geomInfo.color = vsg::vec4{ 1, 1, 1, 1 };
		geomInfo.transform = vsg::scale(0.1f, 0.1f, 0.1f);

		auto sphere = builder.createSphere(geomInfo, stateInfo);
		gizmo->addChild(sphere);

		return gizmo;
	}
}

namespace model3d
{
	struct SolidModelWindow::Impl
	{
		vsgQt::Window * vsgWind = nullptr;
		vsg::ref_ptr<vsg::Camera> camera;
		vsg::ref_ptr<vsg::Trackball> trackball;
		vsg::ref_ptr<vsg::Group> scene;

		void CreateWnd(QWidget * parent, QString const & title = {})
		{
			vsgWind = new vsgQt::Window(parent);

			vsgWind->initializeWindow();

			camera = vsg::Camera::create();
			camera->viewMatrix = vsg::LookAt::create();
			trackball = vsg::Trackball::create(camera, nullptr);
			trackball->addWindow(*vsgWind);

			vsgWind->getViewer().addEventHandler(trackball);
			scene = vsg::Group::create();
			Rebuild(false);

			auto commandGraph = vsg::createCommandGraphForView(*vsgWind, camera, scene);

			vsgWind->getViewer().addRecordAndSubmitTaskAndPresentation({ commandGraph });
			vsgWind->getViewer().compile();
		}

		void Rebuild(bool compile = true)
		{
			vsgWind->getViewer().deviceWaitIdle();
			scene->children.clear();
			scene->addChild(createGizmo());

			// compute the bounds of the scene graph to help position camera
			vsg::ComputeBounds computeBounds;
			scene->accept(computeBounds);
			vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
			double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;
			double nearFarRatio = 0.001;

			uint32_t width = vsgWind->width();
			uint32_t height = vsgWind->height();

			auto lookAt = vsg::LookAt(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0),
				centre, vsg::dvec3(0.0, 0.0, 1.0));

			*(camera->viewMatrix.cast<vsg::LookAt>()) = lookAt;
			camera->projectionMatrix = vsg::Perspective::create(
				30.0,
				static_cast<double>(width) /
				static_cast<double>(height),
				nearFarRatio * radius, radius / nearFarRatio);
			camera->viewportState = vsg::ViewportState::create(VkExtent2D{ width, height });

			if (!compile)
				return;

			auto result = vsgWind->getViewer().compile();
			vsg::updateViewer(vsgWind->getViewer(), result);
		}
	};

	SolidModelWindow::SolidModelWindow(QWidget * parent)
		: base_t(parent)
		, m_impl(new Impl)
	{
		m_impl->CreateWnd(nullptr);
		auto & windowTraits = m_impl->vsgWind->getTraits();

		windowTraits.windowTitle = "vsgQt viewer";

		setCentralWidget(m_impl->vsgWind);
		setGeometry(windowTraits.x, windowTraits.y, windowTraits.width, windowTraits.height);

		QToolBar * toolBar = addToolBar(tr("Rebuild"));
		toolBar->addAction("1", [this]() {OnRebuild(); });
	}

	SolidModelWindow::~SolidModelWindow() = default;

	void SolidModelWindow::OnRebuild()
	{
		m_impl->Rebuild();
		update();
	}
}
