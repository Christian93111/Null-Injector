#pragma once
#include "taskBarIcon.h"
#include "ProcessListDialog.h"

class cMain : public wxFrame {
public:
	cMain();
	~cMain();

private:
	void SetupLayout();
	void ApplyModernStyling();

public:
	wxPanel *m_mainPanel = nullptr;
	wxButton *m_btnInject = nullptr;
	wxButton *m_btnHide = nullptr;
	wxButton *m_btnSelect = nullptr;
	wxButton *m_btnSearch = nullptr;
	wxTextCtrl *m_txtName = nullptr;
	wxTextCtrl *m_txtDelay = nullptr;
	wxTextCtrl *m_txtPath = nullptr;
	wxBoxSizer *m_mainSizer = nullptr;

	wxFileDialog *m_openDialog = nullptr;
	taskBarIcon m_taskBarControl;
	wxNotificationMessage *m_notification = nullptr;

public:
	static void OnInjectButtonExecute(wxCommandEvent &evt, cMain *ref);
	void OnInjectButton(wxCommandEvent &evt);
	void OnHideButton(wxCommandEvent &evt);
	void OnSelectButton(wxCommandEvent &evt);
	void OnSearchButton(wxCommandEvent &evt);

	void onTaskBarDClick(wxCommandEvent &evt);

	wxDECLARE_EVENT_TABLE();

public:
	friend taskBarIcon::taskBarIcon();
};