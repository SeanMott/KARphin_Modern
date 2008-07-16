// Copyright (C) 2003-2008 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#ifndef _GLOBALS_H
#define _GLOBALS_H

enum
{
	IDM_LOADSTATE = 200,
	IDM_SAVESTATE,
	IDM_PLAY,
	IDM_STOP,
	IDM_BROWSE,
	IDM_PLUGIN_OPTIONS,
	IDM_CONFIG_GFX_PLUGIN,
	IDM_CONFIG_DSP_PLUGIN,
	IDM_CONFIG_PAD_PLUGIN,
	IDM_TOGGLE_FULLSCREEN,
	IDM_TOGGLE_DUALCORE,
	IDM_NOTIFYMAPLOADED,
	IDM_UPDATELOGDISPLAY,
	IDM_UPDATEDISASMDIALOG,
	IDM_UPDATEGUI,
	IDM_UPDATESTATUSBAR,
	IDM_HOST_MESSAGE,
	IDM_BOOTING_STARTED,
	IDM_BOOTING_ENDED,
	IDM_ABOUT,
	ID_TOOLBAR = 500,
	LIST_CTRL = 1000
};

#define wxUSE_XPM_IN_MSW 1
#define USE_XPM_BITMAPS 1

// For compilers that support precompilation, includes "wx/wx.h".
//#include "wx/wxprec.h"

//#ifndef WX_PRECOMP
#include "wx/wx.h"
//#endif

#include "wx/toolbar.h"
#include "wx/log.h"
#include "wx/image.h"
#include "wx/aboutdlg.h"
#include "wx/filedlg.h"
#include "wx/spinctrl.h"
#include "wx/srchctrl.h"
#include "wx/listctrl.h"
#include "wx/progdlg.h"
#include "wx/imagpng.h"
#include "wx/button.h"
#include "wx/stattext.h"
#include "wx/choice.h"
#include "wx/cmdline.h"
#include "wx/busyinfo.h"

// define this to use XPMs everywhere (by default, BMPs are used under Win)
// BMPs use less space, but aren't compiled into the executable on other platforms

#if USE_XPM_BITMAPS && defined (__WXMSW__) && !wxUSE_XPM_IN_MSW
#error You need to enable XPM support to use XPM bitmaps with toolbar!
#endif // USE_XPM_BITMAPS


//
// custom message macro
//

#define EVT_HOST_COMMAND(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(\
		wxEVT_HOST_COMMAND, id, wxID_ANY, \
		(wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent(wxCommandEventFunction, &fn), \
		(wxObject*) NULL \
		),

extern const wxEventType wxEVT_HOST_COMMAND;

#endif
