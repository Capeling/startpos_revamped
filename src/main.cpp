#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

#include <StartposPopup.hpp>

struct HookEditorUI : geode::Modify<HookEditorUI, EditorUI> {
    void editObject(CCObject* sender) {
        
        if(m_selectedObject && m_selectedObject->m_objectID == 31) {
            auto obj = static_cast<StartPosObject*>(m_selectedObject);
            auto spUI = StartposPopup::create(obj->m_startSettings);

            spUI->m_noElasticity = true;
            spUI->show();

            return;
        }
        EditorUI::editObject(sender);
    }
};