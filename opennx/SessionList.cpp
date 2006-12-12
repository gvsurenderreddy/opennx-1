// $Id$
//
// Copyright (C) 2006 The OpenNX Team
// Author: Fritz Elfert
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "SessionList.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "SessionList.h"
#include "MySession.h"
#include "LogDialog.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/listctrl.h>

#ifdef MYTRACETAG
# undef MYTRACETAG
#endif
#define MYTRACETAG wxT("SessionList")

class SessionTraverser : public wxDirTraverser
{
    public:
        SessionTraverser(wxArrayString& dirs) : m_dirs(dirs) { }

        virtual wxDirTraverseResult OnFile(const wxString& WXUNUSED(filename))
        {
            return wxDIR_CONTINUE;
        }

        virtual wxDirTraverseResult OnDir(const wxString& dirpath)
        {
            wxString name = wxFileName::FileName(dirpath).GetFullName();
            m_dirs.Add(name);
            return wxDIR_IGNORE;
        }

    private:
        wxArrayString& m_dirs;
};

WX_DECLARE_STRING_HASH_MAP(MySession, SessionHash);

    SessionList::SessionList(wxString dir, wxListCtrl* ctrl)
    : wxThreadHelper()
    , m_dirName(dir)
    , m_dir(NULL)
    , m_reValid(false)
                      , m_listctrl(ctrl)
{
    m_sessions = new SessionHash();
    m_re = new wxRegEx();
    if (m_re->Compile(wxT("(([TF])-)?([SC])-(.*)-([[:digit:]]+)-([[:xdigit:]]{32})$"), wxRE_ADVANCED))
        m_reValid = true;
    if (m_reValid && (!m_dirName.IsEmpty())) {
        Create();
        m_thread->Run();
    }
}

SessionList::~SessionList()
{
    m_thread->Delete();
    while (m_thread->IsRunning())
        wxThread::Sleep(100);
    if (m_dir != NULL)
        delete m_dir;
    delete m_re;
    m_sessions->clear();
    delete m_sessions;
}

void SessionList::SetDir(wxString dir)
{
    m_dirName = dir;
    if (m_reValid && (!m_dirName.IsEmpty())) {
        if (m_thread == NULL) {
            Create();
            m_thread->Run();
        } else {
            m_csDir.Enter();
            if (m_dir)
                delete m_dir;
            m_dir = NULL;
            m_csDir.Leave();
        }
    }
}

