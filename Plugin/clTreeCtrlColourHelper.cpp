#include "clTreeCtrlColourHelper.h"
#include "macros.h"
#include <deque>

clTreeCtrlColourHelper::clTreeCtrlColourHelper(wxTreeCtrl* tree)
    : m_tree(tree)
{
}

clTreeCtrlColourHelper::~clTreeCtrlColourHelper() {}

void clTreeCtrlColourHelper::DoSetBgColour(const wxTreeItemId& item, const wxColour& currentBgColour,
                                           const VirtualDirectoryColour::Map_t& coloursMap) const
{
    CHECK_ITEM_RET(item);
    wxString path = GetItemPath(item);
    wxColour bgColour = currentBgColour;
    VirtualDirectoryColour::Map_t::const_iterator iter = coloursMap.find(path);
    if(iter != coloursMap.end()) {
        bgColour = iter->second.GetColour();
    }
    
    if(bgColour.IsOk()) {
        m_tree->SetItemBackgroundColour(item, bgColour);
    }
    
    if(m_tree->ItemHasChildren(item)) {
        wxTreeItemIdValue cookie;
        wxTreeItemId child = m_tree->GetFirstChild(item, cookie);
        while(child.IsOk()) {
            DoSetBgColour(child, bgColour, coloursMap);
            child = m_tree->GetNextChild(item, cookie);
        }
    }
}

void clTreeCtrlColourHelper::DoClearBgColour(const wxTreeItemId& item, const wxColour& colourToSet,
                                             VirtualDirectoryColour::Map_t& coloursMap) const
{
    CHECK_ITEM_RET(item);
    wxString path = GetItemPath(item);
    VirtualDirectoryColour::Map_t::const_iterator iter = coloursMap.find(path);
    if(iter != coloursMap.end()) {
        coloursMap.erase(iter);
    }
    if(colourToSet.IsOk()) {
        m_tree->SetItemBackgroundColour(item, colourToSet);
    }
    if(m_tree->ItemHasChildren(item)) {
        wxTreeItemIdValue cookie;
        wxTreeItemId child = m_tree->GetFirstChild(item, cookie);
        while(child.IsOk()) {
            DoClearBgColour(child, colourToSet, coloursMap);
            child = m_tree->GetNextChild(item, cookie);
        }
    }
}

void clTreeCtrlColourHelper::ResetBgColour(const wxTreeItemId& item, VirtualDirectoryColour::Map_t& coloursMap) const
{
    CHECK_ITEM_RET(item);
    wxTreeItemId itemParent = m_tree->GetItemParent(item);
    wxColour col = (itemParent.IsOk() && m_tree->GetItemBackgroundColour(itemParent).IsOk())
                       ? m_tree->GetItemBackgroundColour(itemParent)
                       : m_tree->GetBackgroundColour();

    DoClearBgColour(item, col, coloursMap);
}

void clTreeCtrlColourHelper::SetBgColour(const wxTreeItemId& item, const wxColour& currentBgColour,
                                         VirtualDirectoryColour::Map_t& coloursMap) const
{
    wxString path = GetItemPath(item);
    VirtualDirectoryColour vdc(path, currentBgColour);
    if(coloursMap.count(path)) {
        coloursMap.erase(path);
    }
    coloursMap.insert(std::make_pair(path, vdc));
    DoSetBgColour(item, currentBgColour, coloursMap);
}

wxString clTreeCtrlColourHelper::GetItemPath(const wxTreeItemId& item) const
{
    std::deque<wxString> queue;
    wxString text = m_tree->GetItemText(item);
    queue.push_front(text);

    wxTreeItemId p = m_tree->GetItemParent(item);
    while(p.IsOk()) {
        text = m_tree->GetItemText(p);
        queue.push_front(text);
        p = m_tree->GetItemParent(p);
    }

    wxString path;
    while(!queue.empty()) {
        path << queue.front() << "/";
        queue.pop_front();
    }

    if(!path.IsEmpty()) {
        path.RemoveLast();
    }
    return path;
}

