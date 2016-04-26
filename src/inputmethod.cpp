/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlog.h>
#include <sclcore.h>
#include <unistd.h>
#include <privilege_checker_private.h>
#include "inputmethod_private.h"
#include <inputmethod.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "INPUTMETHOD"

using namespace scl;

class CCoreEventCallback : public ISCLCoreEventCallback
{
    void on_init();
    void on_run(int argc, char **argv);
    void on_exit();
    void on_update_cursor_position(sclint ic, const sclchar *ic_uuid, sclint cursor_pos);
    void on_update_surrounding_text(sclint ic, const sclchar *text, sclint cursor);
    void on_focus_out(sclint ic, const sclchar *ic_uuid);
    void on_focus_in(sclint ic, const sclchar *ic_uuid);
    void on_ise_show(sclint ic, const int degree, Ise_Context &context);
    void on_ise_hide(sclint ic, const sclchar *ic_uuid);
    void on_get_geometry(sclu32 *pos_x, sclu32 *pos_y, sclu32 *width, sclu32 *height);
    void on_set_language(Ecore_IMF_Input_Panel_Lang language);
    void on_set_imdata(sclchar *buf, sclu32 len);
    void on_get_imdata(sclchar **buf, sclu32 *len);
    void on_get_language_locale(sclint ic, sclchar **locale);
    void on_set_return_key_type(Ecore_IMF_Input_Panel_Return_Key_Type type);
    void on_set_return_key_disable(sclu32 disabled);
    void on_set_layout(Ecore_IMF_Input_Panel_Layout layout);
    void on_reset_input_context(sclint ic, const sclchar *uuid);
    void on_process_key_event(scim::KeyEvent &key, sclu32 *ret);
    void on_set_display_language(const sclchar *language);
    void on_set_rotation_degree(sclint degree);
    void on_set_accessibility_state(sclboolean state);
    void on_create_option_window(sclwindow window, SCLOptionWindowType type);
    void on_destroy_option_window(sclwindow window);
    void on_check_option_window_availability(sclboolean *ret);
};

typedef struct
{
    ime_focus_in_cb focus_in;   /**< Called when an edit field has focus */
    ime_focus_out_cb focus_out; /**< Called when an edit field loses focus */
    ime_surrounding_text_updated_cb surrounding_text_updated;   /**< Called when an edit field responds to a request with the surrounding text */
    ime_input_context_reset_cb input_context_reset;             /**< Called to reset the input context of an edit field */
    ime_cursor_position_updated_cb cursor_position_updated;     /**< Called when the position of the cursor in an edit field changes */
    ime_language_requested_cb language_requested;   /**< Called when an edit field requests the language from the input panel */
    ime_language_set_cb language_set;   /**< Called to set the preferred language to the input panel */
    ime_imdata_set_cb imdata_set;       /**< Called to set the application specific data to deliver to the input panel */
    ime_imdata_requested_cb imdata_requested;       /**< Called when an associated text input UI control requests the application specific data from the input panel */
    ime_layout_set_cb layout_set;       /**< Called when an edit field requests the input panel to set its layout */
    ime_return_key_type_set_cb return_key_type_set;     /**< Called when an edit field requests the input panel to set the "return" key label */
    ime_return_key_state_set_cb return_key_state_set;   /**< Called when an edit field requests the input panel to enable or disable the "return" key state */
    ime_geometry_requested_cb geometry_requested;       /**< Called when an edit field requests the position and size from the input panel */
    ime_process_key_event_cb process_key_event;         /**< Called when the key event is received from the external keyboard devices */
    ime_display_language_changed_cb display_language_changed;   /**< Called when the system display language is changed */
    ime_rotation_degree_changed_cb rotation_degree_changed;     /**< Called when the device is rotated */
    ime_accessibility_state_changed_cb accessibility_state_changed; /**< Called when Accessibility in Settings application is on or off */
    ime_option_window_created_cb option_window_created;     /**< Called to create the option window */
    ime_option_window_destroyed_cb option_window_destroyed; /**< Called to destroy the option window */
    void *focus_in_user_data;
    void *focus_out_user_data;
    void *surrounding_text_updated_user_data;
    void *input_context_reset_user_data;
    void *cursor_position_updated_user_data;
    void *language_requested_user_data;
    void *language_set_user_data;
    void *imdata_set_user_data;
    void *imdata_requested_user_data;
    void *layout_set_user_data;
    void *return_key_type_set_user_data;
    void *return_key_state_set_user_data;
    void *geometry_requested_user_data;
    void *process_key_event_user_data;
    void *display_language_changed_user_data;
    void *rotation_degree_changed_user_data;
    void *accessibility_state_changed_user_data;
    void *option_window_created_user_data;
    void *option_window_destroyed_user_data;
} ime_event_callback_s;

