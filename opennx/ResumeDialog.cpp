// $Id$
//
// Copyright (C) 2006 The OpenNX team
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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ResumeDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "ResumeDialog.h"

////@begin XPM images

////@end XPM images

/*!
 * ResumeDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ResumeDialog, wxDialog )

/*!
 * ResumeDialog event table definition
 */

BEGIN_EVENT_TABLE( ResumeDialog, wxDialog )

////@begin ResumeDialog event table entries
    EVT_LIST_ITEM_SELECTED( XRCID("ID_LISTCTRL_SESSIONS"), ResumeDialog::OnListctrlSessionsSelected )
    EVT_LIST_ITEM_DESELECTED( XRCID("ID_LISTCTRL_SESSIONS"), ResumeDialog::OnListctrlSessionsDeselected )

    EVT_BUTTON( XRCID("ID_BUTTON_TAKEOVER"), ResumeDialog::OnButtonTakeoverClick )

    EVT_BUTTON( XRCID("ID_BUTTON_RESUME"), ResumeDialog::OnButtonResumeClick )

    EVT_BUTTON( wxID_NEW, ResumeDialog::OnNEWClick )

    EVT_BUTTON( wxID_ABORT, ResumeDialog::OnABORTClick )

////@end ResumeDialog event table entries

END_EVENT_TABLE()

/*!
 * ResumeDialog constructors
 */

ResumeDialog::ResumeDialog( )
{
    Init();
}

ResumeDialog::ResumeDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * ResumeDialog creator
 */

bool ResumeDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ResumeDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    SetParent(parent);
    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ResumeDialog creation
    return true;
}

/*!
 * Member initialisation 
 */

void ResumeDialog::Init()
{
////@begin ResumeDialog member initialisation
    m_pCtrlSessions = NULL;
////@end ResumeDialog member initialisation
}
/*!
 * Control creation for ResumeDialog
 */

void ResumeDialog::CreateControls()
{    
////@begin ResumeDialog content construction
    if (!wxXmlResource::Get()->LoadDialog(this, GetParent(), _T("ID_RESUMEDIALOG")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    m_pCtrlSessions = XRCCTRL(*this, "ID_LISTCTRL_SESSIONS", wxListCtrl);
////@end ResumeDialog content construction

    // Create custom windows not generated automatically here.
////@begin ResumeDialog content initialisation
////@end ResumeDialog content initialisation
//
    m_pCtrlSessions->InsertColumn(0, _("Name"));
    m_pCtrlSessions->InsertColumn(1, _("State"));
    m_pCtrlSessions->InsertColumn(2, _("Type"));
    m_pCtrlSessions->InsertColumn(3, _("Screen"));
    m_pCtrlSessions->InsertColumn(4, _("Port"));
    m_pCtrlSessions->InsertColumn(5, _("Options"));
    m_pCtrlSessions->InsertColumn(6, _("Session ID"));
    for (int i = 0; i < m_pCtrlSessions->GetColumnCount(); i++) 
        m_pCtrlSessions->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
}

void
ResumeDialog::AddSession(const wxString& name, const wxString& state, const wxString& type,
        const wxString& size, const wxString& colors,
        const wxString& port, const wxString& opts, const wxString& id)
{
    long idx = m_pCtrlSessions->InsertItem(0, name, 0);
    m_pCtrlSessions->SetItem(idx, 1, state);
    m_pCtrlSessions->SetItem(idx, 2, type);
    m_pCtrlSessions->SetItem(idx, 3, size + wxT("x") + colors);
    m_pCtrlSessions->SetItem(idx, 4, port);
    m_pCtrlSessions->SetItem(idx, 5, opts);
    m_pCtrlSessions->SetItem(idx, 6, id);
    long lPort;
    port.ToLong(&lPort);
    m_pCtrlSessions->SetItemData(idx, lPort);
}

/*!
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL_SESSIONS
 */

void ResumeDialog::OnListctrlSessionsSelected( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL_SESSIONS in ResumeDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL_SESSIONS in ResumeDialog. 
}

/*!
 * wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_LISTCTRL_SESSIONS
 */

void ResumeDialog::OnListctrlSessionsDeselected( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_LISTCTRL_SESSIONS in ResumeDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_LISTCTRL_SESSIONS in ResumeDialog. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TAKEOVER
 */

void ResumeDialog::OnButtonTakeoverClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TAKEOVER in ResumeDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_TAKEOVER in ResumeDialog. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESUME
 */

void ResumeDialog::OnButtonResumeClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESUME in ResumeDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_RESUME in ResumeDialog. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_NEW
 */

void ResumeDialog::OnNEWClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_NEW in ResumeDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_NEW in ResumeDialog. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ABORT
 */

void ResumeDialog::OnABORTClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ABORT in ResumeDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ABORT in ResumeDialog. 
}

/*!
 * Should we show tooltips?
 */

bool ResumeDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ResumeDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ResumeDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ResumeDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ResumeDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ResumeDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ResumeDialog icon retrieval
}