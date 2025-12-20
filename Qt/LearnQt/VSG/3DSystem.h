/// @file	3DSystem.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    11.12.2025 22:31
///

#pragma once
#include <functional>

#include "3DStructures.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace vsg
{
	class Node;
	class Group;
	class Camera;
	class MatrixTransform;
	class Switch;
}

namespace model3d
{
	/// интерфейс обработки узлов
	struct INodeNameConvertor
	{
		virtual ~INodeNameConvertor() {}
		virtual bool isValidNodeName(std::string const & nodeName) const = 0;
		virtual std::vector<size_t> nodeName2Key(std::string const & nodeName) const = 0;
	};

	struct  I3DSystem
	{
		virtual void setNodeNameConvertor(std::shared_ptr<INodeNameConvertor>) = 0;
		/// есть ли объект в модели
		virtual bool isPresentEntityInModel(std::vector<size_t> const & objectKey) const = 0;
		/// очистить выделенные объекты
		virtual void clearSelection() = 0;
		virtual void clearUnderCursor() = 0;
		/// установить выбранные сущности IFC
		virtual void setObjectUnderCursor(std::vector<size_t> const & objectKey, bool selected, vsg::Group * node = nullptr) = 0;
		virtual void setSelectedObj(vsg::Group * node, bool selected, SelectType selType) = 0;
		/// является ли объект выбранным
		virtual bool isObjectUnderCursor(std::vector<size_t> const & objectKey) const = 0;
		/// удалить узел
		virtual void deleteNode(std::vector<size_t> const & objectKey, vsg::Group * fromParent = nullptr) = 0;
		/// получить узел по ключу
		virtual vsg::Group * findNodeById(std::vector<size_t> const & objKey) const = 0;
		/// очистить модель
		virtual void resetModel() = 0;

		/// установить режим прорисовки границ
		virtual void setShowBoundsMode(bool showBounds) = 0;
		virtual bool getShowBoundsMode() const = 0;

		/// получить границу для отображения упрощенного объекта
		virtual std::optional<float> getLODBound() const = 0;
		virtual void setLODBound(std::optional<float> const &) = 0;

		/// использовать полупрозрачность при выделении объекта
		virtual bool useTransparentModeForSelect() const = 0;
		virtual void setTransparentModeForSelect(bool set) = 0;

		/// подсвечивать выделенные объекты и объекты под курсором
		virtual void setHighlightSelected(bool flag) = 0;

		virtual void setSelectColor(SelectType type, COLORREF color) = 0;

		/// установить режим отображения полигоном
		virtual void setPolygonMode(int mode) = 0; /*VkPolygonMode*/
		///
		virtual void setFrontCullFaceEnabled(bool enable) = 0;

		virtual bool isAuxNode(vsg::Node const & node) const = 0;
		virtual vsg::Camera * getHUDCamera() const = 0;
		virtual vsg::Group * getInteractiveParent() const = 0;

		// todo
		virtual vsg::Group * getRootNode() = 0;
		virtual vsg::MatrixTransform const & getVerticalScaleNode() const = 0;
		// Установить вертикальный масштаб
		virtual void setVerticalScale(double scale) = 0;
		virtual vsg::Switch * getModelNode() = 0;
		virtual bool isEmptyModel() const = 0;
		virtual void clearModelNode() = 0;
		virtual void clearAllNodes() = 0;
		virtual vsg::Switch * getCoordinateAxesNode() = 0;
		virtual void setRootNode(vsg::Group * root) = 0;

		virtual void connectOnObjectUnderCursor(std::function<void(std::vector<size_t> const &)>) = 0;

		virtual bool isValidNodeName(std::string const & nodeName) const = 0;
		virtual std::vector<size_t> nodeName2Key(std::string const & nodeName) const = 0;
		virtual std::wstring entityInfo(std::vector<size_t> const & entityId) const = 0;

		virtual void recreateAxis(CPoint3D const & org = {}) = 0;
		virtual void recreateAxisForPlatform(osg::BoundingBox const &) = 0;
		virtual void updateLightState(osg::Vec3f const & org, double rad) = 0;
		virtual osg::Vec3d const & getAxisOrg() const = 0;

		virtual void updateVisibilityForNodes(size_t prjKey, std::set<size_t> const & unvisibleNodes,
			std::set<size_t> const & restoreVisibility) = 0;
		virtual osg::BoundingSphere showNodes(size_t prjKey, std::set<size_t> const& nodes) = 0;
		virtual void showSkyBox(bool show, QImage const * cubemap) = 0;

		virtual ::osg::Group* objUnderCursor() const = 0;
		virtual std::set<::osg::Group *> selectedObjects() const = 0;

		virtual osg::ClipPlane * addClipPlane(double a, double b, double c, double d) = 0;
		virtual void removeClipPlanes() = 0;
		virtual void removeClipPlane(osg::ClipPlane * clipPlane) = 0;

		virtual osg::BoundingBox calcWorldBoundingBox() const = 0;
	};

	Base3DSystem * Make3DSystem(std::unique_ptr<INodeNameConvertor> && nodeConv = nullptr);
	PDEXTGUICLS_EXT_CLASS void SetLightDir(osg::Light * lihght, double az);
}