static ime_callback_s g_basic_callback = {NULL};
static ime_event_callback_s g_event_callback = {NULL};
static void *g_user_data = NULL;
static bool g_running = false;

static CCoreEventCallback g_core_event_callback;
CSCLCore g_core(&g_core_event_callback);

extern "C" {
    extern void ime_app_main(int argc, char **argv);
}

void CCoreEventCallback::on_init()
{
    if (g_basic_callback.create) {
        g_basic_callback.create(g_user_data);
    }
}

void CCoreEventCallback::on_run(int argc, char **argv)
{
    LOGD("on_run");
    ime_app_main(argc, argv);
}

void CCoreEventCallback::on_exit()
{
    LOGD("on_exit");
    if (g_basic_callback.terminate) {
        g_basic_callback.terminate(g_user_data);
    }
}

void CCoreEventCallback::on_update_cursor_position(sclint ic, const sclchar *ic_uuid, sclint cursor_pos)
{
    if (g_event_callback.cursor_position_updated) {
        g_event_callback.cursor_position_updated(cursor_pos, g_event_callback.cursor_position_updated_user_data);
    }
}

void CCoreEventCallback::on_update_surrounding_text(sclint ic, const sclchar *text, sclint cursor)
{
    if (g_event_callback.surrounding_text_updated) {
        g_event_callback.surrounding_text_updated(ic, text, cursor, g_event_callback.surrounding_text_updated_user_data);
    }
}

void CCoreEventCallback::on_focus_out(sclint ic, const sclchar *ic_uuid)
{
    if (g_event_callback.focus_out) {
        g_event_callback.focus_out(ic, g_event_callback.focus_out_user_data);
    }
}

void CCoreEventCallback::on_focus_in(sclint ic, const sclchar *ic_uuid)
{
    if (g_event_callback.focus_in) {
        g_event_callback.focus_in(ic, g_event_callback.focus_in_user_data);
    }
}

void CCoreEventCallback::on_ise_show(sclint ic, const int degree, Ise_Context &context)
{
    if (g_basic_callback.show) {
        struct _ime_context input_context;

        memset(&input_context, 0, sizeof(struct _ime_context));
        input_context.layout = context.layout;
        input_context.layout_variation = context.layout_variation;
        input_context.cursor_pos = context.cursor_pos;
        input_context.autocapital_type = context.autocapital_type;
        input_context.return_key_type = context.return_key_type;
        input_context.return_key_disabled = context.return_key_disabled;
        input_context.prediction_allow = context.prediction_allow;
        input_context.password_mode = context.password_mode;
        input_context.imdata_size = context.imdata_size;
        input_context.input_hint = context.input_hint;
        input_context.bidi_direction = context.bidi_direction;
        input_context.language = context.language;
        input_context.client_window = context.client_window;

        g_basic_callback.show(ic, static_cast<ime_context_h>(&input_context), g_user_data);
    }
}

void CCoreEventCallback::on_ise_hide(sclint ic, const sclchar *ic_uuid)
{
    if (g_basic_callback.hide) {
        g_basic_callback.hide(ic, g_user_data);
    }
}

void CCoreEventCallback::on_get_geometry(sclu32 *pos_x, sclu32 *pos_y, sclu32 *width, sclu32 *height)
{
    int geometry_pos_x = 0;
    int geometry_pos_y = 0;
    int geometry_width = 0;
    int geometry_height = 0;

    if (g_event_callback.geometry_requested) {
        g_event_callback.geometry_requested(g_event_callback.geometry_requested_user_data, &geometry_pos_x, &geometry_pos_y, &geometry_width, &geometry_height);
    }

    if (pos_x)
        *pos_x = geometry_pos_x;

    if (pos_y)
        *pos_y = geometry_pos_y;

    if (width)
        *width = geometry_width;

    if (height)
        *height = geometry_height;
}

