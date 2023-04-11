#include <Logging.h>
SetLogFilePath("DLL_Injector_UI.log");
//

#include <DLL_Injection/DLL_Injector.h>
#include <SimpleIni.h>
#include <StringFormatting.h>
#include <UserInterface.h>

#include <filesystem>

constexpr auto IniFileName = "dll_injector.ini";

struct InjectableDLL {
    std::string Name;
    std::string DLL;
    std::string EXE;
};

std::vector<InjectableDLL> GetDLLs() {
    std::vector<InjectableDLL> DLLs;

    CSimpleIniA ini;
    auto        iniLoadError = ini.LoadFile(IniFileName);

    if (iniLoadError < 0) return DLLs;

    CSimpleIniA::TNamesDepend DLLsNames;
    ini.GetAllSections(DLLsNames);

    for (auto& DLLName : DLLsNames) {
        InjectableDLL DLL;
        DLL.Name = DLLName.pItem;
        DLL.DLL  = ini.GetValue(DLLName.pItem, "DLL", "");
        DLL.EXE  = ini.GetValue(DLLName.pItem, "EXE", "");
        DLLs.push_back(DLL);
    }

    return DLLs;
}

std::vector<InjectableDLL> DLLs;

void ClickButtonAndInjectDLL(UserInterface::UIButton* button) {
    auto dllName = button->GetText();
    for (auto& DLL : DLLs) {
        if (DLL.Name == dllName) {
            button->SetText(string_format("Injecting {} ...", dllName).c_str());
            DLL_Injector::InjectDLL(DLL.EXE, DLL.DLL);
            button->SetText(string_format("Injected {}", dllName).c_str());
            break;
        }
    }
}

UI_Main {
    DLLs = GetDLLs();

    auto* ui     = UserInterface::GetAPI();
    auto* app    = ui->GetApplication();
    auto* window = app->NewWindow("Choose DLL to Inject");

    window->SetWidth(500);
    window->SetHeight(31 * DLLs.size());

    for (auto& injectableDLL : DLLs)
        window->AddButton(injectableDLL.Name.c_str(), ClickButtonAndInjectDLL);

    window->Show();
    app->Run();
}