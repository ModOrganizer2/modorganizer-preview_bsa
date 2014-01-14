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


#ifndef PREVIEWBASE_H
#define PREVIEWBASE_H

#include <ipluginpreview.h>
#include <functional>

class PreviewBase : public MOBase::IPluginPreview
{

  Q_OBJECT
  Q_INTERFACES(MOBase::IPlugin MOBase::IPluginPreview)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID "org.tannin.PreviewBase" FILE "previewbase.json")
#endif

public:
  PreviewBase();

public:
  virtual bool init(MOBase::IOrganizer *moInfo);
  virtual QString name() const;
  virtual QString author() const;
  virtual QString description() const;
  virtual MOBase::VersionInfo version() const;
  virtual bool isActive() const;
  virtual QList<MOBase::PluginSetting> settings() const;

public:
  virtual std::set<QString> supportedExtensions() const;
  virtual QWidget *genFilePreview(const QString &fileName, const QSize &maxSize) const;

private:

  QWidget *genImagePreview(const QString &fileName, const QSize &maxSize) const;
  QWidget *genTxtPreview(const QString &fileName, const QSize &maxSize) const;

private:
  std::map<QString, std::function<QWidget*(const QString&, const QSize&)> > m_PreviewGenerators;

};

#endif // PREVIEWBASE_H