void CCoreEventCallback::on_set_language(Ecore_IMF_Input_Panel_Lang language)
{
    if (g_event_callback.language_set) {
        g_event_callback.language_set(language, g_event_callback.language_set_user_data);
    }
}

void CCoreEventCallback::on_set_imdata(sclchar *buf, sclu32 len)
{
    if (g_event_callback.imdata_set) {
        g_event_callback.imdata_set((void *)buf, len, g_event_callback.imdata_set_user_data);
    }
}

void CCoreEventCallback::on_get_imdata(sclchar **buf, sclu32 *len)
{
    if (g_event_callback.imdata_requested) {
        g_event_callback.imdata_requested(g_event_callback.imdata_set_user_data, (void **)buf, len);
    }
}

void CCoreEventCallback::on_get_language_locale(sclint ic, sclchar **locale)
{
    if (g_event_callback.language_requested) {
        g_event_callback.language_requested(g_event_callback.language_requested_user_data, locale);
    }
}

void CCoreEventCallback::on_set_return_key_type(Ecore_IMF_Input_Panel_Return_Key_Type type)
{
    if (g_event_callback.return_key_type_set) {
        g_event_callback.return_key_type_set(type, g_event_callback.return_key_type_set_user_data);
    }
}

void CCoreEventCallback::on_set_return_key_disable(sclu32 disabled)
{
    bool return_key_disabled = !!disabled;

    if (g_event_callback.return_key_state_set) {
        g_event_callback.return_key_state_set(return_key_disabled, g_event_callback.return_key_state_set_user_data);
    }
}

void CCoreEventCallback::on_set_layout(Ecore_IMF_Input_Panel_Layout layout)
{
    if (g_event_callback.layout_set) {
        g_event_callback.layout_set(layout, g_event_callback.layout_set_user_data);
    }
}

void CCoreEventCallback::on_reset_input_context(sclint ic, const sclchar *uuid)
{
    if (g_event_callback.input_context_reset) {
        g_event_callback.input_context_reset(g_event_callback.input_context_reset_user_data);
    }
}

void CCoreEventCallback::on_process_key_event(scim::KeyEvent &key, sclu32 *ret)
{
    if (g_event_callback.process_key_event) {
        struct _ime_device_info dev_info = {key.dev_name.c_str(),
            static_cast<Ecore_IMF_Device_Class>(key.dev_class), static_cast<Ecore_IMF_Device_Subclass>(key.dev_subclass)};
        bool processed = g_event_callback.process_key_event(static_cast<ime_key_code_e>(key.code), static_cast<ime_key_mask_e>(key.mask),
            static_cast<ime_device_info_h>(&dev_info), g_event_callback.process_key_event_user_data);

        if (ret) {
            if (processed)
                *ret = 1;
            else
                *ret = 0;
        }
    } else {
        if (ret) {
            *ret = 0;
        }
    }
}

void CCoreEventCallback::on_set_display_language(const sclchar *language)
{
    if (g_event_callback.display_language_changed) {
        g_event_callback.display_language_changed(language, g_event_callback.display_language_changed_user_data);
    }
}

void CCoreEventCallback::on_set_rotation_degree(sclint degree)
{
    if (g_event_callback.rotation_degree_changed) {
        g_event_callback.rotation_degree_changed(degree, g_event_callback.rotation_degree_changed_user_data);
    }
}

void CCoreEventCallback::on_set_accessibility_state(sclboolean state)
{
    if (g_event_callback.accessibility_state_changed) {
        g_event_callback.accessibility_state_changed(state, g_event_callback.accessibility_state_changed_user_data);
    }
}

void CCoreEventCallback::on_create_option_window(sclwindow window, SCLOptionWindowType type)
{
    if (g_event_callback.option_window_created) {
        g_event_callback.option_window_created(static_cast<Evas_Object*>(window), static_cast<ime_option_window_type_e>(type), g_event_callback.option_window_created_user_data);
    }
}

void CCoreEventCallback::on_destroy_option_window(sclwindow window)
{
    if (g_event_callback.option_window_destroyed) {
        g_event_callback.option_window_destroyed(static_cast<Evas_Object*>(window), g_event_callback.option_window_destroyed_user_data);
    }
}

