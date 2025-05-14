/// @file	ExportFunctions.cpp
///
/// @see	ExportFunctions.h
///
/// @author	RDN © Intern :)
///
/// @date    22.05.2024 22:24
///

#include "stdafx.h"
#include "ExportFunctions.h"

#include <QMainWindow>
#include <app/CloseHandler.h>
#include <app/Trackball.h>
#include <commands/BindIndexBuffer.h>
#include <commands/BindVertexBuffers.h>
#include <commands/Commands.h>
#include <commands/DrawIndexed.h>
#include <io/read.h>
#include <nodes/MatrixTransform.h>
#include <nodes/StateGroup.h>
#include <state/BindDescriptorSet.h>
#include <state/ColorBlendState.h>
#include <state/DepthStencilState.h>
#include <state/DescriptorBuffer.h>
#include <state/DescriptorImage.h>
#include <state/InputAssemblyState.h>
#include <state/MultisampleState.h>
#include <state/RasterizationState.h>
#include <state/VertexInputState.h>
#include <utils/ComputeBounds.h>

#include <vsg/all.h>

#include "Window.h"

namespace
{
	class RotationTrackingMatrix : public vsg::Inherit<vsg::ViewMatrix, RotationTrackingMatrix>
	{
	public:
		RotationTrackingMatrix(vsg::ref_ptr<vsg::ViewMatrix> parentTransform) :
			parentTransform_(parentTransform) {
		}

		// The transform() returns the rotation part of the tracked matrix
		vsg::dmat4 transform() const override
		{

			vsg::dvec3 translation, scale;
			vsg::dquat rotation;
			vsg::decompose(parentTransform_->transform(),
				// output
				translation,
				rotation,
				scale);

			return vsg::rotate(rotation);
		}

	private:
		vsg::ref_ptr<vsg::ViewMatrix> parentTransform_;
	};
	// Create an arrow with the back at pos and pointing in the direction of dir
// Place a cone at the end of the arrow with the color color
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

	// Create a tracking overlay with a axes view that shows the orientation
	// of the main camera view matrix
	vsg::ref_ptr<vsg::View> createAxesView(vsg::ref_ptr<vsg::Camera> camera,
		double aspectRatio)
	{
		auto viewMat = RotationTrackingMatrix::create(camera->viewMatrix);

		// Place the gizmo in the view port by modifying its ortho camera x
		// and y limits
		double camWidth = 10;
		double camXOffs = -8;
		double camYOffs = -8;
		auto ortho = vsg::Orthographic::create(camXOffs, camXOffs + camWidth,                               // left, right
			camYOffs / aspectRatio, (camYOffs + camWidth) / aspectRatio, // bottom, top
			-1000, 1000);                                                // near, far

		auto gizmoCamera = vsg::Camera::create(
			ortho,
			viewMat,
			camera->viewportState);

		auto scene = vsg::Group::create();
		scene->addChild(createGizmo());

		auto view = vsg::View::create(gizmoCamera);
		view->addChild(vsg::createHeadlight());
		view->addChild(scene);
		return view;
	}

