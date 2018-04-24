#include "work_queue.hpp"
#include "handletypes.hpp"

std::mutex g_callback_mutex;
std::queue<std::unique_ptr<CallbackItem>> callback_queue;

void MessageCallback::Execute() {
    HandleError err;
    auto hndl = handlesys->CreateHandle(g_MessageType, this->message, nullptr, myself->GetIdentity(), &err);
    if (!hndl) {
        smutils->LogError(myself, "Got null handle, error code: %d", err);
        return;
    }

    auto author_hndl = handlesys->CreateHandle(g_UserType, this->user, nullptr, myself->GetIdentity(), &err);
    if (!hndl) {
        smutils->LogError(myself, "Got null handle, error code: %d", err);
        return;
    }

    g_MessageForward->PushCell(author_hndl);
    g_MessageForward->PushCell(hndl);
    g_MessageForward->Execute();

    handlesys->FreeHandle(hndl, nullptr);
    handlesys->FreeHandle(author_hndl, nullptr);
}

void ReadyCallback::Execute() {
    HandleError err;
    auto hndl = handlesys->CreateHandle(g_ReadyType, this->ready, nullptr, myself->GetIdentity(), &err);
    if (!hndl) {
        smutils->LogError(myself, "Got null handle, error code: %d", err);
        return;
    }

    g_ReadyForward->PushCell(hndl);
    g_ReadyForward->Execute();

    handlesys->FreeHandle(hndl, nullptr);
}

void UserCallback::Execute() {
    if (this->callback) {
        HandleError err;
        auto hndl = handlesys->CreateHandle(g_UserType, this->user, this->plugin, myself->GetIdentity(), &err);
        if (!hndl) {
            smutils->LogError(myself, "Got null handle, error code: %d", err);
        }

        callback->PushCell(hndl);
        callback->PushCell(this->data);
        callback->Execute(nullptr);
    }
}

void On_GameFrame(bool simulating) {
    if (g_callback_mutex.try_lock()) {
        std::lock_guard<std::mutex> guard(g_callback_mutex, std::adopt_lock);
        if (!callback_queue.empty()) {
            while (!callback_queue.empty()) {
                auto callback = std::move(callback_queue.front());
                callback_queue.pop();

                callback->Execute();
            }
        }
    }
}

void AddCallback(std::unique_ptr<CallbackItem> item) {
    std::lock_guard<std::mutex> guard(g_callback_mutex);
    callback_queue.push(std::move(item));
}