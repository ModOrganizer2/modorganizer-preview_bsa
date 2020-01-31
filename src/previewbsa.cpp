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


#include "previewbsa.h"
#include <utility.h>
#include <QImageReader>
#include <QFileInfo>
#include <QLabel>
#include <QTextEdit>
#include <QtPlugin>
#include <QApplication>
#include <QDesktopWidget>


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

  const QStringList& blacklist = m_MOInfo->pluginSetting(name(), "blacklisted_extensions").toString().toLower().split(',');

  // set up image reader to be used for all image types qt (the current installation) supports
  auto imageReader = std::bind(&PreviewBsa::genImagePreview, this, std::placeholders::_1, std::placeholders::_2);

  foreach(const QByteArray & fileType, QImageReader::supportedImageFormats()) {
    auto strFileType = QString(fileType).toLower();

    // skip dds as that one is handled by the dds preview plugin.
    if (strFileType == "dds" || blacklist.contains(strFileType))
      continue;

    m_PreviewGenerators[strFileType] = imageReader;
  }

  const QStringList supportedTextFormats = { "txt", "ini", "json", "log", "cfg" };

  auto textReader = std::bind(&PreviewBsa::genTxtPreview, this, std::placeholders::_1, std::placeholders::_2);

  foreach(const QString fileType, supportedTextFormats) {

    // skip blacklisted ones
    if (blacklist.contains(fileType))
      continue;

    m_PreviewGenerators[fileType] = textReader;
  }
  return true;
}

QString PreviewBsa::name() const
{
  return "Preview Bsa";
}

QString PreviewBsa::author() const
{
  return "Tannin";
}

QString PreviewBsa::description() const
{
  return tr("Supports previewing various types of data files");
}

MOBase::VersionInfo PreviewBsa::version() const
{
  return VersionInfo(1, 1, 0, VersionInfo::RELEASE_FINAL);
}

bool PreviewBsa::isActive() const
{
  m_MOInfo->pluginSetting(name(), "enabled").toBool();
}

QList<MOBase::PluginSetting> PreviewBsa::settings() const
{
  QList<PluginSetting> result;
  result.push_back(PluginSetting("enabled", tr("Enable previewing of basic file types, such as images and text files."), QVariant(true)));
  result.push_back(PluginSetting("blacklisted_extensions", tr("Specify a list of comma separated extensions (without \".\") "
    "that should not be previewed by this plugin."), QVariant("")));
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

QWidget *PreviewBsa::genTxtPreview(const QString &fileName, const QSize&) const
{
  QTextEdit *edit = new QTextEdit();
  edit->setText(MOBase::readFileText(fileName));
  edit->setReadOnly(true);
  return edit;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(previewBsa, PreviewBsa)
#endif