	vsg::ref_ptr<vsg::Camera> createCameraForScene(vsg::Node* scenegraph, int32_t x, int32_t y, uint32_t width, uint32_t height)
	{
		// compute the bounds of the scene graph to help position camera
		vsg::ComputeBounds computeBounds;
		scenegraph->accept(computeBounds);
		vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
		double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;
		double nearFarRatio = 0.001;

		// set up the camera
		auto lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0),
			centre, vsg::dvec3(0.0, 0.0, 1.0));

		auto perspective = vsg::Perspective::create(30.0, static_cast<double>(width) / static_cast<double>(height),
			nearFarRatio * radius, radius / nearFarRatio);

		auto viewportstate = vsg::ViewportState::create(x, y, width, height);

		return vsg::Camera::create(perspective, lookAt, viewportstate);
	}

	vsg::ref_ptr<vsg::Node> CreateScene()
	{
		auto scene = vsg::Group::create();
		scene->addChild(createGizmo());
		return scene;
	}

	vsg::ref_ptr<vsg::Node> CreateScene_1()
	{
		auto options = vsg::Options::create();
		options->fileCache = vsg::getEnv("VSG_FILE_CACHE");
		options->paths.emplace_back("d:/Develop/Projects/Qt/vsgExamples/data/") /*= vsg::getEnvPaths("VSG_FILE_PATH")*/;
		// load shaders
		vsg::ref_ptr<vsg::ShaderStage> vertexShader = vsg::ShaderStage::read(VK_SHADER_STAGE_VERTEX_BIT,
			"main", "shaders/vert_PushConstants.spv", options);
		vsg::ref_ptr<vsg::ShaderStage> fragmentShader = vsg::ShaderStage::read(VK_SHADER_STAGE_FRAGMENT_BIT, "main", "shaders/frag_PushConstants.spv", options);
		if (!vertexShader || !fragmentShader)
		{
			//std::cout << "Could not create shaders." << std::endl;
			return {};
		}

		// read texture image
		vsg::Path textureFile("textures/lz.vsgb");
		auto textureData = vsg::read_cast<vsg::Data>(textureFile, options);
		if (!textureData)
		{
			//std::cout << "Could not read texture file : " << textureFile << std::endl;
			return {};
		}

		// set up graphics pipeline
		vsg::DescriptorSetLayoutBindings descriptorBindings{
			{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr}, // { binding, descriptorType, descriptorCount, stageFlags, pImmutableSamplers}
			{1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr}            // { binding, descriptorType, descriptorCount, stageFlags, pImmutableSamplers}
		};

		auto descriptorSetLayout = vsg::DescriptorSetLayout::create(descriptorBindings);

		vsg::PushConstantRanges pushConstantRanges{
			{VK_SHADER_STAGE_VERTEX_BIT, 0, 128} // projection, view and model matrices, actual push constant calls automatically provided by the VSG's RecordTraversal
		};

		vsg::VertexInputState::Bindings vertexBindingsDescriptions{
			VkVertexInputBindingDescription{0, sizeof(vsg::vec3), VK_VERTEX_INPUT_RATE_VERTEX}, // vertex data
			VkVertexInputBindingDescription{1, sizeof(vsg::vec3), VK_VERTEX_INPUT_RATE_VERTEX}, // colour data
			VkVertexInputBindingDescription{2, sizeof(vsg::vec2), VK_VERTEX_INPUT_RATE_VERTEX}  // tex coord data
		};

		vsg::VertexInputState::Attributes vertexAttributeDescriptions{
			VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}, // vertex data
			VkVertexInputAttributeDescription{1, 1, VK_FORMAT_R32G32B32_SFLOAT, 0}, // colour data
			VkVertexInputAttributeDescription{2, 2, VK_FORMAT_R32G32_SFLOAT, 0},    // tex coord data
		};

		vsg::GraphicsPipelineStates pipelineStates{
			vsg::VertexInputState::create(vertexBindingsDescriptions, vertexAttributeDescriptions),
			vsg::InputAssemblyState::create(),
			vsg::RasterizationState::create(),
			vsg::MultisampleState::create(),
			vsg::ColorBlendState::create(),
			vsg::DepthStencilState::create() };

		auto pipelineLayout = vsg::PipelineLayout::create(vsg::DescriptorSetLayouts{ descriptorSetLayout }, pushConstantRanges);
		auto graphicsPipeline = vsg::GraphicsPipeline::create(pipelineLayout, vsg::ShaderStages{ vertexShader, fragmentShader }, pipelineStates);
		auto bindGraphicsPipeline = vsg::BindGraphicsPipeline::create(graphicsPipeline);

		// create texture image and associated DescriptorSets and binding
		auto texture = vsg::DescriptorImage::create(vsg::Sampler::create(), textureData, 0, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

		auto uniformValue = vsg::vec3Value::create(1.0f, 2.0f, 3.0f);
		auto uniform = vsg::DescriptorBuffer::create(uniformValue, 1, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

		auto descriptorSet = vsg::DescriptorSet::create(descriptorSetLayout, vsg::Descriptors{ texture, uniform });
		auto bindDescriptorSets = vsg::BindDescriptorSets::create(VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->layout, 0, vsg::DescriptorSets{ descriptorSet });

		// create StateGroup as the root of the scene/command graph to hold the GraphicsPipeline, and binding of Descriptors to decorate the whole graph
		auto scenegraph = vsg::StateGroup::create();
		scenegraph->add(bindGraphicsPipeline);
		scenegraph->add(bindDescriptorSets);

		// set up model transformation node
		auto transform = vsg::MatrixTransform::create(); // VK_SHADER_STAGE_VERTEX_BIT

		// add transform to root of the scene graph
		scenegraph->addChild(transform);

		// set up vertex and index arrays
		auto vertices = vsg::vec3Array::create(
			{ {-0.5f, -0.5f, 0.0f},
			 {0.5f, -0.5f, 0.05f},
			 {0.5f, 0.5f, 0.0f},
			 {-0.5f, 0.5f, 0.0f},
			 {-0.5f, -0.5f, -0.5f},
			 {0.5f, -0.5f, -0.5f},
			 {0.5f, 0.5f, -0.5},
			 {-0.5f, 0.5f, -0.5} }); // VK_FORMAT_R32G32B32_SFLOAT, VK_VERTEX_INPUT_RATE_INSTANCE, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE

		auto colors = vsg::vec3Array::create(
			{
				{1.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f},
				{1.0f, 1.0f, 1.0f},
				{1.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f},
				{1.0f, 1.0f, 1.0f},
			}); // VK_FORMAT_R32G32B32_SFLOAT, VK_VERTEX_INPUT_RATE_VERTEX, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE

		auto texcoords = vsg::vec2Array::create(
			{ {0.0f, 0.0f},
			 {1.0f, 0.0f},
			 {1.0f, 1.0f},
			 {0.0f, 1.0f},
			 {0.0f, 0.0f},
			 {1.0f, 0.0f},
			 {1.0f, 1.0f},
			 {0.0f, 1.0f} }); // VK_FORMAT_R32G32_SFLOAT, VK_VERTEX_INPUT_RATE_VERTEX, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE

		auto indices = vsg::ushortArray::create(
			{ 0, 1, 2,
			 2, 3, 0,
			 4, 5, 6,
			 6, 7, 4 }); // VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE

		// setup geometry
		auto drawCommands = vsg::Commands::create();
		drawCommands->addChild(vsg::BindVertexBuffers::create(0, vsg::DataList{ vertices, colors, texcoords }));
		drawCommands->addChild(vsg::BindIndexBuffer::create(indices));
		drawCommands->addChild(vsg::DrawIndexed::create(12, 1, 0, 0, 0));

		// add drawCommands to transform
		transform->addChild(drawCommands);

		return scenegraph;
	}

	vsgQt::Window* createWindow(vsg::ref_ptr<vsg::Node> vsg_scene, QWindow* parent, const QString& title = {})
	{
		auto window = new vsgQt::Window(parent);

		window->setTitle(title);

		window->initializeWindow();
		
		if (!vsg_scene)
			vsg_scene = CreateScene();

		// compute the bounds of the scene graph to help position camera
		vsg::ComputeBounds computeBounds;
		vsg_scene->accept(computeBounds);
		vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
		double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;
		double nearFarRatio = 0.001;

		uint32_t width = window->getTraits().width;
		uint32_t height = window->getTraits().height;

		vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel(vsg_scene->getObject<vsg::EllipsoidModel>("EllipsoidModel"));
		vsg::ref_ptr<vsg::Camera> camera;
		{
			// set up the camera
			auto lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

			vsg::ref_ptr<vsg::ProjectionMatrix> perspective;
			if (ellipsoidModel)
			{
				perspective = vsg::EllipsoidPerspective::create(
					lookAt, ellipsoidModel, 30.0,
					static_cast<double>(width) /
					static_cast<double>(height),
					nearFarRatio, false);
			}
			else
			{
				perspective = vsg::Perspective::create(
					30.0,
					static_cast<double>(width) /
					static_cast<double>(height),
					nearFarRatio * radius, radius / nearFarRatio);
			}

			camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(VkExtent2D{ width, height }));
		}

		auto trackball = vsg::Trackball::create(camera, ellipsoidModel);
		trackball->addWindow(*window);

		window->getViewer().addEventHandler(trackball);

		auto commandGraph = vsg::createCommandGraphForView(*window, camera, vsg_scene);

		window->getViewer().addRecordAndSubmitTaskAndPresentation({ commandGraph });

		return window;
	}

}
namespace gallery
{
	QMainWindow* MakeVSGWindow()
	{
		QMainWindow* mainWindow = new QMainWindow();

		vsg::ref_ptr<vsg::Node> vsg_scene;

		auto window = createWindow(vsg_scene, nullptr, "First Window");
		auto& windowTraits = window->getTraits();

		windowTraits.windowTitle = "vsgQt viewer";

		auto widget = QWidget::createWindowContainer(window, mainWindow);

		mainWindow->setCentralWidget(widget);

		mainWindow->setGeometry(windowTraits.x, windowTraits.y, windowTraits.width, windowTraits.height);

		mainWindow->show();

		window->getViewer().compile();

		return mainWindow;
	}
}
