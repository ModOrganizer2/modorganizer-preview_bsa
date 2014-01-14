/*
Copyright (C) 2014 Sebastian Herbord. All rights reserved.

This file is part of Base Preview plugin for MO

Base Preview plugin is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Base Preview plugin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Base Preview plugin.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "previewbase.h"
#include <utility.h>
#include <QImageReader>
#include <QFileInfo>
#include <QLabel>
#include <QTextEdit>
#include <QtPlugin>


using namespace MOBase;


PreviewBase::PreviewBase()
{
  // set up image reader to be used for all image types qt (the current installation) supports
  auto imageReader = std::bind(&PreviewBase::genImagePreview, this, std::placeholders::_1, std::placeholders::_2);

  foreach (const QByteArray &fileType, QImageReader::supportedImageFormats()) {
    m_PreviewGenerators[QString(fileType).toLower()] = imageReader;
  }

  m_PreviewGenerators["txt"]
    = std::bind(&PreviewBase::genTxtPreview, this, std::placeholders::_1, std::placeholders::_2);

}

bool PreviewBase::init(MOBase::IOrganizer*)
{
  return true;
}

QString PreviewBase::name() const
{
  return tr("Preview Base");
}

QString PreviewBase::author() const
{
  return "Tannin";
}

QString PreviewBase::description() const
{
  return tr("Supports previewing various types of data files");
}

MOBase::VersionInfo PreviewBase::version() const
{
  return VersionInfo(0, 1, 0, VersionInfo::RELEASE_BETA);
}

bool PreviewBase::isActive() const
{
  return true;
}

QList<MOBase::PluginSetting> PreviewBase::settings() const
{
  return QList<MOBase::PluginSetting>();
}

std::set<QString> PreviewBase::supportedExtensions() const
{
  std::set<QString> extensions;
  foreach (const auto &generator, m_PreviewGenerators) {
    extensions.insert(generator.first);
  }

  return extensions;
}

QWidget *PreviewBase::genFilePreview(const QString &fileName, const QSize &maxSize) const
{
  auto iter = m_PreviewGenerators.find(QFileInfo(fileName).completeSuffix().toLower());
  if (iter != m_PreviewGenerators.end()) {
    return iter->second(fileName, maxSize);
  } else {
    return nullptr;
  }
}

QWidget *PreviewBase::genImagePreview(const QString &fileName, const QSize&) const
{
  QLabel *label = new QLabel();
  label->setPixmap(QPixmap(fileName));
  return label;
}

QWidget *PreviewBase::genTxtPreview(const QString &fileName, const QSize&) const
{
  QTextEdit *edit = new QTextEdit();
  edit->setText(MOBase::readFileText(fileName));
  edit->setReadOnly(true);
  return edit;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(previewBase, PreviewBase)
#endif