void CCoreEventCallback::on_check_option_window_availability(sclboolean *ret)
{
    if (ret) {
        if (g_event_callback.option_window_created) // Instead of each 3rd party keyboard, inputmethod will reply the availability of the option (setting).
            *ret = true;
        else
            *ret = false;
    }
}

ime_error_e _check_privilege()
{
    char uid[16];
    ime_error_e ret = IME_ERROR_NONE;

    if (inputmethod_cynara_initialize() == false) {
        LOGE("inputmethod_cynara_initialize () == false");
        return IME_ERROR_PERMISSION_DENIED;
    }

    snprintf(uid, 16, "%d", getuid());
    if (check_privilege(uid, IME_PRIVILEGE) == false) {
        LOGE("check_privilege(uid, IME_PRIVILEGE) == false. uid : %s", uid);
        ret = IME_ERROR_PERMISSION_DENIED;
    }

    inputmethod_cynara_finish();

    return ret;
}

int ime_run(ime_callback_s *basic_cb, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (g_running) {
        LOGE("inputmethod main loop is already running.");
        return IME_ERROR_OPERATION_FAILED;
    }

    if (!basic_cb) {
        LOGE("basic callbacks pointer is null.");
        return IME_ERROR_INVALID_PARAMETER;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_basic_callback = *basic_cb;

    if (!g_basic_callback.create || !g_basic_callback.terminate ||
        !g_basic_callback.show || !g_basic_callback.hide) {
        LOGE("mandatory callback funtions are not set");
        memset(&g_basic_callback, 0, sizeof(ime_callback_s));
        memset(&g_event_callback, 0, sizeof(ime_event_callback_s));
        return IME_ERROR_NO_CALLBACK_FUNCTION;
    }

    g_user_data = user_data;
    g_running = true;

    g_core.run();

    memset(&g_basic_callback, 0, sizeof(ime_callback_s));
    memset(&g_event_callback, 0, sizeof(ime_event_callback_s));
    g_user_data = NULL;
    g_running = false;

    return IME_ERROR_NONE;
}

int ime_event_set_focus_in_cb(ime_focus_in_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.focus_in = callback_func;
    g_event_callback.focus_in_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_focus_out_cb(ime_focus_out_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.focus_out = callback_func;
    g_event_callback.focus_out_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_surrounding_text_updated_cb(ime_surrounding_text_updated_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.surrounding_text_updated = callback_func;
    g_event_callback.surrounding_text_updated_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_input_context_reset_cb(ime_input_context_reset_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.input_context_reset = callback_func;
    g_event_callback.input_context_reset_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_cursor_position_updated_cb(ime_cursor_position_updated_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.cursor_position_updated = callback_func;
    g_event_callback.cursor_position_updated_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_language_requested_cb(ime_language_requested_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.language_requested = callback_func;
    g_event_callback.language_requested_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_language_set_cb(ime_language_set_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.language_set = callback_func;
    g_event_callback.language_set_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_imdata_set_cb(ime_imdata_set_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.imdata_set = callback_func;
    g_event_callback.imdata_set_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_imdata_requested_cb(ime_imdata_requested_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.imdata_requested = callback_func;
    g_event_callback.imdata_requested_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_layout_set_cb(ime_layout_set_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.layout_set = callback_func;
    g_event_callback.layout_set_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_return_key_type_set_cb(ime_return_key_type_set_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.return_key_type_set = callback_func;
    g_event_callback.return_key_type_set_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_return_key_state_set_cb(ime_return_key_state_set_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.return_key_state_set = callback_func;
    g_event_callback.return_key_state_set_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_geometry_requested_cb(ime_geometry_requested_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.geometry_requested = callback_func;
    g_event_callback.geometry_requested_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_process_key_event_cb(ime_process_key_event_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.process_key_event = callback_func;
    g_event_callback.process_key_event_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_display_language_changed_cb(ime_display_language_changed_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.display_language_changed = callback_func;
    g_event_callback.display_language_changed_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_rotation_degree_changed_cb(ime_rotation_degree_changed_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.rotation_degree_changed = callback_func;
    g_event_callback.rotation_degree_changed_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_accessibility_state_changed_cb(ime_accessibility_state_changed_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.accessibility_state_changed = callback_func;
    g_event_callback.accessibility_state_changed_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_option_window_created_cb(ime_option_window_created_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.option_window_created = callback_func;
    g_event_callback.option_window_created_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_event_set_option_window_destroyed_cb(ime_option_window_destroyed_cb callback_func, void *user_data)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!callback_func) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (g_running) {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_event_callback.option_window_destroyed = callback_func;
    g_event_callback.option_window_destroyed_user_data = user_data;

    return IME_ERROR_NONE;
}

int ime_send_key_event(ime_key_code_e keycode, ime_key_mask_e keymask, bool forward_key)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    if (forward_key)
        g_core.forward_key_event(-1, NULL, (sclu32)keycode, (sclu16)keymask);
    else
        g_core.send_key_event(-1, NULL, (sclu32)keycode, (sclu16)keymask);

    return IME_ERROR_NONE;
}

int ime_commit_string(const char *str)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!str) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_core.commit_string(-1, NULL, str);

    return IME_ERROR_NONE;
}

int ime_show_preedit_string(void)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_core.show_preedit_string(-1, NULL);

    return IME_ERROR_NONE;
}

int ime_hide_preedit_string(void)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_core.hide_preedit_string(-1, NULL);

    return IME_ERROR_NONE;
}

int ime_update_preedit_string(const char *str, Eina_List *attrs)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!str) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    scim::AttributeList attrv;
    void *data = NULL;
    ime_preedit_attribute *attr = NULL;

    if (attrs) {
        EINA_LIST_FREE(attrs, data) {
            attr = (ime_preedit_attribute *)data;
            if (attr) {
                attrv.push_back(scim::Attribute(attr->start, attr->length, (scim::AttributeType)attr->type, attr->value));
                free(attr);
            }
        }
    }

    g_core.update_preedit_string(-1, NULL, str, attrv);

    return IME_ERROR_NONE;
}

int ime_request_surrounding_text(int maxlen_before, int maxlen_after)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!g_event_callback.surrounding_text_updated) {
        LOGW("IME_ERROR_NO_CALLBACK_FUNCTION");
        return IME_ERROR_NO_CALLBACK_FUNCTION;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_core.get_surrounding_text(NULL, maxlen_before, maxlen_after);

    return IME_ERROR_NONE;
}

int ime_delete_surrounding_text(int offset, int len)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (len <= 0) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_core.delete_surrounding_text(offset, len);

    return IME_ERROR_NONE;
}

int ime_get_surrounding_text(int maxlen_before, int maxlen_after, char **text, int *cursor_pos)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!text || !cursor_pos) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    int cursor = 0;

    g_core.get_surrounding_text(maxlen_before, maxlen_after, text, cursor);

    if (cursor_pos)
        *cursor_pos = cursor;

    return IME_ERROR_NONE;
}

Evas_Object* ime_get_main_window(void)
{
    ime_error_e retVal = IME_ERROR_NONE;

    Evas_Object *win = NULL;

    if (!g_running) {
        set_last_result(IME_ERROR_NOT_RUNNING);
        LOGW("IME_ERROR_NOT_RUNNING");
        return NULL;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        set_last_result(retVal);
        return NULL;
    }

    win = static_cast<Evas_Object*>(g_core.get_main_window());
    if (win) {
        set_last_result(IME_ERROR_NONE);
    } else {
        set_last_result(IME_ERROR_OPERATION_FAILED);
        LOGW("IME_ERROR_OPERATION_FAILED");
    }

    return win;
}

int ime_set_size(int portrait_width, int portrait_height, int landscape_width, int landscape_height)
{
    ime_error_e retVal = IME_ERROR_NONE;

    SclSize portrait_size, landscape_size;

    if (portrait_width < 1 || portrait_height < 1 || landscape_width < 1 || landscape_height < 1) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    portrait_size.width = portrait_width;
    portrait_size.height = portrait_height;

    landscape_size.width = landscape_width;
    landscape_size.height = landscape_height;

    g_core.set_keyboard_size_hints(portrait_size, landscape_size);

    return IME_ERROR_NONE;
}

int ime_create_option_window(void)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!g_event_callback.option_window_created || !g_event_callback.option_window_destroyed) {
        LOGW("ime_create_option_window_cb() and ime_destroy_option_window_cb() callback functions are not set.");
        return IME_ERROR_NO_CALLBACK_FUNCTION;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    if (g_core.create_option_window()) {
        return IME_ERROR_NONE;
    } else {
        LOGW("IME_ERROR_OPERATION_FAILED");
        return IME_ERROR_OPERATION_FAILED;
    }
}

int ime_destroy_option_window(Evas_Object *window)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!g_event_callback.option_window_created || !g_event_callback.option_window_destroyed) {
        LOGW("ime_create_option_window_cb() and ime_destroy_option_window_cb() callback functions are not set.");
        return IME_ERROR_NO_CALLBACK_FUNCTION;
    }

    if (!window) {
        LOGW("Window pointer is null.");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    g_core.destroy_option_window(window);

    return IME_ERROR_NONE;
}

int ime_context_get_layout(ime_context_h context, Ecore_IMF_Input_Panel_Layout *layout)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !layout) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *layout = context->layout;

    return IME_ERROR_NONE;
}

int ime_context_get_layout_variation(ime_context_h context, ime_layout_variation_e *layout_variation)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !layout_variation) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *layout_variation = static_cast<ime_layout_variation_e>(context->layout_variation);

    return IME_ERROR_NONE;
}

int ime_context_get_cursor_position(ime_context_h context, int *cursor_pos)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !cursor_pos) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *cursor_pos = context->cursor_pos;

    return IME_ERROR_NONE;
}

