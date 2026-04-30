/// @file	SolidModelWindow.cpp
///
/// @see	SolidModelWindow.h
///
/// @author	RDN © Intern :)
///
/// @date    15.02.2026 12:51
///

#include "stdafx.h"

#include "3DSystemBase.h"
#include "I3DSystem.h"
#include "Platform2VSG.h"
#include "SolidModelWindow.h"

#include <app/View.h>
#include <io/read.h>
#include <vsg/nodes/MatrixTransform.h>

#include "VsgQtWindow.h"

#include <vsg/app/Trackball.h>
#include <vsg/app/RenderGraph.h>
#include <vsg/maths/transform.h>
#include <vsg/nodes/StateGroup.h>
#include <vsg/nodes/Switch.h>
#include <vsg/nodes/VertexIndexDraw.h>
#include <vsg/utils/Builder.h>
#include <vsg/text/TextGroup.h>


#include <vsg/utils/ComputeBounds.h>
#include <vsg/utils/GraphicsPipelineConfigurator.h>

#include <vsgXchange/models.h>
#include <vsgXchange/all.h>

#include <QtWidgets/QToolBar>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>
#include <utils/LineSegmentIntersector.h>

namespace
{
	struct ChangeDynamicProperty : public vsg::Visitor
	{
		void apply(vsg::Object& object) override
		{
			object.traverse(*this);
		}

		void apply(vsg::VertexIndexDraw& vid) override
		{
			for (auto& buffer : vid.arrays)
			{
				auto data = buffer->data;

				if (data->is_compatible(typeid(vsg::vec4Array))) 
				{
					if (!m_processData.insert(data.get()).second)
						return;

					// Change the property to dynamic
					data->properties.dataVariance = vsg::DYNAMIC_DATA;
					data->dirty();
				}
			}
		}
		std::set<void*> m_processData;
	};

	class IntersectionHandler : public vsg::Inherit<vsg::Visitor, IntersectionHandler>
	{
	public:
		IntersectionHandler(vsg::Node * scene,
			vsg::ref_ptr<vsg::Camera> camera
		)
		: m_scene(scene)
		, m_camera(std::move(camera))
		{
		}

		void apply(vsg::ButtonPressEvent & buttonPressEvent) override
		{
			intersection(buttonPressEvent);
		}

		void apply(vsg::KeyReleaseEvent & keyReleaseEvent) override
		{
			if (keyReleaseEvent.keyBase == vsg::KEY_Escape)
				selectNode(nullptr);
		}

		void apply(vsg::KeyPressEvent & keyReleaseEvent) override
		{
			if (keyReleaseEvent.keyBase == vsg::KEY_Escape)
				selectNode(nullptr);
		}
		void clear()
		{
			m_selectedNode = nullptr;
		}

		void selectNode(vsg::Node const * node = nullptr)
		{
			if (m_selectedNode)
			{
				if (m_selectedNode == node)
					return;

				setColor(const_cast<vsg::Node*>(m_selectedNode), m_prevColor);
			}
			m_selectedNode = node;
			if (m_selectedNode)
				setColor(const_cast<vsg::Node*>(m_selectedNode), selected_color, &m_prevColor);
		}

		void intersection(vsg::PointerEvent & pointerEvent)
		{
			auto intersector = vsg::LineSegmentIntersector::create(*m_camera, pointerEvent.x, pointerEvent.y);
			m_scene->accept(*intersector);
			//fmt::print("Intersected {} objects\n", intersector->intersections.size());
			if (intersector->intersections.empty())
				return;

			// sort the intersections front to back
			std::sort(intersector->intersections.begin(),
				intersector->intersections.end(),
				[](auto& lhs, auto& rhs) { return lhs->ratio < rhs->ratio; });


			auto& intersection = intersector->intersections[0];
			for (const vsg::Node * node : intersection->nodePath)
			{
				std::string typeName;
				//            std::cout << "className=" << node->className();
				node->getValue("type", typeName);
				if (typeName == "obj")
				{
					selectNode(node);
					return;
				}
			}
		}

