/// @file	3DSystemBase.cpp
///
/// @see	3DSystemBase.h
///
/// @author	RDN © Intern :)
///
/// @date    20.12.2025 13:20
///

#include "stdafx.h"
#include "3DSystemBase.h"
#include "Platform2VSG.h"\

#include <vsg/maths/transform.h>
#include <vsg/nodes/MatrixTransform.h>
#include <vsg/nodes/Geometry.h>
#include <vsg/nodes/Switch.h>
#include <vsg/utils/Builder.h>

#include <vsg/commands/SetLineWidth.h>
#include <vsg/commands/Commands.h>
#include <vsg/nodes/Group.h>
#include <vsg/lighting/DirectionalLight.h>
#include <vsg/lighting/Light.h>
#include <vsg/text/StandardLayout.h>
#include <vsg/text/Text.h>

#include <numbers>
#include <utils/ComputeBounds.h>

namespace
{
	template<typename TVal>
	TVal DegreesToRadians(TVal degree)
	{
		if constexpr (std::is_same_v<float, TVal>)
			return std::numbers::pi_v<float> * degree / 180.f;
		else
			return std::numbers::pi_v<double> * degree / 180.;
	}

	vsg::vec4 ToVec4(std::array<float, 4> const & val)
	{
		return { val[0], val[1], val[2], val[3] };
	}

	std::array<float, 4> ToArray(vsg::vec4 const & val)
	{
		return { val.x, val.y, val.z, val.a };
	}

	
}
namespace model3d
{
	vsg::ref_ptr<vsg::Node> CreateAxisNode(AxisParams const & params, CPoint3D const & axisOrg)
	{
		if (!params.areAxisVisible)
			return nullptr;

		auto axis = vsg::Commands::create();
		axis->setValue("name", "createCoordinateAxes::geode");

		auto lineWidth = vsg::SetLineWidth::create();
		lineWidth->lineWidth = static_cast<float>(params.lineAxisWidth);
		axis->addChild(lineWidth);

		float alpha = 0.5f;

		vsg::ref_ptr<vsg::MatrixTransform> transform = vsg::MatrixTransform::create();
		transform->matrix = vsg::translate(PointToVec(axisOrg));
		/*if (params.isAxisScaling)
		{
			vsg::ref_ptr<vsg::AutoTransform> at = new vsg::AutoTransform;
			at->setAutoScaleToScreen(true);
			at->addChild(geode);
			transform->addChild(at);
		}
		else*/
		{
			transform->addChild(axis);
		}

		
		auto const org = vsg::vec3(0.0, 0.0, 0.0);

		auto FormCoordinateAxis = [&](bool isPositive)
		{
			// знак оси
			int axisSign = isPositive ? 1 : -1;

			// настройка стрелок для положительных осей
			constexpr float arrowAngleDegr = 30.0;
			constexpr float arrowLengthPercetange = 0.09;
			const float arrowLength = arrowLengthPercetange * params.lineAxisLength;
			const float arrowProjectionLength = cos(DegreesToRadians(arrowAngleDegr)) * arrowLength;
			const float endOfArrowCoordX = params.lineAxisLength - arrowProjectionLength;
			const float endOfArrowCoordY = sin(DegreesToRadians(arrowAngleDegr)) * arrowLength;

			uint32_t count = 2 + (isPositive ? 4 : 0);
			count *= 3;
			vsg::ref_ptr<vsg::vec3Array> vertices(new vsg::vec3Array(count));

			uint32_t pos = 0;
			// создаем X ось
			vertices->at(pos++) = org;
			vertices->at(pos++) = vsg::vec3(axisSign * params.lineAxisLength, 0.0, 0.0) + org;

			// если положительная ось - достроим стрелки
			if (isPositive)
			{
				vertices->at(pos++) = vsg::vec3(endOfArrowCoordX, -endOfArrowCoordY, 0.0) + org;
				vertices->at(pos++) = vsg::vec3(params.lineAxisLength, 0.0, 0.0) + org;
				vertices->at(pos++) = vsg::vec3(endOfArrowCoordX, endOfArrowCoordY, 0.0) + org;
				vertices->at(pos++) = vsg::vec3(params.lineAxisLength, 0.0, 0.0) + org;
			}

			// создаем Y ось
			vertices->at(pos++) = org;
			vertices->at(pos++) = vsg::vec3(0.0, axisSign * -params.lineAxisLength, 0.0) + org;

			// если положительная ось - достроим стрелки
			if (isPositive)
			{
				vertices->at(pos++) = vsg::vec3(-endOfArrowCoordY, -endOfArrowCoordX, 0.0) + org;
				vertices->at(pos++) = vsg::vec3(0.0, -params.lineAxisLength, 0.0) + org;
				vertices->at(pos++) = vsg::vec3(endOfArrowCoordY, -endOfArrowCoordX, 0.0) + org;
				vertices->at(pos++) = vsg::vec3(0.0, -params.lineAxisLength, 0.0) + org;
			}

			// создаем Z ось
			vertices->at(pos++) = org;
			vertices->at(pos++) = vsg::vec3(0.0, 0.0, axisSign * params.lineAxisLength) + org;

			if (isPositive)
			{
				vertices->at(pos++) = vsg::vec3(endOfArrowCoordY, 0.0, endOfArrowCoordX) + org;
				vertices->at(pos++) = vsg::vec3(0.0, 0.0, params.lineAxisLength) + org;
				vertices->at(pos++) = vsg::vec3(-endOfArrowCoordY, 0.0, endOfArrowCoordX) + org;
				vertices->at(pos++) = vsg::vec3(0.0, 0.0, params.lineAxisLength) + org;
			}

			const int32_t verticesPerAxis = [isPositive]()
			{
				return isPositive ? 6 : 2;
			}();

			vsg::ref_ptr<vsg::vec4Array> colors(new vsg::vec4Array(verticesPerAxis * 3));
			

			std::vector<vsg::vec4> axisColors = {
				vsg::vec4(1.f, 0.f, 0.f, alpha),
				vsg::vec4(0.f, 0.8f, 0.f, alpha),
				vsg::vec4(0.f, 0.f, 1.f, alpha)
			};

			// привязываем цвета к осям. В каждой оси (всего 3 оси) по verticesPerAxis вершин
			pos = 0;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < verticesPerAxis; ++j)
					colors->at(pos++) = axisColors[i];
			}

