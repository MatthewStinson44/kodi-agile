/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "AddonUtils.h"
#include "Application.h"
#include "utils/XBMCTinyXML.h"
#include "addons/Skin.h"
#include "LanguageHook.h"
#ifdef ENABLE_XBMC_TRACE_API
#include "utils/log.h"
#endif

namespace XBMCAddonUtils
{
  GuiLock::GuiLock(XBMCAddon::LanguageHook* languageHook, bool offScreen)
    : m_languageHook(languageHook), m_offScreen(offScreen)
  {
    if (!m_languageHook)
      m_languageHook = XBMCAddon::LanguageHook::GetLanguageHook();
    if (m_languageHook)
      m_languageHook->DelayedCallOpen();

    if (!m_offScreen)
      g_application.LockFrameMoveGuard();
  }

  GuiLock::~GuiLock()
  {
    if (!m_offScreen)
      g_application.UnlockFrameMoveGuard();

    if (m_languageHook)
      m_languageHook->DelayedCallClose();
  }

  static char defaultImage[1024];

  const char *getDefaultImage(const char* cControlType, const char* cTextureType)
  {
    // create an xml block so that we can resolve our defaults
    // <control type="type">
    //   <description />
    // </control>
    TiXmlElement control("control");
    control.SetAttribute("type", cControlType);
    TiXmlElement filler("description");
    control.InsertEndChild(filler);
    g_SkinInfo->ResolveIncludes(&control);

    // ok, now check for our texture type
    TiXmlElement *pTexture = control.FirstChildElement(cTextureType);
    if (pTexture)
    {
      // found our textureType
      TiXmlNode *pNode = pTexture->FirstChild();
      if (pNode && pNode->Value()[0] != '-')
      {
        strncpy(defaultImage, pNode->Value(), sizeof(defaultImage));
        defaultImage[sizeof(defaultImage) - 1] = '\0';
        return defaultImage;
      }
    }
    return "";
  }

#ifdef ENABLE_XBMC_TRACE_API
  static thread_local TraceGuard* tlParent;

  static char** getSpacesArray(int size)
  {
    char** ret = new char*[size];
    for (int i = 0; i < size; i++)
    {
      ret[i] = new char[i + 1];

      int j;
      for (j = 0; j < i; j++)
        ret[i][j] = ' ';
      ret[i][j] = 0;
    }
    return ret;
  }

  static char** spaces = getSpacesArray(256);

  const char* TraceGuard::getSpaces() { return spaces[depth]; }

  TraceGuard::TraceGuard(const char* _function) :function(_function)
  {
    parent = tlParent;
    depth = parent == NULL ? 0 : parent->depth + 1;

    tlParent = this;

    CLog::Log(LOGDEBUG, "%sNEWADDON Entering %s", spaces[depth], function);
  }

  TraceGuard::TraceGuard() :function(NULL)
  {
    parent = tlParent;
    depth = parent == NULL ? 0 : parent->depth + 1;
    tlParent = this;
    // silent
  }

  TraceGuard::~TraceGuard()
  {
    if (function)
      CLog::Log(LOGDEBUG, "%sNEWADDON Leaving %s", spaces[depth], function);

    // need to pop the stack
    tlParent = this->parent;
  }
#endif


}
