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

#include "Window.h"

namespace
{
	vsg::ref_ptr<vsg::Node> CreateScene()
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

	vsgQt::Window* createWindow(vsg::ref_ptr<vsgQt::Viewer> viewer, vsg::ref_ptr<vsg::WindowTraits> traits,
		vsg::ref_ptr<vsg::Node> vsg_scene, QWindow* parent, const QString& title = {})
	{
		auto window = new vsgQt::Window(viewer, traits, parent);

		window->setTitle(title);

		window->initializeWindow();

		// if this is the first window to be created, use its device for future window creation.
		if (!traits->device)
			traits->device = window->getOrCreateDevice();

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
					nearFarRatio * radius, radius * 4.5);
			}

			camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(VkExtent2D{ width, height }));
		}

		auto trackball = vsg::Trackball::create(camera, ellipsoidModel);
		trackball->addWindow(*window);

		viewer->addEventHandler(trackball);

		auto commandGraph = vsg::createCommandGraphForView(*window, camera, vsg_scene);

		viewer->addRecordAndSubmitTaskAndPresentation({ commandGraph });

		return window;
	}

}
namespace gallery
{
	QMainWindow* MakeVSGWindow()
	{
		QMainWindow* mainWindow = new QMainWindow();

		auto windowTraits = vsg::WindowTraits::create();
		windowTraits->windowTitle = "vsgQt viewer";

		// create the viewer that will manage all the rendering of the views
		auto viewer = vsgQt::Viewer::create();

		vsg::ref_ptr<vsg::Node> vsg_scene;

		auto window = createWindow(viewer, windowTraits, vsg_scene, nullptr, "First Window");

		auto widget = QWidget::createWindowContainer(window, mainWindow);

		mainWindow->setCentralWidget(widget);

		mainWindow->setGeometry(windowTraits->x, windowTraits->y, windowTraits->width, windowTraits->height);

		mainWindow->show();

		viewer->addEventHandler(vsg::CloseHandler::create(viewer));
		viewer->compile();

		return mainWindow;
	}
}