			auto attributteArray = vsg::DataList{vertices};
			attributteArray.push_back(colors);

			vsg::ref_ptr<vsg::Geometry> geom = vsg::Geometry::create();
			geom->assignArrays(attributteArray);

			axis->addChild(geom);

			//geom->addPrimitiveSet(new vsg::DrawArrays(vsg::PrimitiveSet::LINES, 0, verticesPerAxis * 3));

			//if (!isPositive)
			//{
			//	// сделать отрицательные оси пунктиром
			//	vsg::ref_ptr<vsg::StateSet> stateset_negative = geom->getOrCreateStateSet();
			//	stateset_negative->setAttributeAndModes(new vsg::LineStipple(2, 0xAAAA), vsg::StateAttribute::ON);
			//}
		};

		// положительные оси
		{
			FormCoordinateAxis(true);
		}

		// отрицательные оси
		if (!params.isHideNegativeAxis)
		{
			FormCoordinateAxis(false);
		}

		return transform;
	}
}

namespace model3d
{
	void SetLightDir(vsg::DirectionalLight & light, double az)
	{
		auto & lightPosition = light.direction;

		double dirXY = std::cos(az) / std::sqrt(2.0);
		double dirZ = std::sin(az);
		lightPosition.x = static_cast<float>(dirXY);
		lightPosition.y = static_cast<float>(dirXY);
		lightPosition.z = static_cast<float>(dirZ);
	}

