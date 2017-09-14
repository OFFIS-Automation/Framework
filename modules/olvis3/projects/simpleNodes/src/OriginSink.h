// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <filter/PluginInterface.h>

#include <ports/ImagePort.h>
#include <ports/StringPort.h>
#include <ports/IntegerPort.h>

#include <Origin.h>

class COleSafeArray;

#include <map>

class OriginSink : public UserFilter
{
	private:
		static const std::map<int, Origin::COLDATAFORMAT> opencvToOrigin;
		static const std::map<int, VARENUM> opencvToVariantType;

	public:
		OriginSink();

		void execute();
		void start();
		void stop();

	private:
		template <class Type>
		void addToSafeArray(COleSafeArray& safeArray, cv::Mat frame, long index[2]);

	private:
		// Ports
        in::Image m_frameIn;

		// Member
		bool m_initialized = false;

		// Origin
		Origin::IOApplicationPtr m_originApplication;
		Origin::MatrixSheetPtr m_originSheet;
		Origin::MatrixObjectPtr m_originMatrixObject;
};

#endif // VIDEOSTREAMER_H