		void setColor(vsg::Node * node, vsg::vec3 const & color,
			vsg::vec3 * curColor = nullptr)
		{
			struct ChangeColor : public vsg::Visitor
			{
				ChangeColor(vsg::vec3 const & color,
									 vsg::vec3 * curColor)
				: m_color(color)
				, m_curColor(curColor)
				{}
				
				void apply(vsg::Object & object) override
				{
					object.traverse(*this);
				}

				void apply(vsg::StateGroup & sg) override
				{
					sg.traverse(*this);
				}

				void apply(vsg::VertexIndexDraw & vid) override
				{
					for (auto & buffer : vid.arrays)
					{
						auto data = buffer->data;

						if (data->is_compatible(typeid(vsg::vec4Array)))
						{
							if (!m_processData.insert(data.get()).second)
								return;

							// Change the color to green
							auto & color = *(vsg::vec4*)(data->dataPointer(0));
							if (m_curColor)
								*m_curColor = { color.x, color.y, color.z };
							color = vsg::vec4{ m_color.x, m_color.y, m_color.z, 1.0 };
							//fmt::print("Changed color to ({},{},{},{})\n",
							//	_color.r, _color.g, _color.b, _color.a
							//);
							data->dirty();
						}
						else if (data->is_compatible(typeid(vsg::vec3Array)))
						{
							/*auto & color = *(vsg::vec3*)(data->dataPointer(0));
							if (m_curColor)
								*m_curColor = color;
							color = m_color;
							data->dirty();*/
						}
					}
				}
				vsg::vec3 m_color;
				vsg::vec3 * m_curColor = nullptr;
				std::set<void*> m_processData;
			};

			ChangeColor cc(color, curColor);
			node->accept(cc);
		}

		static constexpr vsg::vec3 default_color = vsg::vec3{ 0.5, 0.5, 0.5 }; // Default color
		static constexpr vsg::vec3 selected_color = vsg::vec3{ 1.0, 0, 0 }; // Select color

		vsg::Node const* m_selectedNode = nullptr;
		vsg::vec3 m_prevColor;
		vsg::ref_ptr<vsg::Node> m_scene;
		vsg::ref_ptr<vsg::Camera> m_camera;
	};

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

	vsg::ref_ptr<vsg::Node> CreateBox()
	{
		vsg::GeometryInfo geomInfo;
		vsg::StateInfo stateInfo;
		vsg::Builder builder;
		geomInfo.color = vsg::vec4{ 0, 1, 1, 1 };
		auto box = builder.createBox(geomInfo, stateInfo);
		vsg::ComputeBounds computeBounds;
		computeBounds.apply(*box);
		float radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min);

		auto switchNode = vsg::Switch::create();

		{
			auto transform = vsg::MatrixTransform::create();
			transform->setValue("type", "obj");
			transform->addChild(box);
			vsg::visit<ChangeDynamicProperty>(transform);
			switchNode->addChild(true, transform);

		}

		{
			auto transform = vsg::MatrixTransform::create();
			transform->setValue("type", "obj");
			transform->matrix = vsg::translate(3.0f * radius, 0.0f, 0.0f);
			transform->addChild(box);
			vsg::visit<ChangeDynamicProperty>(transform);
			switchNode->addChild(true, transform);
		}

		{
			auto transform = vsg::MatrixTransform::create();
			transform->setValue("type", "obj");
			transform->matrix = vsg::translate(-3.0f * radius, 0.0f, 0.0f);
			transform->addChild(box);
			vsg::visit<ChangeDynamicProperty>(transform);
			switchNode->addChild(true, transform);
		}

