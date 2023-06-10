#ifndef WXTERMINALCOLOURHANDLER_H
#define WXTERMINALCOLOURHANDLER_H

#include "clAnsiEscapeCodeHandler.hpp"

#include <map>
#include <unordered_map>
#include <vector>
#include <wx/textctrl.h>

class TextView;
class wxTerminalCtrl;
class wxTerminalColourHandler : public wxEvtHandler
{
    TextView* m_ctrl = nullptr;
    wxTextAttr m_defaultAttr;
    wxString m_title;

protected:
    void SetCaretEnd();

protected:
    void Append(const std::string& buffer);
    void Clear();

public:
    wxTerminalColourHandler();
    ~wxTerminalColourHandler();

    wxTerminalColourHandler& operator<<(const std::string& buffer);
    void SetCtrl(TextView* ctrl);
    void SetDefaultStyle(const wxTextAttr& attr);
};

#endif // WXTERMINALCOLOURHANDLER_H
