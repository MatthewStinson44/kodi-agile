#pragma once

/*
 *      Copyright (C) 2005-2015 Team Kodi
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
 *  along with Kodi; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

/*
  Common data structures shared between Kodi and Kodi's visualisations
 */

#include "AddonBase.h"

#include <cstddef>

extern "C"
{
  struct VIS_INFO
  {
    int bWantsFreq;
    int iSyncDelay;
  };

  enum VIS_ACTION
  { 
    VIS_ACTION_NONE = 0,
    VIS_ACTION_NEXT_PRESET,
    VIS_ACTION_PREV_PRESET,
    VIS_ACTION_LOAD_PRESET,
    VIS_ACTION_RANDOM_PRESET,
    VIS_ACTION_LOCK_PRESET,
    VIS_ACTION_RATE_PRESET_PLUS,
    VIS_ACTION_RATE_PRESET_MINUS,
    VIS_ACTION_UPDATE_ALBUMART,
    VIS_ACTION_UPDATE_TRACK
  };

  class VisTrack
  {
  public:
    VisTrack()
    {
      title = artist = album = albumArtist = NULL;
      genre = comment = lyrics = reserved1 = reserved2 = NULL;
      trackNumber = discNumber = duration = year = 0;
      rating = 0;
      reserved3 = reserved4 = 0;
    }

    const char *title;
    const char *artist;
    const char *album;
    const char *albumArtist;
    const char *genre;
    const char *comment;
    const char *lyrics;
    const char *reserved1;
    const char *reserved2;

    int        trackNumber;
    int        discNumber;
    int        duration;
    int        year;
    char       rating;
    int        reserved3;
    int        reserved4;
  };

typedef struct AddonProps_Visualisation
{
  void *device;
  int x;
  int y;
  int width;
  int height;
  float pixelRatio;
  const char *name;
  const char *presets;
  const char *profile;
  const char *submodule;
} AddonProps_Visualisation;
  
typedef struct AddonToKodiFuncTable_Visualisation
{
  KODI_HANDLE kodiInstance;
} AddonToKodiFuncTable_Visualisation;

typedef struct KodiToAddonFuncTable_Visualisation
{
  bool (__cdecl* Start)(void* addonInstance, int iChannels, int iSamplesPerSec, int iBitsPerSample, const char* szSongName);
  void (__cdecl* Stop)(void* addonInstance);
  void (__cdecl* AudioData)(void* addonInstance, const float* pAudioData, int iAudioDataLength, float *pFreqData, int iFreqDataLength);
  void (__cdecl* Render) (void* addonInstance);
  void (__cdecl* GetInfo)(void* addonInstance, VIS_INFO *info);
  bool (__cdecl* OnAction)(void* addonInstance, long flags, const void *param);
  int (__cdecl* HasPresets)(void* addonInstance);
  unsigned int (__cdecl *GetPresets)(void* addonInstance, char ***presets);
  unsigned int (__cdecl *GetPreset)(void* addonInstance);
  unsigned int (__cdecl *GetSubModules)(void* addonInstance, char ***modules);
  bool (__cdecl* IsLocked)(void* addonInstance);
} KodiToAddonFuncTable_Visualisation;

typedef struct AddonInstance_Visualisation
{
  AddonProps_Visualisation props;
  AddonToKodiFuncTable_Visualisation toKodi;
  KodiToAddonFuncTable_Visualisation toAddon;
} AddonInstance_Visualisation;

}