		return switchNode;
	}

	std::string VERT{ R"(
#version 450
layout(push_constant) uniform PushConstants { mat4 projection; mat4 modelView; };
layout(location = 0) in vec3 vertex;
out gl_PerVertex { vec4 gl_Position; };
void main() { gl_Position = (projection * modelView) * vec4(vertex, 1.0); }
)" };

	std::string FRAG0{ R"(
#version 450
layout(set = 0, binding = 0) readonly buffer CellColors { vec4[] cellColors; };
layout(location = 0) out vec4 color;
void main() { color = cellColors[gl_PrimitiveID]; }
)" };

	vsg::ref_ptr<vsg::StateGroup> createScene0()
	{
		auto vertexShader = vsg::ShaderStage::create(VK_SHADER_STAGE_VERTEX_BIT, "main", VERT);
		auto fragmentShader = vsg::ShaderStage::create(VK_SHADER_STAGE_FRAGMENT_BIT, "main", FRAG0);
		auto shaderSet = vsg::ShaderSet::create(vsg::ShaderStages{ vertexShader, fragmentShader });
		shaderSet->addPushConstantRange("pc", "", VK_SHADER_STAGE_VERTEX_BIT, 0, 128);

		shaderSet->addDescriptorBinding("cellColors", "", 0, 0,
			VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT,
			vsg::vec4Array::create({ {1, 0, 0, 1} }));

		shaderSet->addAttributeBinding("vertex", "", 0, VK_FORMAT_R32G32B32_SFLOAT,
			vsg::vec3Array::create(1));

		auto gpConf = vsg::GraphicsPipelineConfigurator::create(shaderSet);

		/**
		 * This array is used in the fragment shader to color each individual triangle
		 * using gl_PrimitiveID as the array index. In principle, this could be
		 * a very large array that would exceed the uniform buffer size limits.
		 */
		auto cellColors = vsg::vec4Array::create({
			{0.176, 0.408, 0.376, 1.0}, // Powderkeg Blue (triangle 0)
			{0.949, 0.663, 0.000, 1.0}, // Westwood Gold  (triangle 1)
			});

		gpConf->assignDescriptor("cellColors", cellColors);

		/// single quad
		auto vertices = vsg::vec3Array::create({ {-1, 0, -1}, {1, 0, -1}, {1, 0, 1}, {-1, 0, 1} });
		auto indices = vsg::uintArray::create({ 0, 1, 2, 0, 2, 3 });
		vsg::DataList vertexArrays;
		gpConf->assignArray(vertexArrays, "vertex", VK_VERTEX_INPUT_RATE_VERTEX, vertices);

		gpConf->init();

		auto stateGroup = vsg::StateGroup::create();
		gpConf->copyTo(stateGroup);

		auto vertexDraw = vsg::VertexIndexDraw::create();
		vertexDraw->assignArrays(vertexArrays);
		vertexDraw->assignIndices(indices);
		vertexDraw->indexCount = static_cast<uint32_t>(indices->size());
		vertexDraw->instanceCount = 1;
		stateGroup->addChild(vertexDraw);

		return stateGroup;
	}

	std::string FRAG1{ R"(
#version 450
layout(location = 0) out vec4 color;
void main() { color = vec4(0.6, 0.6, 0.6, 1.0); }
)" };

	vsg::ref_ptr<vsg::StateGroup> createScene1()
	{
		auto vertexShader = vsg::ShaderStage::create(VK_SHADER_STAGE_VERTEX_BIT, "main", VERT);
		auto fragmentShader = vsg::ShaderStage::create(VK_SHADER_STAGE_FRAGMENT_BIT, "main", FRAG1);
		auto shaderSet = vsg::ShaderSet::create(vsg::ShaderStages{ vertexShader, fragmentShader });
		shaderSet->addPushConstantRange("pc", "", VK_SHADER_STAGE_VERTEX_BIT, 0, 128);

		shaderSet->addAttributeBinding("vertex", "", 0, VK_FORMAT_R32G32B32_SFLOAT,
			vsg::vec3Array::create(1));

		auto gpConf = vsg::GraphicsPipelineConfigurator::create(shaderSet);

		/// single quad
		auto vertices = vsg::vec3Array::create({ {-1, 0, -1}, {1, 0, -1}, {1, 0, 1}, {-1, 0, 1} });
		auto indices = vsg::uintArray::create({ 0, 1, 2, 0, 2, 3 });
		vsg::DataList vertexArrays;
		gpConf->assignArray(vertexArrays, "vertex", VK_VERTEX_INPUT_RATE_VERTEX, vertices);

		gpConf->init();

		auto stateGroup = vsg::StateGroup::create();
		gpConf->copyTo(stateGroup);

		auto vertexDraw = vsg::VertexIndexDraw::create();
		vertexDraw->assignArrays(vertexArrays);
		vertexDraw->assignIndices(indices);
		vertexDraw->indexCount = static_cast<uint32_t>(indices->size());
		vertexDraw->instanceCount = 1;
		stateGroup->addChild(vertexDraw);

		return stateGroup;
	}
}

