#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/imaglist.h>

class ProcessListDialog : public wxDialog {
public:
    ProcessListDialog(wxWindow* parent);

    wxString GetSelectedProcess() const { return m_selectedProcess; }

private:
    void SetupLayout();
    void RefreshApplications();
    void RefreshProcesses();
    
    void OnItemActivated(wxListEvent& event);
    void OnSelectButton(wxCommandEvent& event);
    void OnCancelButton(wxCommandEvent& event);

    wxNotebook* m_notebook;
    wxListView* m_listApps;
    wxListView* m_listProcs;
    wxImageList* m_imageList;

    wxString m_selectedProcess;

    wxDECLARE_EVENT_TABLE();
};