	Base3DSystem::Base3DSystem(std::unique_ptr<INodeNameConvertor> nodeConv)
		: m_polygonModeForSelect(PolygonMode::FILL)
		//, m_polygonModeForSelect(new vsg::PolygonMode(vsg::PolygonMode::FRONT_AND_BACK, vsg::PolygonMode::FILL))
		, m_nodeConv(std::move(nodeConv))
		, m_frontCullFace(CullMode::FRONT)
		, m_showBounds(true)
	{
		m_rootNode = new vsg::Group();

		m_verticalScaleNode = new vsg::MatrixTransform();
		m_rootNode->addChild(m_verticalScaleNode);
		m_swModel = new vsg::Switch();

		/*
		 m_clipNode = new osg::ClipNode();
		m_clipNode->setName("m_clipNode");
		m_clipNode->addChild(m_swModel);
		m_verticalScaleNode->addChild(m_clipNode);m_clipNode = new osg::ClipNode();
		m_clipNode->setName("m_clipNode");
		m_clipNode->addChild(m_swModel);
		m_verticalScaleNode->addChild(m_clipNode); 
		 */
		m_verticalScaleNode->addChild(m_swModel);

		m_swCoordAxes = vsg::Group::create();

		// creating hud camera for axis
		/*vsg::ref_ptr<vsg::Camera> axisCamera = new vsg::Camera;
		axisCamera->setReferenceFrame(vsg::Transform::RELATIVE_RF);
		axisCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
		axisCamera->setRenderOrder(vsg::Camera::POST_RENDER);
		axisCamera->setAllowEventFocus(false);
		axisCamera->getOrCreateStateSet()->setMode(GL_LIGHTING, vsg::StateAttribute::OFF);
		axisCamera->addChild(m_swCoordAxes.get());*/

		m_rootNode->addChild(m_swCoordAxes);

		m_secondLightSource = vsg::DirectionalLight::create();
		SetLightDir(*m_secondLightSource, vsg::PI * 0.25);

		m_mainLightSource = vsg::createHeadlight();
		m_rootNode->addChild(m_mainLightSource);
	}

	Base3DSystem::~Base3DSystem()
	{
		clearModelNode();
	}

	void Base3DSystem::setVerticalScale(double scale)
	{
		auto matrix = vsg::scale(1.0, 1.0, scale);
		auto newAxisOrg = PointToVec(m_axisOrg) * vsg::inverse(m_verticalScaleNode->matrix) * matrix;
		m_verticalScaleNode->matrix= matrix;
		recreateAxis(VecToPoint(newAxisOrg));
	}

	void Base3DSystem::setNodeNameConvertor(std::shared_ptr<INodeNameConvertor> nameConvertor)
	{
		m_nodeConv = nameConvertor;
	}

	bool Base3DSystem::isValidNodeName(std::string const & nodeName) const
	{
		return m_nodeConv->isValidNodeName(nodeName);
	}

	std::vector<size_t> Base3DSystem::nodeName2Key(std::string const & nodeName) const
	{
		return m_nodeConv->nodeName2Key(nodeName);
	}

	std::wstring Base3DSystem::entityInfo(std::vector<size_t> const & /*entityId*/) const
	{
		return std::wstring();
	}

	void Base3DSystem::connectOnObjectUnderCursor(std::function<void(std::vector<size_t> const &)> /*func*/)
	{
	}

	bool Base3DSystem::isPresentEntityInModel(std::vector<size_t> const & /*objectKey*/) const
	{
		return false;
	}

	void Base3DSystem::resetModel()
	{
	}

	void Base3DSystem::setEnableLight(bool enable)
	{
		auto it = std::find(m_rootNode->children.begin(), m_rootNode->children.end(), m_mainLightSource);
		bool present = it != m_rootNode->children.end();
		if (enable != present)
		{
			if (enable)
				m_rootNode->addChild(m_mainLightSource);
			else
				m_rootNode->children.erase(it);
		}
	}

	void Base3DSystem::setShowBoundsMode(bool showBounds)
	{
		m_showBounds = showBounds;
	}

	bool Base3DSystem::getShowBoundsMode() const
	{
		return m_showBounds;
	}

	std::optional<float> Base3DSystem::getLODBound() const
	{
		return std::nullopt;
	}

	void Base3DSystem::setLODBound(std::optional<float> const &)
	{
	}

	bool Base3DSystem::useTransparentModeForSelect() const
	{
		return true;
	}

	void Base3DSystem::setTransparentModeForSelect(bool /*set*/)
	{
	}

	void Base3DSystem::setHighlightSelected(bool /*flag*/)
	{
	}

	void Base3DSystem::setSelectColor(SelectType type, COLORREF color)
	{
	}

	void Base3DSystem::setPolygonMode(std::optional<PolygonMode> const & mode)
	{
		/*if (mode == PolygonMode::FILL)
		{
			if (m_polygonMode)
				m_swModel->getOrCreateStateSet()->removeAttribute(m_polygonMode.release());
		}
		else
		{
			if (!m_polygonMode)
				m_polygonMode = new vsg::PolygonMode();
			m_polygonMode->setMode(vsg::PolygonMode::FRONT_AND_BACK, mode);
			m_swModel->getOrCreateStateSet()->setAttribute(m_polygonMode, ::vsg::StateAttribute::ON);
		}*/
	}