namespace 
{
	class NodeNameConverter : public model3d::INodeNameConvertor
	{
	public:
		bool isValidNodeName(std::string const & nodeName) const override
		{
			return true;
		}

		std::vector<size_t> nodeName2Key(std::string const & nodeName) const override
		{
			return {};
		}
	};
}
namespace model3d
{
	class SolidModelWindow::Impl
	{
		vsgQt::Window * m_vsgWind;
		vsg::ref_ptr<vsg::Camera> m_camera;
		vsg::ref_ptr<vsg::Camera> m_hudCamera;
		vsg::ref_ptr<vsg::TextGroup> m_hudTextGr;
		vsg::ref_ptr<vsg::View> m_hudView;
		vsg::ref_ptr<vsg::Trackball> m_trackball;
		vsg::ref_ptr<IntersectionHandler> m_intersectionHandler;
		std::unique_ptr<I3DSystem> m_sys;
		vsg::ref_ptr<vsg::RenderGraph> m_renderGraph;

	public:
		explicit Impl(QWidget * parent)
			: m_vsgWind(new vsgQt::Window(parent))
			, m_sys(MakeBase3DSystem(std::make_unique<NodeNameConverter>()))
		{
			m_vsgWind->initializeWindow();

			m_camera = vsg::Camera::create();
			m_camera->name = "camera";
			m_camera->viewMatrix = vsg::LookAt::create();
			m_trackball = vsg::Trackball::create(m_camera, nullptr);
			m_trackball->addWindow(*m_vsgWind);

			m_vsgWind->getViewer().addEventHandler(m_trackball);

			m_intersectionHandler = IntersectionHandler::create(m_sys->getModelNode(), m_camera);
			m_vsgWind->getViewer().addEventHandler(m_intersectionHandler);

			Rebuild(false);

			m_renderGraph = vsg::RenderGraph::create(*m_vsgWind);
			vsg::ref_ptr<vsg::Group> grptr(m_sys->getRootNode());
			m_renderGraph->addChild(vsg::View::create(m_camera, grptr));
			auto commandGraph = vsg::CommandGraph::create(*m_vsgWind);
			commandGraph->addChild(m_renderGraph);

			m_vsgWind->getViewer().addRecordAndSubmitTaskAndPresentation({ commandGraph });
			m_vsgWind->getViewer().compile();
		}

		vsgQt::Window * GetVsgWindow()
		{
			return m_vsgWind;
		}

		vsgQt::Window const * GetVsgWindow() const
		{
			return m_vsgWind;
		}

		QColor GetClearColor() const
		{
			return m_vsgWind->clearQColor();
		}

		void SetClearColor(QColor color)
		{
			m_vsgWind->setClearQColor(color);
			UpdateClearColor();
		}

		void RebuildModel(std::function<void (vsg::Switch*)> const & f, bool compile = true)
		{
			m_intersectionHandler->clear();
			m_vsgWind->getViewer().deviceWaitIdle();
			m_sys->clearModelNode();

			f(m_sys->getModelNode());

			// compute the bounds of the scene graph to help position m_camera
			vsg::ComputeBounds computeBounds;
			m_sys->getModelNode()->accept(computeBounds);
			vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
			m_sys->recreateAxisForBox(Box2Rect3D(computeBounds.bounds), true, false);
			double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;
			double nearFarRatio = 0.001;

			uint32_t width = m_vsgWind->width();
			uint32_t height = m_vsgWind->height();

			auto lookAt = vsg::LookAt(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0),
				centre, vsg::dvec3(0.0, 0.0, 1.0));

			*(m_camera->viewMatrix.cast<vsg::LookAt>()) = lookAt;
			m_camera->projectionMatrix = vsg::Perspective::create(
				30.0,
				static_cast<double>(width) /
				static_cast<double>(height),
				nearFarRatio * radius, radius / nearFarRatio);
			m_camera->viewportState = vsg::ViewportState::create(VkExtent2D{ width, height });

			if (compile)
				Compile();
		}

		void Rebuild(bool compile = true)
		{
			RebuildModel([](vsg::Switch * model)
			{
				static int state = 0;
				switch (state)
				{
				case 0:
					model->addChild(true, createGizmo());
					break;
				case 1:
					model->addChild(true, createScene0());
					break;
				case 2:
					model->addChild(true, createScene1());
					break;
				}
				++state;
				if (state == 3)
					state = 0;
			}, compile);
		}

