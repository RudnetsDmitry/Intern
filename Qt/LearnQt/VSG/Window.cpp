/* <editor-fold desc="MIT License">

Copyright(c) 2021 Robert Osfield, Andre Normann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */


#include "stdafx.h"
#include "Window.h"

#include <vsg/all.h>

#include <QPlatformSurfaceEvent>
#include <QtWidgets/QApplication>

#include <vulkan/vulkan.h>

namespace vsgQt {

	Window::Window(vsg::ref_ptr<vsgQt::Viewer> in_viewer, vsg::ref_ptr<vsg::WindowTraits> in_traits, QWindow* parent) :
		QWindow(parent),
		m_viewer(in_viewer),
		m_keyboardMap(KeyboardMap::create())
	{
		if (in_traits)
		{
			m_traits = vsg::WindowTraits::create(*in_traits);
			setGeometry(m_traits->x, m_traits->y, m_traits->width, m_traits->height);
		}
		else
		{
			m_traits = vsg::WindowTraits::create();
			m_traits->x = x();
			m_traits->y = y();
			m_traits->width = width();
			m_traits->height = height();
		}
	}

	Window::~Window()
	{
		cleanup();
	}

	void Window::initializeWindow()
	{
		if (m_windowAdapter)
			return;

		if (!m_traits)
			m_traits = vsg::WindowTraits::create();

#if defined(VK_USE_PLATFORM_WIN32_KHR)
		m_traits->nativeWindow = reinterpret_cast<HWND>(winId());
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		m_traits->nativeWindow = static_cast<::Window>(winId());
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		m_traits->nativeWindow = static_cast<xcb_window_t>(winId());
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
		m_traits->nativeWindow = winId();
#endif

		m_traits->x = convert_coord(x());
		m_traits->y = convert_coord(y());
		m_traits->width = convert_coord(width());
		m_traits->height = convert_coord(height());

		m_windowAdapter = vsg::Window::create(m_traits);
	}

	vsg::ref_ptr<vsg::Device> Window::getOrCreateDevice()
	{
		return m_windowAdapter->getOrCreateDevice();
	}

	vsg::WindowTraits const& Window::getTraits() const
	{
		return *m_traits;
	}

	void Window::cleanup()
	{
		// remove links to all the VSG related classes.
		if (m_windowAdapter)
		{
			// wait for all rendering to be completed before we start cleaning up resources.
			if (m_viewer)
			{
				m_viewer->deviceWaitIdle();
				m_viewer->removeWindow(m_windowAdapter);
			}

			m_windowAdapter->releaseWindow();
		}

		m_windowAdapter = {};
		m_viewer = {};
	}

	bool Window::event(QEvent* e)
	{
		switch (e->type())
		{
		case QEvent::PlatformSurface: {
			auto surfaceEvent = dynamic_cast<QPlatformSurfaceEvent*>(e);
			if (surfaceEvent->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed)
			{
				vsg::clock::time_point event_time = vsg::clock::now();
				m_windowAdapter->bufferedEvents.push_back(vsg::CloseWindowEvent::create(m_windowAdapter, event_time));

				cleanup();
			}
			break;
		}

		case QEvent::Paint:
			if (m_viewer)
				m_viewer->render();
			break;

		default:
			break;
		}

		return QWindow::event(e);
	}

	void Window::exposeEvent(QExposeEvent* /*e*/)
	{
		if (!m_initialized && isExposed())
		{
			initializeWindow();
		}

		if (m_viewer)
			m_viewer->render();
	}

	void Window::hideEvent(QHideEvent* /*e*/)
	{
	}

	void Window::resizeEvent(QResizeEvent* /*e*/)
	{
		if (!m_windowAdapter)
			return;

		vsg::clock::time_point event_time = vsg::clock::now();
		m_windowAdapter->bufferedEvents.push_back(vsg::ConfigureWindowEvent::create(m_windowAdapter, event_time, convert_coord(x()), convert_coord(y()), convert_coord(width()), convert_coord(height())));

		m_windowAdapter->resize();

		if (m_viewer)
			m_viewer->render();
	}

