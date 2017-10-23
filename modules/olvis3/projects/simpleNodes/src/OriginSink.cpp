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

#define _AFXDLL
#include <afxdisp.h>

#include "OriginSink.h"

// According to: http://www.originlab.com/doc/COM/Images-and-Plots-Live-Update#VC.2B.2B

const std::map<int, Origin::COLDATAFORMAT> OriginSink::opencvToOrigin = {
	{CV_8U, Origin::DF_BYTE},
	{CV_8S, Origin::DF_CHAR},
	{CV_16U, Origin::DF_USHORT},
	{CV_16S, Origin::DF_SHORT},
	{CV_32S, Origin::DF_LONG},
	{CV_32F, Origin::DF_FLOAT},
	{CV_64F, Origin::DF_DOUBLE}};

const std::map<int, VARENUM> OriginSink::opencvToVariantType = {
	{CV_8U, VT_UI1},
	{CV_8S, VT_I1},
	{CV_16U, VT_UI2},
	{CV_16S, VT_I2},
	{CV_32S, VT_I4},
	{CV_32F, VT_R4},
	{CV_64F, VT_R8}};

REGISTER_FILTER(OriginSink);

OriginSink::OriginSink()
{
	setName(QObject::tr("Origin Sink"));
	setDesc(QObject::tr("Streams incoming data to Origin"));
	setGroup("output");

	m_frameIn.setName(QObject::tr("Image In"));
	m_frameIn.setDesc(QObject::tr("Image input"));
	addInputPort(m_frameIn);
}

void OriginSink::start()
{
	CoInitialize(nullptr);

	m_originApplication.CreateInstance(__uuidof(Origin::ApplicationSI));
	m_originApplication->PutVisible(Origin::MAINWND_SHOW_BRING_TO_FRONT);

	m_originApplication->CreatePage(_variant_t(static_cast<long>(Origin::OPT_MATRIX)), vtMissing,
		vtMissing, _variant_t(2));

	auto matrixPage = m_originApplication->GetActivePage();
	m_originSheet = matrixPage->GetLayers()->GetItem(_variant_t(0));
	m_originSheet->Activate();
}

void OriginSink::stop()
{
	m_initialized = false;
}

void OriginSink::execute()
{
	const cv::Mat frame = m_frameIn;

	if (frame.channels() != 1)
		throw std::runtime_error("Only singel channel data is allowed");

	if (!m_initialized && m_originSheet)
	{
		m_originSheet->PutCols(frame.cols);
		m_originSheet->PutRows(frame.rows);

		m_originMatrixObject = m_originSheet->GetMatrixObjects()->GetItem(_variant_t(0));

		if (!m_originMatrixObject)
			m_originMatrixObject = m_originSheet->GetMatrixObjects()->Add();

		m_originMatrixObject->Activate();
		m_originMatrixObject->DataFormat = opencvToOrigin.at(frame.depth());

        m_initialized = false;
	}

	if(!m_originMatrixObject)
		return;

	COleSafeArray safeArray;
	DWORD bounds[] = {frame.rows, frame.cols};

	safeArray.Create(opencvToVariantType.at(frame.depth()), 2, bounds);

	for (auto row = 0; row < frame.rows; ++row)
		for (auto col = 0; col < frame.cols; ++col)
		{
			long index[] = {row, col};

			if (frame.depth() == CV_8U)
				addToSafeArray<uint8_t>(safeArray, frame, index);
			else if (frame.depth() == CV_8S)
				addToSafeArray<int8_t>(safeArray, frame, index);
			else if (frame.depth() == CV_16U)
				addToSafeArray<uint16_t>(safeArray, frame, index);
			else if (frame.depth() == CV_16S)
				addToSafeArray<int16_t>(safeArray, frame, index);
			else if (frame.depth() == CV_32S)
				addToSafeArray<int32_t>(safeArray, frame, index);
			else if (frame.depth() == CV_32F)
				addToSafeArray<float>(safeArray, frame, index);
			else if (frame.depth() == CV_64F)
				addToSafeArray<double>(safeArray, frame, index);
		}

	m_originMatrixObject->SetData(safeArray, _variant_t(0), _variant_t(0));
}

template <class Type>
void OriginSink::addToSafeArray(COleSafeArray& safeArray, cv::Mat frame, long index[2])
{
	safeArray.PutElement(index, const_cast<Type *>(&frame.at<Type>(index[0], index[1])));
}
