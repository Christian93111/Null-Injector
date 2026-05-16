#include "pch.h"
#include "cApp.h"

cMain* ref;
char working_dir[1024];

wxIMPLEMENT_APP(cApp);

cApp::cApp() {}
cApp::~cApp() {}

bool cApp::OnInit() {
    char* shut_up_VS = _getcwd(working_dir, FILENAME_MAX);

	m_frame1 = new cMain();
	ref = m_frame1;
	m_frame1->Show();
	return true;
}