	void Window::keyPressEvent(QKeyEvent* e)
	{
		if (!m_windowAdapter)
			return;

		vsg::KeySymbol keySymbol, modifiedKeySymbol;
		vsg::KeyModifier keyModifier;

		if (m_keyboardMap->getKeySymbol(e, keySymbol, modifiedKeySymbol, keyModifier))
		{
			vsg::clock::time_point event_time = vsg::clock::now();
			m_windowAdapter->bufferedEvents.push_back(vsg::KeyPressEvent::create(m_windowAdapter, event_time, keySymbol, modifiedKeySymbol, keyModifier));
		}

		if (m_viewer)
			m_viewer->render();
	}

	void Window::keyReleaseEvent(QKeyEvent* e)
	{
		if (!m_windowAdapter)
			return;

		vsg::KeySymbol keySymbol, modifiedKeySymbol;
		vsg::KeyModifier keyModifier;

		if (m_keyboardMap->getKeySymbol(e, keySymbol, modifiedKeySymbol, keyModifier))
		{
			vsg::clock::time_point event_time = vsg::clock::now();
			m_windowAdapter->bufferedEvents.push_back(vsg::KeyReleaseEvent::create(m_windowAdapter, event_time, keySymbol, modifiedKeySymbol, keyModifier));
		}

		if (m_viewer)
			m_viewer->render();
	}

	void Window::mouseMoveEvent(QMouseEvent* e)
	{
		if (!m_windowAdapter)
			return;

		vsg::clock::time_point event_time = vsg::clock::now();

		auto [mask, button] = convertMouseButtons(e);
		auto [x, y] = convertMousePosition(e);

		m_windowAdapter->bufferedEvents.push_back(vsg::MoveEvent::create(m_windowAdapter, event_time, x, y, mask));

		if (m_viewer)
			m_viewer->render();
	}

	void Window::mousePressEvent(QMouseEvent* e)
	{
		if (!m_windowAdapter)
			return;

		vsg::clock::time_point event_time = vsg::clock::now();

		auto [mask, button] = convertMouseButtons(e);
		auto [x, y] = convertMousePosition(e);

		m_windowAdapter->bufferedEvents.push_back(vsg::ButtonPressEvent::create(m_windowAdapter, event_time, x, y, mask, button));

		if (m_viewer)
			m_viewer->render();
	}

	void Window::mouseReleaseEvent(QMouseEvent* e)
	{
		if (!m_windowAdapter)
			return;

		vsg::clock::time_point event_time = vsg::clock::now();

		auto [mask, button] = convertMouseButtons(e);
		auto [x, y] = convertMousePosition(e);

		m_windowAdapter->bufferedEvents.push_back(vsg::ButtonReleaseEvent::create(m_windowAdapter, event_time, x, y, mask, button));

		if (m_viewer)
			m_viewer->render();
	}

	void Window::wheelEvent(QWheelEvent* e)
	{
		if (!m_windowAdapter)
			return;

		vsg::clock::time_point event_time = vsg::clock::now();
		m_windowAdapter->bufferedEvents.push_back(vsg::ScrollWheelEvent::create(m_windowAdapter, event_time, e->angleDelta().y() < 0 ? vsg::vec3(0.0f, -1.0f, 0.0f) : vsg::vec3(0.0f, 1.0f, 0.0f)));

		if (m_viewer)
			m_viewer->render();
	}

	std::pair<vsg::ButtonMask, uint32_t> Window::convertMouseButtons(QMouseEvent* e) const
	{
		uint16_t mask{ 0 };
		uint32_t button = 0;

		if (e->buttons() & Qt::LeftButton) mask = mask | vsg::BUTTON_MASK_1;
		if (e->buttons() & Qt::MiddleButton) mask = mask | vsg::BUTTON_MASK_2;
		if (e->buttons() & Qt::RightButton) mask = mask | vsg::BUTTON_MASK_3;

		switch (e->button())
		{
		case Qt::LeftButton: button = 1; break;
		case Qt::MiddleButton: button = 2; break;
		case Qt::RightButton: button = 3; break;
		default: break;
		}

		return { static_cast<vsg::ButtonMask>(mask), button };
	}

	std::pair<int32_t, int32_t> Window::convertMousePosition(QMouseEvent* e) const
	{
#if QT_VERSION_MAJOR == 6
		return { convert_coord(e->position().x()), convert_coord(e->position().y()) };
#else
		return { convert_coord(e->x()), convert_coord(e->y()) };
#endif
	}

}
