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

#include <maths/transform.h>
#include <nodes/MatrixTransform.h>
#include <vsg/commands/SetLineWidth.h>
#include <vsg/commands/Commands.h>
#include <vsg/nodes/Group.h>

#include "Platform2VSG.h"

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

		vsg::dmat4 resMatrix = vsg::translate(PointToVec(axisOrg));
		vsg::ref_ptr<vsg::MatrixTransform> transform = vsg::MatrixTransform::create();

		if (params.isAxisScaling)
		{
			vsg::ref_ptr<vsg::AutoTransform> at = new vsg::AutoTransform;
			at->setAutoScaleToScreen(true);
			at->addChild(geode);
			transform->addChild(at);
		}
		else
		{
			transform->addChild(geode);
		}

		transform->setMatrix(resMatrix);
		transform->dirtyBound();

		vsg::Vec3f const org = vsg::Vec3f(0.0, 0.0, 0.0);

		auto FormCoordinateAxis = [&](bool isPositive)
		{
			// знак оси
			int axisSign = isPositive ? 1 : -1;

			// настройка стрелок для положительных осей
			constexpr double arrowAngleDegr = 30.0;
			constexpr double arrowLengthPercetange = 0.09;
			const double arrowLength = arrowLengthPercetange * params.lineAxisLength;
			const double arrowProjectionLength = cos(math::DegreesToRadians(arrowAngleDegr)) * arrowLength;
			const double endOfArrowCoordX = params.lineAxisLength - arrowProjectionLength;
			const double endOfArrowCoordY = sin(math::DegreesToRadians(arrowAngleDegr)) * arrowLength;

			vsg::ref_ptr<vsg::Geometry> geom = new vsg::Geometry();
			vsg::ref_ptr<vsg::Vec3Array> vertices = new vsg::Vec3Array();

			// создаем X ось
			vertices->push_back(org);
			vertices->push_back(vsg::Vec3f(axisSign * params.lineAxisLength, 0.0, 0.0) + org);

			// если положительная ось - достроим стрелки
			if (isPositive)
			{
				vertices->push_back(vsg::Vec3f(endOfArrowCoordX, -endOfArrowCoordY, 0.0) + org);
				vertices->push_back(vsg::Vec3f(params.lineAxisLength, 0.0, 0.0) + org);
				vertices->push_back(vsg::Vec3f(endOfArrowCoordX, endOfArrowCoordY, 0.0) + org);
				vertices->push_back(vsg::Vec3f(params.lineAxisLength, 0.0, 0.0) + org);
			}

			// создаем Y ось
			vertices->push_back(org);
			vertices->push_back(vsg::Vec3f(0.0, axisSign * -params.lineAxisLength, 0.0) + org);

			// если положительная ось - достроим стрелки
			if (isPositive)
			{
				vertices->push_back(vsg::Vec3f(-endOfArrowCoordY, -endOfArrowCoordX, 0.0) + org);
				vertices->push_back(vsg::Vec3f(0.0, -params.lineAxisLength, 0.0) + org);
				vertices->push_back(vsg::Vec3f(endOfArrowCoordY, -endOfArrowCoordX, 0.0) + org);
				vertices->push_back(vsg::Vec3f(0.0, -params.lineAxisLength, 0.0) + org);
			}

			// создаем Z ось
			vertices->push_back(org);
			vertices->push_back(vsg::Vec3f(0.0, 0.0, axisSign * params.lineAxisLength) + org);

			if (isPositive)
			{
				vertices->push_back(vsg::Vec3f(endOfArrowCoordY, 0.0, endOfArrowCoordX) + org);
				vertices->push_back(vsg::Vec3f(0.0, 0.0, params.lineAxisLength) + org);
				vertices->push_back(vsg::Vec3f(-endOfArrowCoordY, 0.0, endOfArrowCoordX) + org);
				vertices->push_back(vsg::Vec3f(0.0, 0.0, params.lineAxisLength) + org);
			}

			vsg::ref_ptr<vsg::Vec4Array> colors = new vsg::Vec4Array();

			const int verticesPerAxis = [isPositive]()
			{
				return isPositive ? 6 : 2;
			}();

			std::vector<vsg::Vec4f> axisColors = {
				vsg::Vec4f(1.f, 0.f, 0.f, alpha),
				vsg::Vec4f(0.f, 0.8f, 0.f, alpha),
				vsg::Vec4f(0.f, 0.f, 1.f, alpha)
			};

			// привязываем цвета к осям. В каждой оси (всего 3 оси) по verticesPerAxis вершин
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < verticesPerAxis; ++j)
					colors->push_back(axisColors[i]);
			}

			colors->setBinding(vsg::Array::BIND_PER_VERTEX);
			geode->addDrawable(geom);
			geom->setColorArray(colors);
			geom->setVertexArray(vertices);
			geom->addPrimitiveSet(new vsg::DrawArrays(vsg::PrimitiveSet::LINES, 0, verticesPerAxis * 3));

			if (!isPositive)
			{
				// сделать отрицательные оси пунктиром
				vsg::ref_ptr<vsg::StateSet> stateset_negative = geom->getOrCreateStateSet();
				stateset_negative->setAttributeAndModes(new vsg::LineStipple(2, 0xAAAA), vsg::StateAttribute::ON);
			}
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
	namespace
	{
		struct OsgNodeNameConvertorForIFC : public IOsgNodeNameConvertor
		{
			virtual bool isValidNodeName(std::string const & nodeName) const
			{
				if (nodeName.at(0) != '#')
					return false;

				if (nodeName.compare(0, 9, "intersect") == 0)
					return false;

				return true;
			}

			virtual std::vector<size_t> nodeName2Key(std::string const & nodeName) const
			{
				std::string nodeNameId = nodeName.substr(1);
				size_t lastIndex = nodeNameId.find_first_not_of("0123456789");
				std::string idStr = nodeNameId.substr(0, lastIndex);
				return { static_cast<size_t>(std::stoi(idStr)) };
			}
		};
	}

	Base3DSystem::Base3DSystem(std::unique_ptr<IOsgNodeNameConvertor> nodeConv)
		: m_nodeConv(std::move(nodeConv))
		, m_polygonModeForSelect(new vsg::PolygonMode(vsg::PolygonMode::FRONT_AND_BACK, vsg::PolygonMode::FILL))
		, m_frontCullFace(new vsg::CullFace(vsg::CullFace::FRONT))
		, m_showBounds(true)
	{
		if (!m_nodeConv)
			m_nodeConv = std::make_unique<OsgNodeNameConvertorForIFC>();

		m_rootnode = new vsg::Group();
		m_rootnode->setName("m_rootnode");

		m_verticalScaleNode = new vsg::MatrixTransform();
		m_verticalScaleNode->setName("m_verticalScaleNode");
		m_rootnode->addChild(m_verticalScaleNode);
		m_swModel = new vsg::Switch();
		m_swModel->setName("m_sw_model");

		m_clipNode = new vsg::ClipNode();
		m_clipNode->setName("m_clipNode");
		m_clipNode->addChild(m_swModel);
		m_verticalScaleNode->addChild(m_clipNode);

		m_swCoordAxes = new vsg::Switch();
		m_swCoordAxes->setName("swCoordAxes");

		// creating hud camera for axis
		vsg::ref_ptr<vsg::Camera> axisCamera = new vsg::Camera;
		axisCamera->setReferenceFrame(vsg::Transform::RELATIVE_RF);
		axisCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
		axisCamera->setRenderOrder(vsg::Camera::POST_RENDER);
		axisCamera->setAllowEventFocus(false);
		axisCamera->getOrCreateStateSet()->setMode(GL_LIGHTING, vsg::StateAttribute::OFF);
		axisCamera->addChild(m_swCoordAxes.get());

		m_rootnode->addChild(axisCamera.get());

		m_secondLightSource = new vsg::LightSource;
		vsg::Light * light = m_secondLightSource->getLight();
		light->setLightNum(1);
		light->setDiffuse({ 1.0f, 1.0f, 1.0f, 1.0f });
		light->setSpecular({ 0.0f, 0.0f, 0.0f, 0.0f });
		light->setAmbient({ 0.2f, 0.2f, 0.2f, 1.0f });
		SetLightDir(light, vsg::PI_4);
		m_swModel->addChild(m_secondLightSource);

		m_swModel->getOrCreateStateSet()->setMode(GL_LIGHT1, vsg::StateAttribute::OFF);
	}

	Base3DSystem::~Base3DSystem()
	{
		clearModelNode();
	}

	void Base3DSystem::setVerticalScale(double scale)
	{
		auto matrix = vsg::Matrix::scale(1.0, 1.0, scale);
		auto newAxisOrg = m_axisOrg * m_verticalScaleNode->getInverseMatrix() * matrix;
		m_verticalScaleNode->setMatrix(matrix);
		recreateAxis(newAxisOrg);
	}

	void Base3DSystem::setNodeNameConvertor(std::shared_ptr<IOsgNodeNameConvertor> nameConvertor)
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
		if (m_geomConverter)
		{
			m_geomConverter->clearMessagesCallback();
			m_geomConverter->resetModel();
		}

		m_geomConverter.reset();
		m_buildingModel.reset();
		m_geomSettings.reset();
	}

	std::shared_ptr<GeometryConverter> Base3DSystem::getBackGeometryConverter()
	{
		if (!m_geomConverter)
		{
			m_buildingModel = std::make_shared<BuildingModel>();
			m_geomSettings = std::make_shared<GeometrySettings>();
			m_geomConverter = std::make_shared<GeometryConverter>(m_buildingModel, m_geomSettings);
		}
		return m_geomConverter;
	}

	void Base3DSystem::setEnableLight(bool enable)
	{
		auto newState = enable ? vsg::StateAttribute::ON : vsg::StateAttribute::OFF;
		m_swModel->getOrCreateStateSet()->setMode(GL_LIGHTING, newState);
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

	void Base3DSystem::setPolygonMode(vsg::PolygonMode::Mode mode)
	{
		if (mode == vsg::PolygonMode::FILL)
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
		}
	}

	void Base3DSystem::setFrontCullFaceEnabled(bool enable)
	{
		if (enable)
			m_swModel->getOrCreateStateSet()->setAttributeAndModes(m_frontCullFace,
				vsg::StateAttribute::ON | vsg::StateAttribute::OVERRIDE);
		else
			m_swModel->getOrCreateStateSet()->removeAttribute(m_frontCullFace);
	}

	bool Base3DSystem::isAuxNode(vsg::Node const & /*node*/) const
	{
		return false;
	}

	vsg::ref_ptr<vsg::Camera> Base3DSystem::getHUDCamera() const
	{
		return nullptr;
	}

	vsg::ref_ptr<vsg::Group> Base3DSystem::getInteractiveParent() const
	{
		return nullptr;
	}

	void Base3DSystem::updateLightState(vsg::Vec3f const & /*org*/, double /*rad*/)
	{
	}

	vsg::ref_ptr<osgText::Text> Base3DSystem::createText(vsg::Vec4 const & color, std::string const & text, vsg::Vec3 const & position,
		float characterSizeAspectRatio)
	{
		vsg::ref_ptr<osgText::Text> resText = new  osgText::Text();
		//resText->setFont("fonts/calibri.ttf");
		resText->setAlignment(osgText::Text::RIGHT_TOP);
		resText->setAxisAlignment(osgText::Text::SCREEN);
		resText->setColor(color);
		resText->setCharacterSize(0.5f * characterSizeAspectRatio);
		resText->setText(text);
		resText->setPosition(position);
		resText->setEnableDepthWrites(false);

		return resText;
	}

	vsg::ref_ptr<::vsg::Geometry> Base3DSystem::createGrid(double gridR)
	{
		gridR = std::max(gridR, 1.0);

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

		return geom;
	}

	void Base3DSystem::recreateAxisForPlatform(vsg::BoundingBox const & bb)
	{
		ClearAllChildNodes(m_swCoordAxes);

		vsg::ref_ptr<vsg::Geode> geode = new vsg::Geode();
		geode->setName("createCoordinateAxes::geode");
		vsg::ref_ptr<vsg::StateSet> stateset = geode->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, vsg::StateAttribute::OFF);
		float alpha = 0.5f;
		m_swCoordAxes->addChild(geode);

		m_axisOrg = vsg::Vec3d(0.0, .0, 0.0);

		double xLength = (bb.xMax() - bb.xMin()) * 0.5;
		double yLength = (bb.yMax() - bb.yMin()) * 0.5;
		double zLength = bb.zMax() - bb.zMin();

		double r = std::max(xLength, yLength);
		r += r * 0.2;

		// positive axes
		vsg::ref_ptr<vsg::Geometry> geom = new vsg::Geometry();
		geode->addDrawable(geom);

		vsg::ref_ptr<vsg::Vec3dArray> vertices = new vsg::Vec3dArray();

		vertices->push_back(m_axisOrg);
		vsg::Vec3d xEnd = vsg::Vec3d(r, .0, 0.0);
		vertices->push_back(xEnd);

		vertices->push_back(m_axisOrg);
		vsg::Vec3f yEnd = vsg::Vec3d(.0, -r, 0.0);
		vertices->push_back(yEnd);


		vertices->push_back(m_axisOrg);
		vsg::Vec3d zEnd = vsg::Vec3d(0.0, 0.0, r);
		vertices->push_back(zEnd);

		vsg::ref_ptr<vsg::Vec4Array> colors = new vsg::Vec4Array();

		vsg::Vec4f xColor(1.f, 0.f, 0.f, alpha);
		colors->push_back(xColor);
		colors->push_back(xColor);

		vsg::Vec4f yColor(0.f, 0.8f, 0.f, alpha);
		colors->push_back(yColor);
		colors->push_back(yColor);

		vsg::Vec4f zColor(0.f, 0.f, 1.f, alpha);
		colors->push_back(zColor);
		colors->push_back(zColor);
		colors->setBinding(vsg::Array::BIND_PER_VERTEX);
		geom->setColorArray(colors);

		geom->setVertexArray(vertices);
		geom->addPrimitiveSet(new vsg::DrawArrays(vsg::PrimitiveSet::LINES, 0, 6));

		float aspectRation = static_cast<float>(r) * 0.2f;

		geode->addDrawable(createText(xColor, "x", xEnd, aspectRation));
		geode->addDrawable(createText(yColor, "y", yEnd, aspectRation));
		geode->addDrawable(createText(zColor, "z", zEnd, aspectRation));

		// grid
		geode->addDrawable(createGrid(std::max(xLength, yLength) * 1.2));
	}

	vsg::Vec3d const & Base3DSystem::getAxisOrg() const
	{
		return m_axisOrg;
	}

	void Base3DSystem::recreateAxis(vsg::Vec3d const & axisOrg)
	{
		m_axisOrg = axisOrg;
		ClearAllChildNodes(m_swCoordAxes);

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

	vsg::ClipPlane * Base3DSystem::addClipPlane(double a, double b, double c, double d)
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
	}

	vsg::BoundingBox Base3DSystem::calcWorldBoundingBox() const
	{
		vsg::ComputeBoundsVisitor cbv;
		m_swModel->accept(cbv);
		vsg::BoundingBox localBB = cbv.getBoundingBox();
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

	vsg::BoundingSphere Base3DSystem::showNodes(size_t /*prjKey*/, std::set<size_t> const & /*nodes*/)
	{
		ASSERT(false);

		vsg::BoundingSphere boundingSphere;
		return boundingSphere;
	}

	void Base3DSystem::showSkyBox(bool /*show*/, QImage const * cubemap)
	{
	}

	void Base3DSystem::updateVisibilityForNodes(size_t /*prjKey*/, std::set<size_t> const & /*unvisibleNodes*/,
		std::set<size_t> const & /*restoreVisibility*/)
	{
		ASSERT(false);
	}

	void Base3DSystem::deleteNode(std::vector<size_t> const & /*objectKey*/, vsg::Group * /*fromParent = nullptr*/)
	{
		ASSERT(false);
	}

	void Base3DSystem::setSelectedObj(vsg::Group * /*node*/, bool /*selected*/, SelectType /*selType*/)
	{
		ASSERT(false);
	}

	void Base3DSystem::setObjectUnderCursor(std::vector<size_t> const & /*objectKey*/, bool /*selected*/, vsg::Group * /* grp = nullptr*/)
	{
	}

	vsg::Group * Base3DSystem::getRootNode()
	{
		return m_rootnode;
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
		return m_swModel->getNumChildren() < 2;
	}

	void Base3DSystem::clearAllNodes()
	{
		clearModelNode();
		ClearAllChildNodes(m_swCoordAxes);
	}

	void Base3DSystem::clearModelNode()
	{
		auto dontRemoveLight = [this](auto const * node)
		{
			return node != m_secondLightSource;
		};
		ClearAllChildNodes(m_swModel, dontRemoveLight);

		if (m_swModel->getNumChildren() == 0) // ClearAllChildNodes достает m_secondLightSource из m_swModel
			m_swModel->addChild(m_secondLightSource);

		for (auto & pairData : m_unvisibleModel)
			ClearAllChildNodes(pairData.second);
		m_unvisibleModel.clear();
		removeClipPlanes();
	}

	vsg::Switch * Base3DSystem::getCoordinateAxesNode()
	{
		return m_swCoordAxes;
	}

	void Base3DSystem::setAxisParams(AxisParams const & axisParams)
	{
		m_axisParams = axisParams;
	}

	void Base3DSystem::setRootNode(vsg::Group * root)
	{
		m_rootnode = root;
	}

	model3d::ArbitraryPropTypes Base3DSystem::getHideTypes() const
	{
		return {};
	}

	bool Base3DSystem::changeHideTypes(ArbitraryPropTypes)
	{
		return false;
	}

	void Base3DSystem::ClearAllChildNodes(vsg::Group * group, std::function<bool(vsg::Node *)> const & needRemoveNode /*= nullptr*/)
	{
		using node_t = vsg::Node;

		int numChildren = group->getNumChildren();

		for (int i = numChildren - 1; i >= 0; --i)
		{
			node_t * node = group->getChild(i);

			node_t const * constNode = node;
			auto const & parents = constNode->getParents();

			if (needRemoveNode && !needRemoveNode(node))
			{
				/// удалим из списка потомков group узел, который не нужно удалять
				if (parents.size() > 1)
				{
					/// Чтобы сохранить связь с другими родителями для node, удалим таким образом
					group->removeChildren(i, 1); 
				}
				else
				{
					// Остался один родитель(group)
					// Чтобы не удалился node, очистим список родителей таким образом
					const_cast<node_t::ParentList *>(&parents)->clear();
				}
				continue;
			}
			/// хак - чистим родителей
			const_cast<node_t::ParentList *>(&parents)->clear();

			if (auto childGroup = node->asGroup())
				ClearAllChildNodes(childGroup, needRemoveNode);

			node->releaseGLObjects();
			if (node->getStateSet())
				node->setStateSet(nullptr);

			if (needRemoveNode)
				group->removeChildren(i, 1);
		}

		if (!needRemoveNode)
			group->removeChildren(0, numChildren);
	}

	//////////////////////////////////////////////////////////////////////////
	Base3DSystem * MakeBase3DSystem(std::unique_ptr<IOsgNodeNameConvertor> && nodeConv)
	{
		return new Base3DSystem(std::move(nodeConv));
	}

	void SetLightDir(vsg::Light * lihght, double az)
	{
		auto lightPosition = lihght->getPosition();

		double dirXY = std::cos(az) / std::sqrt(2.0);
		double dirZ = std::sin(az);
		lightPosition.x() = static_cast<float>(dirXY);
		lightPosition.y() = static_cast<float>(dirXY);
		lightPosition.z() = static_cast<float>(dirZ);

		lihght->setPosition(lightPosition);
	}
}
