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

#include "PythonTypeConverter.h"
#include <QPointF>
#include <QRectF>
#include <QStringList>
#include <QVector3D>
#include <QVector4D>
#include <RcUnit.h>

#undef slots
#include "Python.h"

PythonTypeConverter::PythonTypeConverter()
{
}


QVariant PythonTypeConverter::toVariant(PyObject *input)
{

    if(PyBool_Check(input))
        return QVariant(input == Py_True);
    if(PyLong_Check(input))
        return QVariant((int)PyLong_AsLong(input));
    if(PyFloat_Check(input))
        return QVariant(PyFloat_AsDouble(input));

    QString strVal = toString(input);
    if(!strVal.isNull())
        return strVal;

    if(PyList_Check(input))
    {
        int len = PyList_Size(input);
        QList<QVariant> list;
        for(int i=0; i<len; i++)
            list << toVariant(PyList_GetItem(input, i));
        return bestFittingList(list);
    }
    if(PyTuple_Check(input))
    {
        int len = PyTuple_Size(input);
        QList<QVariant> list;
        for(int i=0; i<len; i++)
            list << toVariant(PyTuple_GetItem(input, i));
        return bestFittingList(list);
    }
    if(PyDict_Check(input))
    {
        QVariantMap map;
        PyObject* key = 0;
        PyObject* value = 0;
        Py_ssize_t i = 0;
        while (PyDict_Next(input, &i, &key, &value))
        {
            map.insert(toVariant(key).toString(), toVariant(value));
        }
        if(map.contains("type"))
        {
            QString type = map["type"].toString();
            if(type == "Point")
                return QPointF(map["x"].toDouble(), map["y"].toDouble());
            if(type == "Size")
                return QSizeF(map["width"].toDouble(), map["height"].toDouble());
            if(type == "Rect")
                return QRectF(map["x"].toDouble(), map["y"].toDouble(),map["width"].toDouble(), map["height"].toDouble());
            if(type == "Vector3D")
                return QVector3D(map["x"].toDouble(), map["y"].toDouble(),map["z"].toDouble());
            if(type == "Vector4D")
                return QVector4D(map["x"].toDouble(), map["y"].toDouble(),map["z"].toDouble(),map["w"].toDouble());
            if(type == "Pose2d")
                return QVariant::fromValue<Pose2d>(Pose2d(map["x"].toDouble(), map["y"].toDouble(),map["phi"].toDouble()));
        }
        return map;

    }
    return QVariant();

}

PyObject* PythonTypeConverter::fromVariant(const QVariant &var)
{
    if(var.type() == QVariant::Invalid)
        Py_RETURN_NONE;
    if(var.type() == QVariant::Bool)
        return PyBool_FromLong(var.toInt());
    if(var.type() == QVariant::Int)
        return PyLong_FromLong(var.toInt());
    if(var.type() == QVariant::Double || var.userType() == QMetaType::Float)
        return PyFloat_FromDouble(var.toDouble());
    if(var.type() == QVariant::String)
        return PyUnicode_FromString(var.toString().toUtf8().data());
    if(var.type() == QVariant::List)
    {
        const QList<QVariant> list = var.toList();
        PyObject* pyList = PyList_New(list.size());
        for(int i=0; i<list.size(); i++)
        {
            PyObject* obj = fromVariant(list[i]);
            PyList_SET_ITEM(pyList, i , obj);
        }
        return pyList;
    }
    if(var.type() == QVariant::Map)
    {
        QVariantMap map = var.toMap();
        PyObject* dict = PyDict_New();
        foreach(const QString& key, map.keys())
        {
            const QVariant& value = map[key];
            PyObject* pKey = fromVariant(key);
            PyObject* pValue = fromVariant(value);
            PyDict_SetItem(dict, pKey, pValue);
            Py_DecRef(pKey);
            Py_DecRef(pValue);
        }
        return dict;
    }
    if(var.canConvert(QVariant::RectF))
    {
        QRectF rect = var.toRectF();
        QVariantMap map;
        map["type"] = "Rect";
        map["width"] = rect.width();
        map["height"] = rect.height();
        map["x"] = rect.x();
        map["y"] = rect.y();
        return fromVariant(QVariant(map));
    }
    if(var.canConvert(QVariant::PointF))
    {
        QPointF point = var.toPointF();
        QVariantMap map;
        map["type"] = "Point";
        map["x"] = point.x();
        map["y"] = point.y();
        return fromVariant(QVariant(map));
    }
    if(var.canConvert(QVariant::SizeF))
    {
        QSizeF size = var.toSizeF();
        QVariantMap map;
        map["type"] = "Size";
        map["width"] = size.width();
        map["height"] = size.height();
        return fromVariant(QVariant(map));
    }
    if(var.canConvert(QVariant::Vector3D))
    {
        QVector3D vector = var.value<QVector3D>();
        QVariantMap map;
        map["type"] = "Vector3D";
        map["x"] = vector.x();
        map["y"] = vector.y();
        map["z"] = vector.z();
        return fromVariant(QVariant(map));
    }
    if(var.canConvert(QVariant::Vector4D))
    {
        QVector4D vector = var.value<QVector4D>();
        QVariantMap map;
        map["type"] = "Vector4D";
        map["x"] = vector.x();
        map["y"] = vector.y();
        map["z"] = vector.z();
        map["w"] = vector.w();
        return fromVariant(QVariant(map));
    }
    if(var.canConvert<Pose2d>())
    {
        Pose2d pose = var.value<Pose2d>();
        QVariantMap map;
        map["type"] = "Pose2d";
        map["x"] = pose.x;
        map["y"] = pose.y;
        map["phi"] = pose.phi;
        return fromVariant(QVariant(map));
    }
    Py_RETURN_NONE;
}

QString PythonTypeConverter::toString(PyObject *input, bool force)
{
    if(!input)
        return QString();
    if(PyBytes_Check(input))
        return QString(PyBytes_AsString(input));
    else if(PyUnicode_Check(input))
    {
        PyObject* utf8 = PyUnicode_AsUTF8String(input);
        QString var = QString::fromUtf8(PyBytes_AsString(utf8));
        Py_XDECREF(utf8);
        return var;
    }
    if(force)
    {
        PyObject* str = PyObject_Str(input);
        QString val = toString(str);
        Py_XDECREF(str);
        return val;
    }
    return QString();
}

PyObject* PythonTypeConverter::fromString(QString str)
{
    return PyUnicode_FromString(str.toUtf8().data());
}

QVariant PythonTypeConverter::bestFittingList(const QList<QVariant> &list)
{
    foreach(const QVariant& var, list)
    {
        if(var.type() != QVariant::String)
            return list;
    }
    return QVariant(list).toStringList();
}
