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

#ifdef _WIN32
#include <windows.h>
#else

#endif

#include "Thread.h"
#include "Timer.H"

#include "Console.h"
#include "Core.h"
#include "CoreTiming.h"
#include "Boot/Boot.h"
#include "PatchEngine.h"

#include "HW/Memmap.h"
#include "HW/PeripheralInterface.h"
#include "HW/GPFifo.h"
#include "HW/CPU.h"
#include "HW/CPUCompare.h"
#include "HW/HW.h"
#include "HW/DSP.h"
#include "HW/GPFifo.h"
#include "HW/AudioInterface.h"
#include "HW/VideoInterface.h"
#include "HW/CommandProcessor.h"
#include "HW/PixelEngine.h"
#include "HW/SystemTimers.h"

#include "PowerPC/PowerPC.h"

#include "Plugins/Plugin_Video.h"
#include "Plugins/Plugin_PAD.h"
#include "Plugins/Plugin_DSP.h"

#include "MemTools.h"
#include "Host.h"
#include "LogManager.h"

#ifndef _WIN32
#define WINAPI
#endif

// The idea behind the recent restructure is to fix various stupid problems.
// glXMakeCurrent/ wglMakeCurrent takes a context and makes it current on the current thread.
// So it's fine to init ogl on one thread, and then make it current and start blasting on another.