	void Base3DSystem::setFrontCullFaceEnabled(bool enable)
	{
		/*if (enable)
			m_swModel->getOrCreateStateSet()->setAttributeAndModes(m_frontCullFace,
				vsg::StateAttribute::ON | vsg::StateAttribute::OVERRIDE);
		else
			m_swModel->getOrCreateStateSet()->removeAttribute(m_frontCullFace);*/
	}

	bool Base3DSystem::isAuxNode(vsg::Node const & /*node*/) const
	{
		return false;
	}

	vsg::Camera * Base3DSystem::getHUDCamera() const
	{
		return nullptr;
	}

	vsg::Group * Base3DSystem::getInteractiveParent() const
	{
		return nullptr;
	}

	void Base3DSystem::updateLightState(CPoint3D const& /*org*/, double /*rad*/)
	{
	}

	vsg::ref_ptr<vsg::Text> Base3DSystem::createText(std::array<float, 4> const& color, std::string const& text, CPoint3D const& position, float characterSizeAspectRatio)
	{
		vsg::ref_ptr<vsg::Text> resText = vsg::Text::create();
		//resText->setFont("fonts/calibri.ttf");

		auto layout = vsg::StandardLayout::create();
		layout->horizontalAlignment = vsg::StandardLayout::RIGHT_ALIGNMENT;
		layout->verticalAlignment = vsg::StandardLayout::TOP_ALIGNMENT;
		layout->color = vsg::vec4(color[0], color[1], color[2], color[3]);
		layout->position = PointToVec(position);

		resText->text = vsg::stringValue::create(text);
		//resText->setAxisAlignment(osgText::Text::SCREEN);
		//resText->setCharacterSize(0.5f * characterSizeAspectRatio);
		//resText->setEnableDepthWrites(false);

		return resText;
	}

	vsg::ref_ptr<::vsg::Geometry> Base3DSystem::createGrid(double gridR)
	{
		/*gridR = std::max(gridR, 1.0);

		vsg::ref_ptr<vsg::Geometry> geom = new vsg::Geometry();
		vsg::ref_ptr<vsg::Vec3dArray> vertices = new vsg::Vec3dArray();

		vsg::Vec3d leftBottom(-gridR, -gridR, 0.0);
		vsg::Vec3d leftTop(-gridR, gridR, 0.0);

		vsg::Vec3d rightBottom(gridR, -gridR, 0.0);

		for (double pos = .0; pos <= 2 * gridR; pos += 1.0)
		{
			vertices->push_back(vsg::Vec3d(pos, 0.0, 0.0) + leftBottom);
			vertices->push_back(vsg::Vec3d(pos, 0.0, 0.0) + leftTop);

			vertices->push_back(vsg::Vec3d(0.0, pos, 0.0) + leftBottom);
			vertices->push_back(vsg::Vec3d(0.0, pos, 0.0) + rightBottom);
		}

		geom->setVertexArray(vertices);
		geom->addPrimitiveSet(new vsg::DrawArrays(vsg::PrimitiveSet::LINES, 0, static_cast<GLsizei>(vertices->size())));

		vsg::ref_ptr<vsg::Vec4Array> colors = new vsg::Vec4Array();
		colors->push_back(vsg::Vec4f(.7f, .7f, .7f, 1.0));
		colors->setBinding(vsg::Array::BIND_OVERALL);
		geom->setColorArray(colors);

		geom->getOrCreateStateSet()->setAttributeAndModes(new vsg::LineStipple(2, 0xAAAA), vsg::StateAttribute::ON);

		return geom;*/
		return nullptr;
	}

