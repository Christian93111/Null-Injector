target_sources(NullInjector PRIVATE 
    cApp.cpp        
    cMain.cpp       
    FixFilePerms.cpp
    inject.cpp
    pch.cpp
    taskBarIcon.cpp
    ProcessListDialog.cpp
    icon.rc
)

target_precompile_headers(NullInjector PRIVATE "pch.h")