namespace Core
{
// forwarding
//void Callback_VideoRequestWindowSize(int _iWidth, int _iHeight, BOOL _bFullscreen);
void Callback_VideoLog(const TCHAR* _szMessage, BOOL _bDoBreak);
void Callback_VideoCopiedToXFB();
void Callback_DSPLog(const TCHAR* _szMessage);
void Callback_DSPInterrupt();
void Callback_DVDLog(const TCHAR* _szMessage);
void Callback_DVDSetStatusbar(TCHAR* _szMessage);
void Callback_PADLog(const TCHAR* _szMessage);
TPeekMessages Callback_PeekMessages = NULL;
TUpdateFPSDisplay g_pUpdateFPSDisplay = NULL;

#ifdef _WIN32
DWORD WINAPI EmuThread(void *pArg);
#else
void *EmuThread(void *pArg);
#endif
void Stop();

bool g_bHwInit = false;
HWND g_pWindowHandle = NULL;
Common::Thread* g_pThread = NULL;

SCoreStartupParameter g_CoreStartupParameter; //uck

Common::Event emuThreadGoing;

// Called from GUI thread
bool Init(const SCoreStartupParameter _CoreParameter)
{
	if (g_pThread != NULL)
		return false;

	LogManager::Init();	
	Host_SetWaitCursor(true);

	g_CoreStartupParameter = _CoreParameter;
	// Init the Hardware

	// start the thread again
	_dbg_assert_(HLE, g_pThread == NULL);

	// load plugins
	if (!PluginDSP::LoadPlugin(g_CoreStartupParameter.m_strDSPPlugin.c_str())) {
		PanicAlert("Failed to load DSP plugin %s", g_CoreStartupParameter.m_strDSPPlugin.c_str());
		return false;
	}
	if (!PluginPAD::LoadPlugin(g_CoreStartupParameter.m_strPadPlugin.c_str())) {
		PanicAlert("Failed to load PAD plugin %s", g_CoreStartupParameter.m_strPadPlugin.c_str());
		return false;
	}
	if (!PluginVideo::LoadPlugin(g_CoreStartupParameter.m_strVideoPlugin.c_str())) {
		PanicAlert("Failed to load video plugin %s", g_CoreStartupParameter.m_strVideoPlugin.c_str());
		return false;
	}

#ifdef _WIN32
	PluginDSP::DllDebugger((HWND)_CoreParameter.hMainWindow);
#endif

	emuThreadGoing.Init();

	g_pThread = new Common::Thread(EmuThread, (void*)&g_CoreStartupParameter);
	
	emuThreadGoing.Wait();
	emuThreadGoing.Shutdown();
	// all right ... here we go
	Host_SetWaitCursor(false);
	return true;
}

// Called from GUI thread or VI thread
void Stop() // - Hammertime!
{
	Host_SetWaitCursor(true);
	if (PowerPC::state == PowerPC::CPU_POWERDOWN)
		return;
	// stop the CPU
	PowerPC::state = PowerPC::CPU_POWERDOWN;
	
	CCPU::StepOpcode(); //kick it if it's waiting

	// The quit is to get it out of its message loop
	// Should be moved inside the plugin.
#ifdef _WIN32
	PostMessage((HWND)g_pWindowHandle, WM_QUIT, 0, 0);
#else
	// TODO(ector) : post message exit
#endif

	delete g_pThread; //Wait for emuthread to close
	Core::StopTrace();
	LogManager::Shutdown();
	g_pThread = 0;
	Host_SetWaitCursor(false);
}

THREAD_RETURN CpuThread(void *pArg)
{
    Common::SetCurrentThreadName("CPU thread");
	
	const SCoreStartupParameter& _CoreParameter = g_CoreStartupParameter;
    if (!g_CoreStartupParameter.bUseDualCore)
    {
        PluginVideo::Video_Prepare(); //wglMakeCurrent
    }

	if (_CoreParameter.bRunCompareServer)
	{
		CPUCompare::StartServer();
		PowerPC::state = PowerPC::CPU_RUNNING;
	}
	else if (_CoreParameter.bRunCompareClient)
	{
		PanicAlert("Compare Debug : Press OK when ready.");
		CPUCompare::ConnectAsClient();
	}

	Common::Thread::SetCurrentThreadAffinity(1); //Force to first core

	// Let's run under memory watch
	EMM::InstallExceptionHandler();
	// StartConsoleThread();
	CCPU::Run();

	if (_CoreParameter.bRunCompareServer || _CoreParameter.bRunCompareClient)
	{
		CPUCompare::Stop();
	}
	return 0;
}

void Callback_DebuggerBreak()
{
	CCPU::EnableStepping(true);
}

THREAD_RETURN EmuThread(void *pArg)
{
	Host_BootingStarted();

	Common::SetCurrentThreadName("Emuthread - starting");
	const SCoreStartupParameter& _CoreParameter = *(SCoreStartupParameter*)pArg;

	Common::Thread::SetCurrentThreadAffinity(2); //Force to second core
	
	LOG(OSREPORT, "Starting core = %s mode", _CoreParameter.bWii ? "Wii" : "Gamecube");
	LOG(OSREPORT, "Dualcore = %s", _CoreParameter.bUseDualCore ? "Yes" : "No");

	HW::Init();	
	
    emuThreadGoing.Set();

    // Load the VideoPlugin	
	SVideoInitialize VideoInitialize;
	VideoInitialize.pGetMemoryPointer	= Memory::GetPointer;
	VideoInitialize.pSetPEToken			= PixelEngine::SetToken;
	VideoInitialize.pSetPEFinish		= PixelEngine::SetFinish;
	VideoInitialize.pWindowHandle		= _CoreParameter.hMainWindow; //      NULL; // filled by video_initialize
	VideoInitialize.pLog				= Callback_VideoLog;
	VideoInitialize.pRequestWindowSize	= NULL; //Callback_VideoRequestWindowSize;
	VideoInitialize.pCopiedToXFB		= Callback_VideoCopiedToXFB;
	VideoInitialize.pVIRegs             = VideoInterface::m_UVIUnknownRegs;
	VideoInitialize.pPeekMessages       = NULL;
    VideoInitialize.pUpdateFPSDisplay   = NULL;
	VideoInitialize.pCPFifo             = (SCPFifoStruct*)&CommandProcessor::fifo;
	VideoInitialize.pUpdateInterrupts   = &(CommandProcessor::UpdateInterruptsFromVideoPlugin);
	VideoInitialize.pMemoryBase         = Memory::base;
	PluginVideo::Video_Initialize(&VideoInitialize);

	// Under linux, this is an X11 Display, not an HWND!
    g_pWindowHandle = (HWND)VideoInitialize.pWindowHandle;
    Callback_PeekMessages = VideoInitialize.pPeekMessages;
    g_pUpdateFPSDisplay = VideoInitialize.pUpdateFPSDisplay;

    // Load and init DSPPlugin	
	DSPInitialize dspInit;
	dspInit.hWnd = g_pWindowHandle;
	dspInit.pARAM_Read_U8 = (u8  (__cdecl *)(const u32))DSP::ReadARAM; 
	dspInit.pGetARAMPointer = DSP::GetARAMPtr;
	dspInit.pGetMemoryPointer = Memory::GetPointer;
	dspInit.pLog = Callback_DSPLog;
	dspInit.pDebuggerBreak = Callback_DebuggerBreak;
	dspInit.pGenerateDSPInterrupt = Callback_DSPInterrupt;
	dspInit.pGetAudioStreaming = AudioInterface::Callback_GetStreaming;
	PluginDSP::DSP_Initialize(dspInit);

	// Load and Init PadPlugin	
	SPADInitialize PADInitialize;
	PADInitialize.hWnd = g_pWindowHandle;
	PADInitialize.pLog = Callback_PADLog;
	PluginPAD::PAD_Initialize(PADInitialize);

	// The hardware is initialized.
	g_bHwInit = true;

	// Load GCM/DOL/ELF whatever ... we boot with the interpreter core
	PowerPC::SetCore(PowerPC::CORE_INTERPRETER);
	CBoot::BootUp(_CoreParameter);

	if( g_pUpdateFPSDisplay != NULL )
        g_pUpdateFPSDisplay("Loading...");

	// setup our core, but can't use dynarec if we are compare server
	if (_CoreParameter.bUseDynarec && !_CoreParameter.bRunCompareServer || _CoreParameter.bRunCompareClient)
		PowerPC::SetCore(PowerPC::CORE_DYNAREC);
	else
		PowerPC::SetCore(PowerPC::CORE_INTERPRETER);

    // update the window again because all stuff is initialized
    Host_UpdateDisasmDialog();
    Host_UpdateMainFrame();
	Host_BootingEnded();

	//This thread, after creating the EmuWindow, spawns a CPU thread,
	//then takes over and becomes the graphics thread

	//In single core mode, the CPU thread does the graphics. In fact, the
	//CPU thread should in this case also create the emuwindow...

	//Spawn the CPU thread
    Common::Thread *cpuThread = new Common::Thread(CpuThread, pArg);

	//////////////////////////////////////////////////////////////////////////
	// ENTER THE VIDEO THREAD LOOP
	//////////////////////////////////////////////////////////////////////////
	
	if (!Core::GetStartupParameter().bUseDualCore)
	{
		Common::SetCurrentThreadName("Idle thread");
		//TODO(ector) : investigate using GetMessage instead .. although
		//then we lose the powerdown check. ... unless powerdown sends a message :P
		while (PowerPC::state != PowerPC::CPU_POWERDOWN)
		{
			if (Callback_PeekMessages) {
				Callback_PeekMessages();
			}
			Common::SleepCurrentThread(20);
		}
	}
	else
	{
        PluginVideo::Video_Prepare(); //wglMakeCurrent
		Common::SetCurrentThreadName("Video thread");
		PluginVideo::Video_EnterLoop();
	}

	// Wait for CPU thread to exit - it should have been signaled to do so by now
	if( g_pUpdateFPSDisplay != NULL )
        g_pUpdateFPSDisplay("Stopping...");
	delete cpuThread;
	// Returns after game exited

	g_bHwInit = false;

	PluginPAD::PAD_Shutdown();
	PluginPAD::UnloadPlugin();
	PluginDSP::DSP_Shutdown();
	PluginDSP::UnloadPlugin();
	PluginVideo::Video_Shutdown();
	PluginVideo::UnloadPlugin();

	HW::Shutdown();

	LOG(MASTER_LOG,"EmuThread exited");
	//The CPU should return when a game is stopped and cleanup should be done here, 
	//so we can restart the plugins (or load new ones) for the next game

    Host_UpdateMainFrame();
	return 0;
}

bool SetState(EState _State)
{
	switch(_State)
	{
	case CORE_UNINITIALIZED:
        Stop();
		break;

	case CORE_PAUSE:
		CCPU::EnableStepping(true);
		break;

	case CORE_RUN:
		CCPU::EnableStepping(false);
		break;

	default:
		return false;
	}

	return true;
}

EState GetState()
{
	if (g_bHwInit)
	{
		if (CCPU::IsStepping())
			return CORE_PAUSE;
		else
			return CORE_RUN;
	}
	return CORE_UNINITIALIZED;
}

const SCoreStartupParameter& GetStartupParameter()
{
	return g_CoreStartupParameter;
}

bool MakeScreenshot(const std::string& _rFilename)
{
	bool bResult = false;
	if (PluginVideo::IsLoaded())
	{
		TCHAR szTmpFilename[MAX_PATH];
		strcpy(szTmpFilename, _rFilename.c_str());
		bResult = PluginVideo::Video_Screenshot(szTmpFilename);
	}
	return bResult;
}

void* GetWindowHandle()
{
    return g_pWindowHandle;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Callbacks for plugins / engine ---
/////////////////////////////////////////////////////////////////////////////////////////////////////

// __________________________________________________________________________________________________
// Callback_VideoLog
// WARNING - THIS IS EXECUTED FROM VIDEO THREAD
void Callback_VideoLog(const TCHAR *_szMessage, BOOL _bDoBreak)
{
	LOG(VIDEO, _szMessage);
}

// __________________________________________________________________________________________________
// Callback_VideoCopiedToXFB
// WARNING - THIS IS EXECUTED FROM VIDEO THREAD
// We do not touch anything outside this function here
void Callback_VideoCopiedToXFB()
{ 
	//count FPS
	static Common::Timer Timer;
	static u32 frames = 0;
	static u64 ticks = 0;
	static u64 idleTicks = 0;
	frames++;

	if (Timer.GetTimeDifference() >= 1000)
	{
		u64 newTicks = CoreTiming::GetTicks();
		u64 newIdleTicks = CoreTiming::GetIdleTicks();

		s64 diff = (newTicks - ticks)/1000000;
		s64 idleDiff = (newIdleTicks - idleTicks)/1000000;

		ticks = newTicks;
		idleTicks = newIdleTicks;

		float t = (float)(Timer.GetTimeDifference()) / 1000.f;
		char isoName[64];
        isoName[0] = 0x00;
		char temp[256];
		sprintf(temp, "%s - FPS: %8.2f - %s - %i MHz (%i real, %i idle skipped) out of %i MHz", 
			isoName, (float)frames / t, 
			g_CoreStartupParameter.bUseDynarec ? "JIT" : "Interpreter", 
			(int)(diff),
			(int)(diff-idleDiff),
			(int)(idleDiff),
			SystemTimers::GetTicksPerSecond()/1000000);
		
		if( g_pUpdateFPSDisplay != NULL )
            g_pUpdateFPSDisplay(temp);

		Host_UpdateStatusBar(temp);


		frames = 0;
        Timer.Update();
	}
	PatchEngine_ApplyFramePatches();
}

// __________________________________________________________________________________________________
// Callback_DSPLog
// WARNING - THIS MAY EXECUTED FROM DSP THREAD
void Callback_DSPLog(const TCHAR* _szMessage)
{
	LOG(AUDIO, _szMessage);
}

// __________________________________________________________________________________________________
// Callback_DSPInterrupt
// WARNING - THIS MAY EXECUTED FROM DSP THREAD
void Callback_DSPInterrupt()
{
	DSP::GenerateDSPInterruptFromPlugin(DSP::INT_DSP);
}

// __________________________________________________________________________________________________
// Callback_DVDLog
//
void Callback_DVDLog(TCHAR* _szMessage)
{
	LOG(DVDINTERFACE, _szMessage);
}

// __________________________________________________________________________________________________
// Callback_DVDSetStatusbar
//
void Callback_DVDSetStatusbar(TCHAR* _szMessage)
{
	//Todo: PostMessage to main window to set the string
//	strDVDMessage = _szMessage;
//	if (g_CoreStartupParameter.m_StatusUpdate != NULL) 
//		g_CoreStartupParameter.m_StatusUpdate();
}

// __________________________________________________________________________________________________
// Callback_PADLog
//
void Callback_PADLog(const TCHAR* _szMessage)
{
	LOG(SERIALINTERFACE, _szMessage);
}

} // end of namespace Core