	void Base3DSystem::recreateAxisForPlatform(CRect3D const & bb)
	{
		ClearAllChildNodes(*m_swCoordAxes);

		vsg::ref_ptr<vsg::Group> geode = vsg::Group::create();
		float alpha = 0.5f;
		m_swCoordAxes->addChild(geode);

		m_axisOrg = CPoint3D(0.0);

		double xLength = (bb.max.xy.x - bb.min.xy.x) * 0.5;
		double yLength = (bb.max.xy.y - bb.min.xy.y) * 0.5;
		double zLength = bb.max.z - bb.min.z;

		double r = std::max(xLength, yLength);
		r += r * 0.2;

		// positive axes
		vsg::ref_ptr<vsg::Geometry> geom = vsg::Geometry::create(6);
		geode->addChild(geom);

		vsg::ref_ptr<vsg::dvec3Array> vertices = vsg::dvec3Array::create();
		int32_t pos = 0;

		auto axisOrg = PointToVec(m_axisOrg);
		vertices->at(pos++) = axisOrg;
		auto xEnd = vsg::dvec3(r, .0, 0.0);
		vertices->at(pos++) = xEnd;

		vertices->at(pos++) = axisOrg;
		auto yEnd = vsg::dvec3(.0, -r, 0.0);
		vertices->at(pos++) = yEnd;

		vertices->at(pos++) = axisOrg;
		auto zEnd = vsg::dvec3(0.0, 0.0, r);
		vertices->at(pos++) = zEnd;

		vsg::ref_ptr<vsg::vec4Array> colors = vsg::vec4Array::create(6);

		pos = 0;

		vsg::vec4 xColor(1.f, 0.f, 0.f, alpha);
		colors->at(pos++) = xColor;
		colors->at(pos++) = xColor;

		vsg::vec4 yColor(0.f, 0.8f, 0.f, alpha);
		colors->at(pos++) = yColor;
		colors->at(pos++) = yColor;

		vsg::vec4 zColor(0.f, 0.f, 1.f, alpha);
		colors->at(pos++) = zColor;
		colors->at(pos++) = zColor;

		auto attributteArray = vsg::DataList{ vertices };
		attributteArray.push_back(colors);

		geom->assignArrays(attributteArray);
		//geom->addPrimitiveSet(new vsg::DrawArrays(vsg::PrimitiveSet::LINES, 0, 6));

		geode->addChild(geom);

		float aspectRation = static_cast<float>(r) * 0.2f;

		geode->addChild(createText(ToArray(xColor), "x", VecToPoint(xEnd), aspectRation));
		geode->addChild(createText(ToArray(yColor), "y", VecToPoint(yEnd), aspectRation));
		geode->addChild(createText(ToArray(zColor), "z", VecToPoint(zEnd), aspectRation));

		// grid
		//geode->addDrawable(createGrid(std::max(xLength, yLength) * 1.2));
	}

	CPoint3D const & Base3DSystem::getAxisOrg() const
	{
		return m_axisOrg;
	}

	void Base3DSystem::recreateAxis(CPoint3D const & axisOrg)
	{
		m_axisOrg = axisOrg;
		ClearAllChildNodes(*m_swCoordAxes);

		if (auto newNode = CreateAxisNode(m_axisParams, axisOrg))
			m_swCoordAxes->addChild(newNode);
	}

	void Base3DSystem::clearUnderCursor()
	{
	}

	void Base3DSystem::clearSelection()
	{
	}

	bool Base3DSystem::isObjectUnderCursor(std::vector<size_t> const & /*objectKey*/) const
	{
		return false;
	}

	vsg::Group * Base3DSystem::findNodeById(std::vector<size_t> const & /*objKey*/) const
	{
		return nullptr;
	}

	std::set<::vsg::Group *> Base3DSystem::selectedObjects() const
	{
		return {};
	}

	/*vsg::ClipPlane * Base3DSystem::addClipPlane(double a, double b, double c, double d)
	{
		int num = m_clipNode->getNumClipPlanes();
		vsg::ref_ptr<vsg::ClipPlane> clipPlane = new vsg::ClipPlane();
		clipPlane->setClipPlane(a, b, c, d);
		clipPlane->setClipPlaneNum(num);
		clipPlane->setDataVariance(Object::DYNAMIC);
		if (m_clipNode->addClipPlane(clipPlane.release()))
			return m_clipNode->getClipPlane(num);
		return nullptr;
	}

	void Base3DSystem::removeClipPlanes()
	{
		while (m_clipNode->getNumClipPlanes())
			m_clipNode->removeClipPlane(0u);
	}

	void Base3DSystem::removeClipPlane(vsg::ClipPlane * clipPlane)
	{
		VERIFY(m_clipNode->removeClipPlane(clipPlane));
	}*/

	CRect3D Base3DSystem::calcWorldBoundingBox() const
	{
		vsg::ComputeBounds cbv;
		m_swModel->accept(cbv);
		auto localBB = cbv.bounds;
		vsg::Matrix localToWorld = vsg::computeLocalToWorld(m_swModel->getParent(0)->getParentalNodePaths()[0]);

		vsg::BoundingBox bb;
		for (unsigned int i = 0; i < 8; ++i)
			bb.expandBy(localBB.corner(i) * localToWorld);
		return bb;
	}

	::vsg::Group * Base3DSystem::objUnderCursor() const
	{
		return nullptr;
	}

