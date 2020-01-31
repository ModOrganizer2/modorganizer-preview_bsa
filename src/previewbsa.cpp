/*
Copyright (C) 2014 Sebastian Herbord. All rights reserved.

This file is part of Bsa Preview plugin for MO

Bsa Preview plugin is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bsa Preview plugin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Bsa Preview plugin.  If not, see <http://www.gnu.org/licenses/>.
*/
#define QT

#include "previewbsa.h"
#include <utility.h>
#include <QImageReader>
#include <QFileInfo>
#include <QLabel>
#include <QTextEdit>
#include <QtPlugin>
#include <QApplication>
#include <QDesktopWidget>
#include <libbsarch.h>
#include <bs_archive_auto.hpp>
using namespace libbsarch;


using namespace MOBase;



PreviewBsa::PreviewBsa()
  : m_MOInfo(nullptr)
{
}

bool PreviewBsa::init(IOrganizer *moInfo)
{
  m_MOInfo = moInfo;

  if (!isActive()) {
    return true;
  }

  auto bsaPreview = std::bind(&PreviewBsa::genBsaPreview, this, std::placeholders::_1, std::placeholders::_2);
 
  m_PreviewGenerators["bsa"] = bsaPreview;
  m_PreviewGenerators["ba2"] = bsaPreview;
  
  return true;
}

QString PreviewBsa::name() const
{
  return "Preview Bsa";
}

QString PreviewBsa::author() const
{
  return "AL12";
}

QString PreviewBsa::description() const
{
  return tr("Supports previewing contents of Bethesda Archive files, BSAs and BA2s");
}

MOBase::VersionInfo PreviewBsa::version() const
{
  return VersionInfo(0, 0, 1, VersionInfo::RELEASE_ALPHA);
}

bool PreviewBsa::isActive() const
{
  return m_MOInfo->pluginSetting(name(), "enabled").toBool();
}

QList<MOBase::PluginSetting> PreviewBsa::settings() const
{
  QList<PluginSetting> result;
  result.push_back(PluginSetting("enabled", tr("Enable previewing of BSA file contents"), QVariant(true)));
  return result;
}

std::set<QString> PreviewBsa::supportedExtensions() const
{
  std::set<QString> extensions;
  for (const auto &generator : m_PreviewGenerators) {
    extensions.insert(generator.first);
  }

  return extensions;
}

QWidget *PreviewBsa::genFilePreview(const QString &fileName, const QSize &maxSize) const
{
  auto iter = m_PreviewGenerators.find(QFileInfo(fileName).suffix().toLower());
  if (iter != m_PreviewGenerators.end()) {
    return iter->second(fileName, maxSize);
  } else {
    return nullptr;
  }
}

QWidget *PreviewBsa::genImagePreview(const QString &fileName, const QSize&) const
{
  QLabel *label = new QLabel();
  QPixmap pic = QPixmap(fileName);
  QSize screenSize = QApplication::desktop()->screenGeometry().size();
  // ensure the output image is no more than 80% of the screen height.
  // If the aspect ratio is higher than that of the screen this would still allow the image to extend
  // beyond the screen but it ensures you can drag the window and close it
  int maxHeight = static_cast<int>(screenSize.height() * 0.8f);
  if (pic.size().height() > maxHeight) {
    pic = pic.scaledToHeight(maxHeight, Qt::SmoothTransformation);
  }
  label->setPixmap(pic);
  return label;
}

QWidget *PreviewBsa::genBsaPreview(const QString &fileName, const QSize&) const
{
  bs_archive_auto arch; //bs_archive_auto is easier to use, but is less performant when working with memory
  arch.load_from_disk("C:/Archive.bsa");
  const auto& files = arch.list_files();
  QString result = "";
  for (auto file : files) {
    result = result + QString(file) + "\n";
  }
  QTextEdit *edit = new QTextEdit();
  edit->setText(result);
  edit->setReadOnly(true);
  return edit;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(previewBsa, PreviewBsa)
#endif


