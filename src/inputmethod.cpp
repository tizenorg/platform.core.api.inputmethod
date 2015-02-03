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
#include <inputmethod.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "INPUTMETHOD"

using namespace scl;

inputmethod_primitive_callback_s g_inputmethod_primitive_callback = {NULL};
inputmethod_extended_callback_s g_inputmethod_extended_callback = {NULL};

class CCoreEventCallback : public ISCLCoreEventCallback
{
    void on_init();
    void on_run(int argc, char **argv);
    void on_exit();
    void on_update_cursor_position(sclint ic, const sclchar *ic_uuid, sclint cursor_pos);
    void on_update_surrounding_text(sclint ic, const sclchar *text, sclint cursor);
    void on_focus_out(sclint ic, const sclchar *ic_uuid);
    void on_focus_in(sclint ic, const sclchar *ic_uuid);
    void on_ise_show(sclint ic, const int degree, Ise_Context context);
    void on_ise_hide(sclint ic, const sclchar *ic_uuid);
    void on_get_geometry(sclu32 *pos_x, sclu32 *pos_y, sclu32 *width, sclu32 *height);
    void on_set_language(Ecore_IMF_Input_Panel_Lang language);
    void on_set_imdata(sclchar *buf, sclu32 len);
    void on_get_language_locale(sclint ic, sclchar **locale);
    void on_set_return_key_type(Ecore_IMF_Input_Panel_Return_Key_Type type);
    void on_set_return_key_disable(bool disabled);
    void on_set_layout(Ecore_IMF_Input_Panel_Layout layout);
    void on_reset_input_context(sclint ic, const sclchar *uuid);
    void on_set_display_language(const sclchar *language);
    void on_set_rotation_degree(sclint degree);
    void on_set_accessibility_state(sclboolean state);
    void on_create_option_window(sclwindow window, SCLOptionWindowType type);
    void on_destroy_option_window(sclwindow window);
};

static CCoreEventCallback g_core_event_callback;
CSCLCore g_core(&g_core_event_callback);

void CCoreEventCallback::on_init()
{
    if (g_inputmethod_primitive_callback.create) {
        g_inputmethod_primitive_callback.create();
    }
}

void CCoreEventCallback::on_run(int argc, char **argv)
{
    LOGD ("on_run\n");
    inputmethod_efl_main(argc, argv);
}

void CCoreEventCallback::on_exit()
{
    if (g_inputmethod_primitive_callback.terminate) {
        g_inputmethod_primitive_callback.terminate();
    }
}

void CCoreEventCallback::on_update_cursor_position(sclint ic, const sclchar *ic_uuid, sclint cursor_pos)
{
    if (g_inputmethod_extended_callback.update_cursor_position) {
        g_inputmethod_extended_callback.update_cursor_position(cursor_pos);
    }
}

void CCoreEventCallback::on_update_surrounding_text(sclint ic, const sclchar *text, sclint cursor)
{
    if (g_inputmethod_extended_callback.update_surrounding_text) {
        g_inputmethod_extended_callback.update_surrounding_text(ic, text, cursor);
    }
}

void CCoreEventCallback::on_focus_out(sclint ic, const sclchar *ic_uuid)
{
    if (g_inputmethod_extended_callback.focus_out) {
        g_inputmethod_extended_callback.focus_out(ic);
    }
}

void CCoreEventCallback::on_focus_in(sclint ic, const sclchar *ic_uuid)
{
    if (g_inputmethod_extended_callback.focus_in) {
        g_inputmethod_extended_callback.focus_in(ic);
    }
}

void CCoreEventCallback::on_ise_show(sclint ic, const int degree, Ise_Context context)
{
    input_context_s input_context;
    input_context.language = context.language;
    input_context.layout = context.layout;
    input_context.return_key_type = context.return_key_type;
    input_context.client_window = context.client_window;
    input_context.imdata_size = context.imdata_size;
    input_context.cursor_pos = context.cursor_pos;
    input_context.return_key_disabled = context.return_key_disabled;
    input_context.prediction_allow = context.prediction_allow;
    input_context.password_mode = context.password_mode;
    input_context.layout_variation = context.layout_variation;
    input_context.autocapital_type = context.autocapital_type;
    input_context.input_hint = context.input_hint;
    input_context.bidi_direction = context.bidi_direction;

    if (g_inputmethod_primitive_callback.show) {
        g_inputmethod_primitive_callback.show(ic, input_context);
    }
}

void CCoreEventCallback::on_ise_hide(sclint ic, const sclchar *ic_uuid)
{
    if (g_inputmethod_primitive_callback.hide) {
        g_inputmethod_primitive_callback.hide(ic);
    }
}

