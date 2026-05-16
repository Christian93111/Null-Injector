#include "pch.h"
#include "ProcessListDialog.h"
#include <wx/imaglist.h>
#include <wx/filename.h>
#include <TlHelp32.h>
#include <shellapi.h>
#include <psapi.h>

wxBEGIN_EVENT_TABLE(ProcessListDialog, wxDialog)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY, ProcessListDialog::OnItemActivated)
EVT_BUTTON(wxID_OK, ProcessListDialog::OnSelectButton)
EVT_BUTTON(wxID_CANCEL, ProcessListDialog::OnCancelButton)
wxEND_EVENT_TABLE()

struct WindowData {
    wxString title;
    wxString exeName;
    HICON icon;
};

// Helper to get full path of a process by PID
wxString GetProcessFullPath(DWORD pid) {
    TCHAR path[MAX_PATH] = L"";
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess) {
        if (GetModuleFileNameEx(hProcess, NULL, path, MAX_PATH)) {
            CloseHandle(hProcess);
            return wxString(path);
        }
        CloseHandle(hProcess);
    }
    return wxEmptyString;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (!IsWindowVisible(hwnd)) {
        return TRUE;
    }
    
    TCHAR title[256];
    GetWindowText(hwnd, title, 256);
    if (wxString(title).IsEmpty()) {
        return TRUE;
    }

    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);

    wxString fullPath = GetProcessFullPath(pid);
    wxString exeName = L"Unknown";
    HICON hIcon = NULL;

    if (!fullPath.IsEmpty()) {
        wxFileName fn(fullPath);
        exeName = fn.GetFullName();
        
        SHFILEINFO sfi = { 0 };

        if (SHGetFileInfo(fullPath.t_str(), 0, &sfi, sizeof(sfi), SHGFI_ICON | SHGFI_SMALLICON)) {
            hIcon = sfi.hIcon;
        }
    }

    std::vector<WindowData>* data = (std::vector<WindowData>*)lParam;
    data->push_back({ title, exeName, hIcon });
    return TRUE;
}

ProcessListDialog::ProcessListDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, "Process List", wxDefaultPosition, wxSize(400, 500)) {
    
    SetupLayout();
    
    RefreshApplications();
    RefreshProcesses();
}

void ProcessListDialog::SetupLayout() {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    m_notebook = new wxNotebook(this, wxID_ANY);

    m_imageList = new wxImageList(16, 16);
    
    m_listApps = new wxListView(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    m_listApps->AppendColumn("Process Name", wxLIST_FORMAT_LEFT, 120);
    m_listApps->AppendColumn("Window Title", wxLIST_FORMAT_LEFT, 240);
    m_listApps->SetImageList(m_imageList, wxIMAGE_LIST_SMALL);

    m_listProcs = new wxListView(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    m_listProcs->AppendColumn("PID", wxLIST_FORMAT_LEFT, 80);
    m_listProcs->AppendColumn("Process Name", wxLIST_FORMAT_LEFT, 280);
    m_listProcs->SetImageList(m_imageList, wxIMAGE_LIST_SMALL);

    m_notebook->AddPage(m_listApps, "Applications");
    m_notebook->AddPage(m_listProcs, "Processes");

    mainSizer->Add(m_notebook, 1, wxEXPAND | wxALL, 10);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okBtn = new wxButton(this, wxID_OK, "Select", wxDefaultPosition, wxSize(100, 30));
    wxButton* cancelBtn = new wxButton(this, wxID_CANCEL, "Cancel", wxDefaultPosition, wxSize(100, 30));
    
    btnSizer->Add(okBtn, 0, wxRIGHT, 10);
    btnSizer->Add(cancelBtn, 0);
    
    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxBOTTOM, 15);

    SetSizer(mainSizer);
}

void ProcessListDialog::RefreshApplications() {
    std::vector<WindowData> windows;
    EnumWindows(EnumWindowsProc, (LPARAM)&windows);

    m_listApps->DeleteAllItems();
    for (const auto& win : windows) {
        int iconIdx = -1;
        if (win.icon) {
            wxIcon icon;
            icon.CreateFromHICON(win.icon);
            iconIdx = m_imageList->Add(icon);
            DestroyIcon(win.icon); // Clean up
        }
        
        long itemIdx = m_listApps->InsertItem(m_listApps->GetItemCount(), win.exeName, iconIdx);
        m_listApps->SetItem(itemIdx, 1, win.title);
    }
}

void ProcessListDialog::RefreshProcesses() {
    m_listProcs->DeleteAllItems();
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe = { sizeof(pe) };
        if (Process32First(hSnap, &pe)) {
            do {
                if (pe.th32ProcessID == 0) continue;
                
                wxString fullPath = GetProcessFullPath(pe.th32ProcessID);
                int iconIdx = -1;
                
                if (!fullPath.IsEmpty()) {
                    SHFILEINFO sfi = { 0 };
                    if (SHGetFileInfo(fullPath.t_str(), 0, &sfi, sizeof(sfi), SHGFI_ICON | SHGFI_SMALLICON)) {
                        wxIcon icon;
                        icon.CreateFromHICON(sfi.hIcon);
                        iconIdx = m_imageList->Add(icon);
                        DestroyIcon(sfi.hIcon);
                    }
                }

                wxString pidStr = wxString::Format("%08X", pe.th32ProcessID);
                long itemIdx = m_listProcs->InsertItem(m_listProcs->GetItemCount(), pidStr, iconIdx);
                m_listProcs->SetItem(itemIdx, 1, pe.szExeFile);
            } while (Process32Next(hSnap, &pe));
        }
        CloseHandle(hSnap);
    }
}

void ProcessListDialog::OnItemActivated(wxListEvent& event) {
    OnSelectButton(wxCommandEvent());
}

void ProcessListDialog::OnSelectButton(wxCommandEvent& event) {
    wxListView* activeList = (m_notebook->GetSelection() == 0) ? m_listApps : m_listProcs;
    long selected = activeList->GetFirstSelected();
    if (selected != -1) {
        // Always get Column 0 (which contains the .exe name for both tabs)
        m_selectedProcess = activeList->GetItemText(selected, 0);
        EndModal(wxID_OK);
    }
}

void ProcessListDialog::OnCancelButton(wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}