		void AddHudCamera()
		{
			CreateHudCamera();
			if (!m_hudView)
				return;
			m_renderGraph->addChild(m_hudView);
			Compile();
		}

		void RemoveHudCamera()
		{
			if (!m_hudView)
				return;
			std::erase_if(m_renderGraph->children, [this](auto const & child)
			{
				return child == m_hudView;
			});
			Compile();
		}


	private:
		void UpdateClearColor()
		{
			// set up the clearValues based on the RenderPass's attachments.
			m_renderGraph->setClearValues(m_vsgWind->clearColor(), VkClearDepthStencilValue{ 0.0f, 0 });
		}

		void Compile()
		{
			auto result = m_vsgWind->getViewer().compile();
			vsg::updateViewer(m_vsgWind->getViewer(), result);
		}

		void CreateHudCamera()
		{
			if (m_hudCamera)
				return;

			auto hudTextGr = m_sys->createHudTextGroup();
			if (!hudTextGr)
				return;

			m_hudTextGr = hudTextGr;

			m_hudCamera = vsg::Camera::create(m_camera->projectionMatrix,
				vsg::LookAt::create(vsg::dvec3(0.0, 0.0, 8.8), // Eye position
					vsg::dvec3(0.0, 0.0, 0.0), // Center
					vsg::dvec3(0.0, 1.0, 0.0)), // Up vector
				m_camera->viewportState);
			m_hudCamera->name = "hudCamera";

			m_hudView = vsg::View::create(m_hudCamera, m_hudTextGr);
		}
	};

	SolidModelWindow::SolidModelWindow(QWidget * parent)
		: base_t(parent)
		, m_impl(new Impl(nullptr))
	{
		auto vsgWnd = m_impl->GetVsgWindow();
		auto & windowTraits = vsgWnd->getTraits();

		windowTraits.windowTitle = "vsgQt viewer";

		setCentralWidget(vsgWnd);
		setGeometry(windowTraits.x, windowTraits.y, windowTraits.width, windowTraits.height);

		QToolBar * toolBar = addToolBar(tr("MainToolbar"));
		toolBar->addAction("Rebuild", [this]() {OnRebuild(); });
		toolBar->addAction("LoadModel", [this]() {OnLoadModel(); });
		auto * hudCameraAct = toolBar->addAction("HudCamera");
		hudCameraAct->setCheckable(true);
		connect(hudCameraAct, &QAction::triggered, [this, hudCameraAct]() {OnSetHudCamera(hudCameraAct); });
		toolBar->addAction("SetClearColor", [this]() {OnSetClearColor(); });
		toolBar->addAction("BuildBox", [this]()
		{
			OnBuild("");
		});
	}

	SolidModelWindow::~SolidModelWindow() = default;

	void SolidModelWindow::OnRebuild()
	{
		m_impl->Rebuild();
		update();
	}

	void SolidModelWindow::OnLoadModel()
	{
		auto filePath = QFileDialog::getOpenFileName(this, tr("Open Image"));
		if (filePath.isEmpty())
			return;

		auto options = vsg::Options::create();
		auto reader = vsgXchange::assimp::create();
		auto object = reader->read(filePath.toStdWString(), options).cast<vsg::Node>();
		if (!object)
			return;
		
		m_impl->RebuildModel([&object](vsg::Switch * model)
		{
			model->addChild(true, object);
		});
		update();
	}

	void SolidModelWindow::OnSetHudCamera(QAction* act)
	{
		if (act->isChecked())
		{
			m_impl->AddHudCamera();
		}
		else
		{
			m_impl->RemoveHudCamera();
		}
		update();
		//https://github.com/vsg-dev/VulkanSceneGraph/discussions/1234
		//github.com/projectchrono/chrono.git
	}

	void SolidModelWindow::OnSetClearColor()
	{
		auto color = QColorDialog::getColor(m_impl->GetClearColor(), this);
		if (color == QColor())
			return;
		m_impl->SetClearColor(color);
		update();
	}

	void SolidModelWindow::OnBuild(std::string_view cmd)
	{
		m_impl->RebuildModel([cmd](vsg::Switch* model)
			{
				model->addChild(true, CreateBox());
			});
		update();
	}
}