void CCoreEventCallback::on_get_geometry(sclu32 *pos_x, sclu32 *pos_y, sclu32 *width, sclu32 *height)
{
    int geometry_pos_x = 0;
    int geometry_pos_y = 0;
    int geometry_width = 0;
    int geometry_height = 0;

    if (g_inputmethod_extended_callback.get_geometry) {
        g_inputmethod_extended_callback.get_geometry(&geometry_pos_x, &geometry_pos_y, &geometry_width, &geometry_height);
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
    if (g_inputmethod_extended_callback.set_language) {
        g_inputmethod_extended_callback.set_language(language);
    }
}

void CCoreEventCallback::on_set_imdata(sclchar *buf, sclu32 len)
{
    if (g_inputmethod_extended_callback.set_imdata) {
        g_inputmethod_extended_callback.set_imdata((void *)buf, len);
    }
}

void CCoreEventCallback::on_get_language_locale(sclint ic, sclchar **locale)
{
    if (g_inputmethod_extended_callback.get_language_locale) {
        g_inputmethod_extended_callback.get_language_locale(locale);
    }
}

void CCoreEventCallback::on_set_return_key_type(Ecore_IMF_Input_Panel_Return_Key_Type type)
{
    if (g_inputmethod_extended_callback.set_return_key_type) {
        g_inputmethod_extended_callback.set_return_key_type(type);
    }
}

void CCoreEventCallback::on_set_return_key_disable(bool disabled)
{
    if (g_inputmethod_extended_callback.set_return_key_disable) {
        g_inputmethod_extended_callback.set_return_key_disable(disabled);
    }
}

void CCoreEventCallback::on_set_layout(Ecore_IMF_Input_Panel_Layout layout)
{
    if (g_inputmethod_extended_callback.set_layout) {
        g_inputmethod_extended_callback.set_layout(layout);
    }
}

void CCoreEventCallback::on_reset_input_context(sclint ic, const sclchar *uuid)
{
    if (g_inputmethod_extended_callback.reset_input_context) {
        g_inputmethod_extended_callback.reset_input_context();
    }
}

void CCoreEventCallback::on_set_display_language(const sclchar *language)
{
    if (g_inputmethod_extended_callback.set_display_language) {
        g_inputmethod_extended_callback.set_display_language(language);
    }
}

void CCoreEventCallback::on_set_rotation_degree(sclint degree)
{
    if (g_inputmethod_extended_callback.set_rotation_degree) {
        g_inputmethod_extended_callback.set_rotation_degree(degree);
    }
}

void CCoreEventCallback::on_set_accessibility_state(sclboolean state)
{
    if (g_inputmethod_extended_callback.set_accessibility_state) {
        g_inputmethod_extended_callback.set_accessibility_state(state);
    }
}

void CCoreEventCallback::on_create_option_window(sclwindow window, SCLOptionWindowType type)
{
    if (g_inputmethod_extended_callback.create_option_window) {
        g_inputmethod_extended_callback.create_option_window(static_cast<Evas_Object*>(window), (inputmethod_option_window_type_e)type);
    }
}

void CCoreEventCallback::on_destroy_option_window(sclwindow window)
{
    if (g_inputmethod_extended_callback.destroy_option_window) {
        g_inputmethod_extended_callback.destroy_option_window(static_cast<Evas_Object*>(window));
    }
}

void inputmethod_set_primitive_cb(inputmethod_primitive_callback_s callback)
{
    g_inputmethod_primitive_callback = callback;
}

void inputmethod_unset_primitive_cb(void)
{
    memset(&g_inputmethod_primitive_callback, 0x00, sizeof(g_inputmethod_primitive_callback));
}

void inputmethod_set_extended_cb(inputmethod_extended_callback_s callback)
{
    g_inputmethod_extended_callback = callback;
}

void inputmethod_unset_extended_cb(void)
{
    memset(&g_inputmethod_extended_callback, 0x00, sizeof(g_inputmethod_extended_callback));
}

void inputmethod_run(void)
{
    g_core.run();
}

void inputmethod_send_key_event(inputmethod_key_code_e keycode, inputmethod_key_mask_e keymask)
{
    g_core.forward_key_event(-1, NULL, (sclu32)keycode, (sclu16)keymask);
}

void inputmethod_commit_string(const char *str)
{
    g_core.commit_string(-1, NULL, str);
}

void inputmethod_show_preedit_string(void)
{
    g_core.show_preedit_string(-1, NULL);
}

void inputmethod_hide_preedit_string(void)
{
    g_core.hide_preedit_string(-1, NULL);
}

void inputmethod_update_preedit_string(const char *str)
{
    g_core.update_preedit_string(-1, NULL, str);
}

void inputmethod_get_surrounding_text(int maxlen_before, int maxlen_after)
{
    g_core.get_surrounding_text(NULL, maxlen_before, maxlen_after);
}

void inputmethod_delete_surrounding_text(int offset, int len)
{
    g_core.delete_surrounding_text(offset, len);
}

Evas_Object* inputmethod_get_main_window(void)
{
    return static_cast<Evas_Object*>(g_core.get_main_window());
}

void inputmethod_set_size_hints(int portrait_width, int portrait_height, int landscape_width, int landscape_height)
{
    SclSize portrait_size, landscape_size;

    portrait_size.width = portrait_width;
    portrait_size.height = portrait_height;

    landscape_size.width = landscape_width;
    landscape_size.height = landscape_height;

    g_core.set_keyboard_size_hints(portrait_size, landscape_size);
}

Evas_Object* inputmethod_create_option_window(void)
{
    return static_cast<Evas_Object*>(g_core.create_option_window());
}

void inputmethod_destroy_option_window(Evas_Object *window)
{
    g_core.destroy_option_window(window);
}