int ime_context_get_autocapital_type(ime_context_h context, Ecore_IMF_Autocapital_Type *autocapital_type)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !autocapital_type) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *autocapital_type = context->autocapital_type;

    return IME_ERROR_NONE;
}

int ime_context_get_return_key_type(ime_context_h context, Ecore_IMF_Input_Panel_Return_Key_Type *return_key_type)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !return_key_type) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *return_key_type = context->return_key_type;

    return IME_ERROR_NONE;
}

int ime_context_get_return_key_state(ime_context_h context, bool *return_key_state)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !return_key_state) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *return_key_state = static_cast<bool>(context->return_key_disabled);

    return IME_ERROR_NONE;
}

int ime_context_get_prediction_mode(ime_context_h context, bool *prediction_mode)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !prediction_mode) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *prediction_mode = static_cast<bool>(context->prediction_allow);

    return IME_ERROR_NONE;
}

int ime_context_get_password_mode(ime_context_h context, bool *password_mode)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !password_mode) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *password_mode = static_cast<bool>(context->password_mode);

    return IME_ERROR_NONE;
}

int ime_context_get_input_hint(ime_context_h context, Ecore_IMF_Input_Hints *input_hint)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !input_hint) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *input_hint = context->input_hint;

    return IME_ERROR_NONE;
}

