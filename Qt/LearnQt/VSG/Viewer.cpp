/* <editor-fold desc="MIT License">

Copyright(c) 2021 Robert Osfield, Andre Normann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include "stdafx.h"

#include <vsg/all.h>

#include <QtWidgets/QApplication>

#include "Viewer.h"


namespace vsgQt {

	bool Viewer::pollEvents(bool discardPreviousEvents)
	{
		if (discardPreviousEvents) _events.clear();
		for (auto& window : _windows)
		{
			_events.splice(_events.end(), window->bufferedEvents);
			window->bufferedEvents.clear();
		}

		return !_events.empty();
	}

	void Viewer::render(double simulationTime)
	{
		if (advanceToNextFrame(simulationTime))
		{
			handleEvents();
			update();
			recordAndSubmit();
			present();
		}
		else
		{
			if (status->cancel())
			{
				QCoreApplication::quit();
			}
		}
	}
}
