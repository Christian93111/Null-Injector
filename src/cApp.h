#pragma once

#include "cMain.h"

class cApp : public wxApp
{
public:
	cApp();
	~cApp();

	cMain* m_frame1;

public:
	virtual bool OnInit();
};

extern cMain* ref;
extern char working_dir[1024];