	CSphere Base3DSystem::showNodes(size_t /*prjKey*/, std::set<size_t> const & /*nodes*/)
	{
		return {};
	}

	void Base3DSystem::showSkyBox(bool /*show*/, void const * /*cubemap*/)
	{
	}

	void Base3DSystem::updateVisibilityForNodes(size_t /*prjKey*/, std::set<size_t> const & /*unvisibleNodes*/,
		std::set<size_t> const & /*restoreVisibility*/)
	{
		//ASSERT(false);
	}

	void Base3DSystem::deleteNode(std::vector<size_t> const & /*objectKey*/, vsg::Group * /*fromParent = nullptr*/)
	{
		//ASSERT(false);
	}

	void Base3DSystem::setSelectedObj(vsg::Group * /*node*/, bool /*selected*/, SelectType /*selType*/)
	{
		//ASSERT(false);
	}

	void Base3DSystem::setObjectUnderCursor(std::vector<size_t> const & /*objectKey*/, bool /*selected*/, vsg::Group * /* grp = nullptr*/)
	{
	}

	vsg::Group * Base3DSystem::getRootNode()
	{
		return m_rootNode;
	}

	vsg::MatrixTransform const & Base3DSystem::getVerticalScaleNode() const
	{
		return *m_verticalScaleNode;
	}

	vsg::Switch * Base3DSystem::getModelNode()
	{
		return m_swModel;
	}

	bool Base3DSystem::isEmptyModel() const
	{
		// 1 - ый ребенок - m_secondLightSource
		return m_swModel->children.size() < 2;
	}

	void Base3DSystem::clearAllNodes()
	{
		clearModelNode();
		ClearAllChildNodes(*m_swCoordAxes);
	}

	void Base3DSystem::clearModelNode()
	{
		auto dontRemoveLight = [this](auto const * node)
		{
			return node != m_secondLightSource;
		};
		ClearAllChildNodes(*m_swModel, dontRemoveLight);

		if (m_swModel->children.empty()) // ClearAllChildNodes достает m_secondLightSource из m_swModel
			m_swModel->addChild(true, m_secondLightSource);

		for (auto & pairData : m_unvisibleModel)
			ClearAllChildNodes(*pairData.second);
		m_unvisibleModel.clear();
		//removeClipPlanes();
	}

	vsg::Group * Base3DSystem::getCoordinateAxesNode()
	{
		return m_swCoordAxes.get();
	}

	void Base3DSystem::setAxisParams(AxisParams const & axisParams)
	{
		m_axisParams = axisParams;
	}

	void Base3DSystem::setRootNode(vsg::Group * root)
	{
		m_rootNode = root;
	}

	/*model3d::ArbitraryPropTypes Base3DSystem::getHideTypes() const
	{
		return {};
	}

	bool Base3DSystem::changeHideTypes(ArbitraryPropTypes)
	{
		return false;
	}*/

	namespace
	{
		template<class TNode>
		void TClearAllChildNodes(TNode & group, std::function<bool(vsg::Node*)> const& needRemoveNode /*= nullptr*/)
		{
			for (auto it = group.children.begin(); it != group.children.end();)
			{
				if (needRemoveNode && !needRemoveNode(*it))
				{
					++it;
					continue;
				}

				auto clearGroup = [&](auto & node)
				{
						TClearAllChildNodes(node, needRemoveNode);
						if (node.children.empty())
						{
							it = group.children.erase(it);
						}
						else
							++it;
				};

				if (auto childAsGroup = it->cast<vsg::Group>())
				{
					clearGroup(*childAsGroup);
				}
				else if (auto childAsSwitch = it->cast<vsg::Switch>())
				{
					clearGroup(*childAsSwitch);
				}
				else
					it = group.children.erase(it);
			}
		}
	}

	void Base3DSystem::ClearAllChildNodes(vsg::Node & group, std::function<bool(vsg::Node *)> const & needRemoveNode /*= nullptr*/)
	{
		if (auto childAsGroup = group.cast<vsg::Group>())
		{
			TClearAllChildNodes(*childAsGroup, needRemoveNode);
		}
		else if (auto childAsSwitch = group.cast<vsg::Switch>())
		{
			TClearAllChildNodes(*childAsSwitch, needRemoveNode);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	Base3DSystem * MakeBase3DSystem(std::unique_ptr<INodeNameConvertor> && nodeConv)
	{
		return new Base3DSystem(std::move(nodeConv));
	}
}
