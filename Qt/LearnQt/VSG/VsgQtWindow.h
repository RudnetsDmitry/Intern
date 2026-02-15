#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2021 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

//#include <QVulkanInstance>
#include <QWidget>
#include <QWindow>

#include <vsg/app/Window.h>

#include "KeyboardMap.h"
#include "Viewer.h"

namespace vsg
{
	enum ButtonMask : uint16_t;
}

namespace vsgQt
{
	class Window : public QWidget
	{
		using base_t = QWidget;
	public:
		Window(QWidget * parent);

		~Window() override;
		
		operator vsg::ref_ptr<vsg::Window>() { return m_windowAdapter; }

		/// Initialize the Vulkan integration using VulkanSceneGraph VkInstance/VkSurface support
		virtual void initializeWindow();

		vsg::ref_ptr<vsg::Device> getOrCreateDevice();

		vsg::WindowTraits const & getTraits() const;
		vsg::WindowTraits & getTraits();

		Viewer & getViewer();
		Viewer const & getViewer() const;

	protected:
		void cleanup();

		bool event(QEvent* e) override;

		void paintEvent(QPaintEvent * event) override;
		//void exposeEvent(QExposeEvent*) override;
		void hideEvent(QHideEvent* ev) override;

		void keyPressEvent(QKeyEvent*) override;
		void keyReleaseEvent(QKeyEvent*) override;
		void mouseMoveEvent(QMouseEvent*) override;
		void mousePressEvent(QMouseEvent*) override;
		void mouseReleaseEvent(QMouseEvent*) override;
		void resizeEvent(QResizeEvent*) override;
		void wheelEvent(QWheelEvent*) override;

		/// convert Qt's window coordinate into Vulkan/VSG ones by scaling by the devicePixelRatio()
		template<typename T>
		int32_t convert_coord(T c) const { return static_cast<int32_t>(std::round(static_cast<qreal>(c) * devicePixelRatio())); }

		std::pair<vsg::ButtonMask, uint32_t> convertMouseButtons(QMouseEvent* e) const;
		std::pair<int32_t, int32_t> convertMousePosition(QMouseEvent* e) const;

	private:
		vsg::ref_ptr<vsg::WindowTraits> m_traits;
		vsg::ref_ptr<Viewer> m_viewer;

		vsg::ref_ptr<vsg::Window> m_windowAdapter;
		vsg::ref_ptr<KeyboardMap> m_keyboardMap;

	};

} // namespace vsgQt

EVSG_type_name(vsgQt::Window);