void SessionList::ScanDir()
{
    wxCriticalSectionLocker dlocker(m_csDir);

    if (m_dir == NULL) {
        if (m_dirName.IsEmpty())
            return;
        if (!wxDir::Exists(m_dirName)) {
            m_dirName = wxT("");
            return;
        }
        m_dir = new wxDir(m_dirName);
    }
    ::wxLogTrace(MYTRACETAG, wxT("SessionList: scanning %s"), m_dirName.c_str());

    // get the names of all session directories
    wxArrayString sdirs;
    SessionTraverser traverser(sdirs);
    m_dir->Traverse(traverser);
    size_t cnt = sdirs.GetCount();
    size_t i;

    // Format of session dir name:
    //
    // ([TF]-)?[SC]-(.*)-[:digit:]+-[:xdigit:]{32}
    //
    // 1. element is session status
    // "T-" resp. "F-" stands for terminated or failed respectively.
    // If it is missing, the session is undefined (probably running).
    //
    // 2. element is session type
    // S- resp. C- stands for server resp. client session type
    //
    // 3. element is host name of the NX server
    // 4. element is port name
    // 5. element is an md5sum. (//FE: For what data?)
    //

    bool changed = false;
    SessionHash::iterator it;
    for (it = m_sessions->begin(); it != m_sessions->end(); ++it)
        it->second.bSetTouched(false);

    for (i = 0; i < cnt; i++) {
        wxString tmp = sdirs[i];
        if (m_re->Matches(tmp)) {
            wxString md5 = m_re->GetMatch(tmp,6);

            it = m_sessions->find(md5);
            if (it == m_sessions->end()) {
                // New session found
                long port;

                m_re->GetMatch(tmp,5).ToLong(&port);
                ::wxLogTrace(MYTRACETAG, wxT("State='%s', Type='%s', Host='%s', Port=%d, MD5='%s'"),
                        m_re->GetMatch(tmp,2).c_str(), m_re->GetMatch(tmp,3).c_str(),
                        m_re->GetMatch(tmp,4).c_str(), port, md5.c_str());
                // Create new hash entry
                MySession s(m_dirName + wxFileName::GetPathSeparator() + tmp,
                        m_re->GetMatch(tmp,2), m_re->GetMatch(tmp,3), m_re->GetMatch(tmp,4), port, md5);
                (*m_sessions)[md5] = s;
                if (m_listctrl != NULL) {
                    long idx;

                    wxMutexGuiEnter();

                    // Hostname
                    idx = m_listctrl->InsertItem(0, s.sGetHost(), 0);
                    // Port
                    m_listctrl->SetItem(idx, 1, s.sGetPort());
                    // Session ID
                    m_listctrl->SetItem(idx, 2, s.sGetMd5());
                    // Creation Time
                    m_listctrl->SetItem(idx, 3, s.sGetCreationTime());
                    // PID
                    m_listctrl->SetItem(idx, 4, s.sGetPID());
                    // Status
                    switch (s.eGetSessionStatus()) {
                        case MySession::Terminated:
                            m_listctrl->SetItem(idx, 5, _("terminated"));
                            break;
                        case MySession::Failed:
                            m_listctrl->SetItem(idx, 5, _("failed"));
                            break;
                        case MySession::Running:
                            m_listctrl->SetItem(idx, 5, _("running"));
                            break;
                        default:
                            m_listctrl->SetItem(idx, 5, _("unknown"));
                            break;
                    }
                    // Type
                    m_listctrl->SetItem(idx, 6,
                            (s.eGetSessionType() == MySession::Server) ? _("Server") : _("Client"));

                    // Data
                    //                    m_listctrl->SetItemData(idx, (long)(*m_sessions)[md5].m_md5.c_str());
                    m_listctrl->SetItemData(idx, (long)s.sGetMd5().c_str());
                    wxMutexGuiLeave();

                    changed = true;
                }
            } else {
                // Existing session found, mark it
                it->second.bSetTouched(true);
            }
        }
    }
    bool finished = false;

    while (!finished) {
        finished = true;
        for (it = m_sessions->begin(); it != m_sessions->end(); ++it) {
            if (!it->second.bGetTouched()) {
                ::wxLogTrace(MYTRACETAG, wxT("Session '%s' disappeared"), it->second.sGetMd5().c_str());
                finished = false;
                if (m_listctrl != NULL) {
                    long item;

                    wxMutexGuiEnter();

                    item = m_listctrl->FindItem(-1, it->second.sGetMd5().c_str());
                    if (item != -1) {
                        m_listctrl->DeleteItem(item);
                        changed = true;
                    }

                    wxMutexGuiLeave();

                }
                m_sessions->erase(it);
                break;
            }
        }
    }
    if (changed && (m_listctrl != NULL)) {

        wxMutexGuiEnter();

        for (int i = 0; i < m_listctrl->GetColumnCount(); i++) 
            m_listctrl->SetColumnWidth(i, wxLIST_AUTOSIZE);

        wxMutexGuiLeave();
    }

    ::wxLogTrace(MYTRACETAG, wxT("SessionList: Now %d sessions"), m_sessions->size());
}

void SessionList::ShowSesssionLog(int idx)
{
    SessionHash::iterator it;
    it = m_sessions->find(wxString((wxChar *)m_listctrl->GetItemData(idx)));
    if (it == m_sessions->end()) {

    } else {
        LogDialog d(NULL);
        d.ReadLogFile(it->second.sGetDir() + wxFileName::GetPathSeparator() + wxT("session"));
        d.ShowModal();
    }
}

wxThread::ExitCode SessionList::Entry()
{
    int cnt = 0;
    while (!m_thread->TestDestroy()) {
        if (cnt-- == 0) {
            ScanDir();
            cnt = 20;
        }
        wxThread::Sleep(100);
    }
    return 0;
}
