////// @file	3DSystemBase.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    20.12.2025 11:58
///

#pragma once


#pragma once

#include "I3DSystem.h"

#include <map>

#include <vsg/core/ref_ptr.h>

class GeometrySettings;

namespace vsg
{
	class DirectionalLight;
	class Camera;
	class MatrixTransform;
	class Geometry;
	class Light;
	class Text;
}

namespace model3d
{
	class Base3DSystem : public I3DSystem
	{
	protected:
		vsg::ref_ptr<vsg::Group>					m_rootNode;
		vsg::ref_ptr<vsg::MatrixTransform>			m_verticalScaleNode;

		vsg::ref_ptr<vsg::Group>					m_swCoordAxes;
		//vsg::ref_ptr<vsg::ClipNode>					m_clipNode;
		vsg::ref_ptr<vsg::Switch>					m_swModel;

		vsg::ref_ptr<vsg::MatrixTransform>			m_transformLight;

		std::optional<PolygonMode>				m_polygonMode;			// Режим отображения объектов в модели (существует, когда модель отображается ребрами или точками)
		std::optional<PolygonMode>				m_polygonModeForSelect; // Режим отображения выбранных объетов (рисуются грани) - используется если модль отображается ребрами или точками
		std::shared_ptr<INodeNameConvertor> m_nodeConv;
		std::optional<CullMode> m_frontCullFace;

		std::map<std::vector<size_t>, vsg::ref_ptr<vsg::Group>> m_unvisibleModel;

		/// данный источник света используется для моделей, загруженных из IFC
		vsg::ref_ptr<vsg::DirectionalLight> m_secondLightSource;
		/// основной источник света
		vsg::ref_ptr<vsg::Node> m_mainLightSource;

		// Надо ли отображать границы тел
		bool m_showBounds;

		// Параметры координатных осей
		AxisParams m_axisParams;

		// Центр координатных осей
		CPoint3D m_axisOrg;

	public:
		Base3DSystem(std::unique_ptr<INodeNameConvertor> nodeConv);
		virtual ~Base3DSystem();

	public:
		void setNodeNameConvertor(std::shared_ptr<INodeNameConvertor> nameConvertor) override;
		bool isValidNodeName(std::string const & nodeName) const override;
		std::vector<size_t> nodeName2Key(std::string const & nodeName) const override;
		std::wstring entityInfo(std::vector<size_t> const & entityId) const override;
		void connectOnObjectUnderCursor(std::function<void(std::vector<size_t> const &)> func) override;
		bool isPresentEntityInModel(std::vector<size_t> const & objectKey) const override;
		void resetModel() override;
		void setShowBoundsMode(bool showBounds) override;
		bool getShowBoundsMode() const override;
		virtual std::optional<float> getLODBound() const override;
		virtual void setLODBound(std::optional<float> const &) override;
		virtual bool useTransparentModeForSelect() const override;
		virtual void setTransparentModeForSelect(bool set) override;
		virtual void setHighlightSelected(bool flag) override;

		virtual void setSelectColor(SelectType type, COLORREF color)override;

		void setEnableLight(bool enable);

		void setPolygonMode(std::optional<PolygonMode> const & mode) override;
		void setFrontCullFaceEnabled(bool enable) override;
		bool isAuxNode(vsg::Node const & node) const override;
		vsg::Camera * getHUDCamera() const override;
		vsg::Group * getInteractiveParent() const override;
		void updateLightState(CPoint3D const & /*org*/, double /*rad*/) override;

		static vsg::ref_ptr<vsg::Text> createText(std::array<float, 4> const & color, std::string const & text, CPoint3D const & position, float characterSizeAspectRatio);
		static vsg::ref_ptr<::vsg::Geometry> createGrid(double gridR);

		void recreateAxisForPlatform(CRect3D const & bb) override;
		void recreateAxis(CPoint3D const& org = CPoint3D(.0)) override;
		CPoint3D const & getAxisOrg() const override;

		void clearUnderCursor();
		void clearSelection() override;

		bool isObjectUnderCursor(std::vector<size_t> const & objectKey) const override;
		vsg::Group* findNodeById(std::vector<size_t> const & objKey) const override;

		std::set<::vsg::Group *> selectedObjects() const override;
		/*vsg::ClipPlane * addClipPlane(double a, double b, double c, double d) override;
		void removeClipPlanes() override;
		void removeClipPlane(vsg::ClipPlane * clipPlane);*/
		CRect3D calcWorldBoundingBox() const override;

		::vsg::Group* objUnderCursor() const override;
		CSphere showNodes(size_t prjKey, std::set<size_t> const& nodes) override;
		void showSkyBox(bool show, /*QImage*/void const * cubemap) override;

		void updateVisibilityForNodes(size_t prjKey, std::set<size_t> const & unvisibleNodes,
			std::set<size_t> const & restoreVisibility) override;
		void deleteNode(std::vector<size_t> const & objectKey, vsg::Group * fromParent = nullptr) override;
		void setSelectedObj(vsg::Group* node, bool selected, SelectType selType) override;
		void setObjectUnderCursor(std::vector<size_t> const & objectKey, bool selected, vsg::Group* grp = nullptr) override;
		vsg::Group * getRootNode() override;
		vsg::MatrixTransform const & getVerticalScaleNode() const override;
		void setVerticalScale(double scale) override;
		vsg::Switch* getModelNode() override;
		bool isEmptyModel() const override;
		void clearAllNodes() override;
		void clearModelNode() override;

		vsg::Group * getCoordinateAxesNode() override;
		void setAxisParams(AxisParams const & axisParams);

		void setRootNode(vsg::Group * root) override;

		/// получить/изменить настройки скрытия объектов
		/*virtual ArbitraryPropTypes getHideTypes() const;
		virtual bool changeHideTypes(ArbitraryPropTypes);*/

	protected:
		static void ClearAllChildNodes(vsg::Node & group, std::function<bool (vsg::Node *)> const & needRemoveNode = nullptr);
	};

	//////////////////////////////////////////////////////////////////////////
	 Base3DSystem * MakeBase3DSystem(std::unique_ptr<INodeNameConvertor> && nodeConv = nullptr);
}