int ime_context_get_bidi_direction(ime_context_h context, Ecore_IMF_BiDi_Direction *bidi)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !bidi) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *bidi = context->bidi_direction;

    return IME_ERROR_NONE;
}

int ime_context_get_language(ime_context_h context, Ecore_IMF_Input_Panel_Lang *language)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!context || !language) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *language = context->language;

    return IME_ERROR_NONE;
}

int ime_device_info_get_name(ime_device_info_h dev_info, char **dev_name)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!dev_info || !dev_name) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    if (!dev_info->dev_name)
        *dev_name = strdup("");
    else
        *dev_name = strdup(dev_info->dev_name);

    return IME_ERROR_NONE;
}

int ime_device_info_get_class(ime_device_info_h dev_info, Ecore_IMF_Device_Class *dev_class)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!dev_info || !dev_class) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *dev_class = dev_info->dev_class;

    return IME_ERROR_NONE;
}

int ime_device_info_get_subclass(ime_device_info_h dev_info, Ecore_IMF_Device_Subclass *dev_subclass)
{
    ime_error_e retVal = IME_ERROR_NONE;

    if (!dev_info || !dev_subclass) {
        LOGW("IME_ERROR_INVALID_PARAMETER");
        return IME_ERROR_INVALID_PARAMETER;
    }

    if (!g_running) {
        LOGW("IME_ERROR_NOT_RUNNING");
        return IME_ERROR_NOT_RUNNING;
    }

    retVal = _check_privilege();
    if (retVal != IME_ERROR_NONE) {
        LOGE("_check_privilege returned %d.", retVal);
        return retVal;
    }

    *dev_subclass = dev_info->dev_subclass;

    return IME_ERROR_NONE;
}


