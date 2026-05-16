#include "pch.h"
#include "cMain.h"
#include "FixFilePerms.h"
#include "inject.h"
#include "cApp.h"
#include "icon/icon.xpm"
#include <TlHelp32.h>
#include <wx/filename.h>
#include <vector>
#include <set>

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(101, OnInjectButton)
EVT_BUTTON(102, OnHideButton)
EVT_BUTTON(103, OnSelectButton)
EVT_BUTTON(105, OnSearchButton)
wxEND_EVENT_TABLE();

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Null Injector", wxDefaultPosition, wxSize(450, 260), wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN) {
    
    // Try to load the ico
    wxIcon icon;
    if (wxFileExists("icon/icon.ico")) {
        icon.LoadFile("icon/icon.ico", wxBITMAP_TYPE_ICO);
    } else {
        icon = wxIcon(icon_xpm);
    }
    
    this->SetIcon(icon);

    m_mainPanel = new wxPanel(this, wxID_ANY);

    ApplyModernStyling();
    SetupLayout();
    
    m_taskBarControl.SetIcon(icon, "Double Click to show Null Injector");

    m_notification = new wxNotificationMessage("Null Injector", "is now hidden in your system tray", this, 0);
    m_notification->UseTaskBarIcon(&m_taskBarControl);
    
    m_openDialog = new wxFileDialog(this, "Select the .dll file", working_dir, "Null.dll" ,"Dynamic link library (*.dll)|*.dll", wxFD_OPEN);
}

void cMain::ApplyModernStyling() {
    wxFont titleFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    m_btnInject = new wxButton(m_mainPanel, 101, "Inject", wxDefaultPosition, wxSize(150, 40));
    m_btnInject->SetFont(titleFont);

    m_btnHide = new wxButton(m_mainPanel, 102, "Hide on Tray", wxDefaultPosition, wxSize(120, 25));
    m_btnSelect = new wxButton(m_mainPanel, 103, "Select", wxDefaultPosition, wxSize(80, 25));
    m_btnSearch = new wxButton(m_mainPanel, 105, "Search", wxDefaultPosition, wxSize(70, 25));

    m_txtName = new wxTextCtrl(m_mainPanel, wxID_ANY, "Click \"Search\" to choose a EXE file", wxDefaultPosition, wxSize(200, 23), wxTE_READONLY);
    m_txtPath = new wxTextCtrl(m_mainPanel, wxID_ANY, "Click \"Select\" to choose a DLL file", wxDefaultPosition, wxSize(250, 23), wxTE_READONLY);

    CreateStatusBar(1);
    SetStatusText("Version: v1.0 | Made by: Fan2K", 0);
}

void cMain::SetupLayout() {
    m_mainSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 8, 8);
    gridSizer->Add(new wxStaticText(m_mainPanel, wxID_ANY, "Process Name:"), 0, wxALIGN_CENTER_VERTICAL);
    
    wxBoxSizer* procInputSizer = new wxBoxSizer(wxHORIZONTAL);
    procInputSizer->Add(m_txtName, 1, wxEXPAND | wxRIGHT, 5);
    procInputSizer->Add(m_btnSearch, 0);
    gridSizer->Add(procInputSizer, 0, wxEXPAND);

    wxBoxSizer* dllSizer = new wxBoxSizer(wxHORIZONTAL);
    dllSizer->Add(m_txtPath, 1, wxEXPAND | wxRIGHT, 5);
    dllSizer->Add(m_btnSelect, 0);

    topSizer->Add(gridSizer, 0, wxALL | wxEXPAND, 15);
    topSizer->Add(dllSizer, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 15);
    topSizer->Add(m_btnInject, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
    topSizer->Add(m_btnHide, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    m_mainPanel->SetSizer(topSizer);
    m_mainSizer->Add(m_mainPanel, 1, wxEXPAND);
    this->SetSizer(m_mainSizer);
}

cMain::~cMain() {
    m_openDialog->Destroy();
    m_notification->Close();
    delete(m_notification);
}

void cMain::OnInjectButton(wxCommandEvent& evt) {
    cMain::OnInjectButtonExecute(evt, this);
}

void cMain::OnInjectButtonExecute(wxCommandEvent& evt, cMain* ref) {
    std::string debug;
    DWORD procId = GetProcId(ref->m_txtName->GetValue().mb_str());

    if (procId == 0) {
        debug = std::string("Process not found: ") + std::string(ref->m_txtName->GetValue().mb_str());
        ref->SetStatusText(debug, 0);
        return;
    }

    wxString wxStrPath = ref->m_txtPath->GetValue();
    std::wstring wStrPath = wxStrPath.ToStdWstring();
    std::ifstream test(wStrPath.c_str());
    if (!test) {
        debug = "Invalid DLL path!";
        ref->SetStatusText(debug, 0);
        return;
    }

    SetAccessControl(wStrPath, L"S-1-15-2-1");
    performInjection(procId, wStrPath.c_str());
    debug = std::string("Successfully Injected into PID: ") + std::to_string(procId);
    ref->SetStatusText(debug, 0);

    evt.Skip();
}

void cMain::OnHideButton(wxCommandEvent& evt) {
    m_notification->Show();
    this->Hide();
    evt.Skip();
}

void cMain::OnSelectButton(wxCommandEvent& evt) {
    if (m_openDialog->ShowModal() == wxID_OK) {
        m_txtPath->SetValue(m_openDialog->GetPath());
    }
    evt.Skip();
}

void cMain::OnSearchButton(wxCommandEvent& evt) {
    ProcessListDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK) {
        m_txtName->SetValue(dialog.GetSelectedProcess());
    }
    evt.Skip();
}