#include "Extension.hpp"

Extension extension;
SMEXT_LINK(&extension);

BaseClass *BaseClass::head = NULL;

bool Extension::SDK_OnLoad(char *error, size_t err_max, bool late) {
    auto head = BaseClass::head;
    while (head) {
        head->OnExtLoad();
        head = head->next;
    }

    return true;
}

void Extension::SDK_OnUnload() {

}
