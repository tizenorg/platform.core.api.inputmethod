/*
 * Copyright (c) 2014-2015 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UIX_INPUTMETHOD_H__
#define __TIZEN_UIX_INPUTMETHOD_H__

/**
 * @file inputmethod.h
 * @brief This file contains inputmethod APIs and related enumeration.
 */

#include <tizen.h>
#include "inputmethod_keydef.h"

#include <Ecore_IMF.h>
#include <Evas.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_UIX_INPUTMETHOD_MODULE
 * @{
 */

/**
 * @brief Enumeration for inputmethod function error
 *
 * @since_tizen 2.4
 */
typedef enum
{
    INPUTMETHOD_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
    INPUTMETHOD_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
    INPUTMETHOD_ERROR_NO_CALLBACK_FUNCTION = TIZEN_ERROR_UIX_CLASS | 0x0001, /**< Necessary callback function is not set */
    INPUTMETHOD_ERROR_NOT_RUNNING = TIZEN_ERROR_UIX_CLASS | 0x0002, /**< IME main loop isn't started yet */
    INPUTMETHOD_ERROR_OPERATION_FAILED = TIZEN_ERROR_UIX_CLASS | 0x0003, /**< Operation failed */
} inputmethod_error_e;

/**
 * @brief Enumeration of the option window type
 *
 * @since_tizen 2.4
 *
 * @see inputmethod_option_window_created_cb
 */
typedef enum
{
    INPUTMETHOD_OPTION_WINDOW_TYPE_KEYBOARD,  /**< Open from Keyboard */
    INPUTMETHOD_OPTION_WINDOW_TYPE_SETTING_APPLICATION, /**< Open from Setting application */
} inputmethod_option_window_type_e;

/**
 * @brief Enumeration of layout variation
 *
 * @since_tizen 2.4
 *
 * @see inputmethod_context_get_layout_variation
 */
typedef enum
{
    INPUTMETHOD_LAYOUT_NORMAL_VARIATION_NORMAL = 0, /**< The plain normal layout */
    INPUTMETHOD_LAYOUT_NORMAL_VARIATION_FILENAME, /**< Filename layout; symbols such as '/', '*', '\', '|', '<', '>', '?', '"' and ':' should be disabled */
    INPUTMETHOD_LAYOUT_NORMAL_VARIATION_PERSON_NAME, /**< The name of a person */
    INPUTMETHOD_LAYOUT_NUMBERONLY_VARIATION_NORMAL = 0, /**< The plain normal number layout */
    INPUTMETHOD_LAYOUT_NUMBERONLY_VARIATION_SIGNED, /**< The number layout to allow a negative sign */
    INPUTMETHOD_LAYOUT_NUMBERONLY_VARIATION_DECIMAL, /**< The number layout to allow decimal point to provide fractional value */
    INPUTMETHOD_LAYOUT_NUMBERONLY_VARIATION_SIGNED_AND_DECIMAL, /**< The number layout to allow decimal point and negative sign */
    INPUTMETHOD_LAYOUT_PASSWORD_VARIATION_NORMAL = 0, /**< The normal password layout */
    INPUTMETHOD_LAYOUT_PASSWORD_VARIATION_NUMBERONLY, /**< The password layout to allow only number */
} inputmethod_layout_variation_e;

/**
 * @brief Structure of an associated text input UI control's input context.
 *
 * @details This is one of parameters of inputmethod_show_cb() callback function. IME application
 * should configure its input panel with this structure information.
 *
 * @since_tizen 2.4
 *
 * @see inputmethod_context_get_layout, inputmethod_context_get_layout_variation, inputmethod_context_get_cursor_position,
 * inputmethod_context_get_autocapital_type, inputmethod_context_get_return_key_type, inputmethod_context_get_return_key_state,
 * inputmethod_context_get_prediction_mode, inputmethod_context_get_password_mode, inputmethod_context_get_input_hint,
 * inputmethod_context_get_bidi_direction, inputmethod_context_get_language
 */
typedef struct _inputmethod_context *inputmethod_context_h;

/**
 * @brief Called when the input panel is created.
 *
 * @remarks This callback function is mandatory and must be registered using inputmethod_run(). The
 * inputmethod_get_main_window() can be used to get the created input panel window.
 *
 * @since_tizen 2.4
 *
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The inputmethod_run() function calls this callback function.
 *
 * @see inputmethod_run, inputmethod_set_size, inputmethod_get_main_window
 */
typedef void (*inputmethod_create_cb)(void *user_data);

/**
 * @brief Called when the input panel is terminated.
 *
 * @remarks This callback function is mandatory and must be registered using inputmethod_run(). The
 * inputmethod_get_main_window() can be used to get the created input panel window.
 *
 * @since_tizen 2.4
 *
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_run, inputmethod_get_main_window
 */
typedef void (*inputmethod_terminate_cb)(void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to show itself.
 *
 * @remarks This callback function is mandatory and must be registered using inputmethod_run().
 * IME application should configure its input panel with #inputmethod_context_h structure information.
 * The inputmethod_get_main_window() can be used to get the created input panel window.
 *
 * @since_tizen 2.4
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] context The input context information pointer
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_run, inputmethod_get_main_window, inputmethod_context_get_layout,
 * inputmethod_context_get_layout_variation, inputmethod_context_get_cursor_position,
 * inputmethod_context_get_autocapital_type, inputmethod_context_get_return_key_type,
 * inputmethod_context_get_return_key_state, inputmethod_context_get_prediction_mode,
 * inputmethod_context_get_password_mode, inputmethod_context_get_input_hint,
 * inputmethod_context_get_bidi_direction, inputmethod_context_get_language
 */
typedef void (*inputmethod_show_cb)(int context_id, inputmethod_context_h context, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to hide itself.
 *
 * @remarks This callback function is mandatory and must be registered using inputmethod_run(). The
 * inputmethod_get_main_window() can be used to get the created input panel window.
 *
 * @since_tizen 2.4
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @see inputmethod_run, inputmethod_get_main_window
 */
typedef void (*inputmethod_hide_cb)(int context_id, void *user_data);

/**
 * @brief Called when an associated text input UI control has focus.
 *
 * @since_tizen 2.4
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_focus_in_cb().
 *
 * @see inputmethod_event_set_focus_in_cb
 */
typedef void (*inputmethod_focus_in_cb)(int context_id, void *user_data);

/**
 * @brief Called when an associated text input UI control loses focus.
 *
 * @since_tizen 2.4
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_focus_out_cb().
 *
 * @see inputmethod_event_set_focus_out_cb
 */
typedef void (*inputmethod_focus_out_cb)(int context_id, void *user_data);

/**
 * @brief Called when an associated text input UI control responds to a request with the surrounding text.
 *
 * @remarks The inputmethod_request_surrounding_text() must be called to invoke this callback function, asynchronously.
 *
 * @since_tizen 2.4
 *
 * @param[in] context_id The input context identification value of an associated text input UI control
 * @param[in] text The UTF-8 string requested
 * @param[in] cursor_pos The cursor position
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_surrounding_text_updated_cb().
 *
 * @see inputmethod_event_set_surrounding_text_updated_cb, inputmethod_request_surrounding_text
 */
typedef void (*inputmethod_surrounding_text_updated_cb)(int context_id, const char *text, int cursor_pos, void *user_data);

/**
 * @brief Called to reset the input context of an associated text input UI control.
 *
 * @since_tizen 2.4
 *
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_input_context_reset_cb().
 *
 * @see inputmethod_event_set_input_context_reset_cb
 */
typedef void (*inputmethod_input_context_reset_cb)(void *user_data);

/**
 * @brief Called when the position of the cursor in an associated text input UI control changes.
 *
 * @since_tizen 2.4
 *
 * @param[in] cursor_pos The cursor position
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_cursor_position_updated_cb().
 *
 * @see inputmethod_event_set_cursor_position_updated_cb
 */
typedef void (*inputmethod_cursor_position_updated_cb)(int cursor_pos, void *user_data);

/**
 * @brief Called when an associated text input UI control requests for the language of the input panel.
 *
 * @remarks The allocated @a lang_code will be freed internally.
 *
 * @since_tizen 2.4
 *
 * @param[in] user_data User data to be passed from the callback registration function
 * @param[out] lang_code Input panel's current input language code
 *
 * @pre The callback can be registered using inputmethod_event_set_language_requested_cb().
 *
 * @see inputmethod_event_set_language_requested_cb
 */
typedef void (*inputmethod_language_requested_cb)(void *user_data, char **lang_code);

/**
 * @brief Called to set the preferred language to the input panel.
 *
 * @remarks @a language information is already set to the input panel when it is shown
 * through #inputmethod_context_h. This callback function will be only called when the client
 * application changes the edit field's language attribute after the input panel is shown.
 *
 * @since_tizen 2.4
 *
 * @param[in] language The preferred language that the client wants
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_language_set_cb().
 *
 * @see inputmethod_event_set_language_set_cb
 */
typedef void (*inputmethod_language_set_cb)(Ecore_IMF_Input_Panel_Lang language, void *user_data);

/**
 * @brief Called to set the application specific data to deliver to the input panel.
 *
 * @details This API is used by applications to deliver specific data to the input panel.
 * The data format MUST be negotiated by both application and the input panel.
 *
 * @since_tizen 2.4
 *
 * @param[in] data The specific data to be set to the input panel
 * @param[in] data_len The length of data, in bytes, to send to the input panel
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_imdata_set_cb().
 *
 * @see inputmethod_event_set_imdata_set_cb
 */
typedef void (*inputmethod_imdata_set_cb)(void *data, unsigned int data_len, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to set its layout.
 *
 * @remarks @a layout information is already set to the input panel when it is shown
 * through #inputmethod_context_h. This callback function will be only called when the client
 * application changes the edit field's layout attribute after the input panel is shown.
 *
 * @since_tizen 2.4
 *
 * @param[in] layout The input panel layout
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_layout_set_cb().
 *
 * @see inputmethod_event_set_layout_set_cb
 */
typedef void (*inputmethod_layout_set_cb)(Ecore_IMF_Input_Panel_Layout layout, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to set the @c Return key label.
 * The input panel can show text or image on the @c Return button according to the @c Return key action.
 *
 * @remarks @a type information is already set to the input panel when it is shown
 * through #inputmethod_context_h. This callback function will be only called when the client
 * application changes the edit field's @c Return key type attribute after the input panel
 * is shown.
 *
 * @since_tizen 2.4
 *
 * @param[in] type The type of @c Return key on the input panel
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_return_key_type_set_cb().
 *
 * @see inputmethod_event_set_return_key_type_set_cb
 */
typedef void (*inputmethod_return_key_type_set_cb)(Ecore_IMF_Input_Panel_Return_Key_Type type, void *user_data);

/**
 * @brief Called when an associated text input UI control requests the input panel to enable
 * or disable the @c Return key state.
 *
 * @remarks @a disabled information is already set to the input panel when it is shown
 * through #inputmethod_context_h. This callback function will be only called when the client
 * application changes the edit field's @c Return key disable attribute after the input panel
 * is shown.
 *
 * @since_tizen 2.4
 *
 * @param[in] disabled The Boolean state to disable @c Return key. The @c Return key is enabled by default
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_return_key_state_set_cb().
 *
 * @see inputmethod_event_set_return_key_state_set_cb
 */
typedef void (*inputmethod_return_key_state_set_cb)(bool disabled, void *user_data);

/**
 * @brief Called when an associated text input UI control requests for the position
 * and size of the input panel.
 *
 * @since_tizen 2.4
 *
 * @param[in] user_data User data to be passed from the callback registration function
 * @param[out] x The x position in screen
 * @param[out] y The y position in screen
 * @param[out] w The window width
 * @param[out] h The window height
 *
 * @pre The callback can be registered using inputmethod_event_set_geometry_requested_cb().
 *
 * @see inputmethod_event_set_geometry_requested_cb
 */
typedef void (*inputmethod_geometry_requested_cb)(void *user_data, int *x, int *y, int *w, int *h);

/**
 * @brief Called when the system display language is changed.
 *
 * @since_tizen 2.4
 *
 * @param[in] language The language code
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_display_language_changed_cb().
 *
 * @see inputmethod_event_set_display_language_changed_cb
 */
typedef void (*inputmethod_display_language_changed_cb)(const char *language, void *user_data);

/**
 * @brief Called when the device is rotated.
 *
 * @since_tizen 2.4
 *
 * @param[in] degree The rotation degree
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_rotation_degree_changed_cb().
 *
 * @see inputmethod_event_set_rotation_degree_changed_cb
 */
typedef void (*inputmethod_rotation_degree_changed_cb)(int degree, void *user_data);

/**
 * @brief Called when Accessibility in Settings application is on or off.
 *
 * @since_tizen 2.4
 *
 * @param[in] state Accessibility option state
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_accessibility_state_changed_cb().
 *
 * @see inputmethod_event_set_accessibility_state_changed_cb
 */
typedef void (*inputmethod_accessibility_state_changed_cb)(bool state, void *user_data);

/**
 * @brief Called to create the option window.
 *
 * @remarks if Input panel requests to open the option window, @a type will be #INPUTMETHOD_OPTION_WINDOW_TYPE_KEYBOARD.
 * And if Settings application requests to open it, @a type will be #INPUTMETHOD_OPTION_WINDOW_TYPE_SETTING_APPLICATION.
 *
 * @since_tizen 2.4
 *
 * @param[in] window The created window object
 * @param[in] type The type of option window
 * @param[in] user_data User data to be passed from the callback registration function
 *
 * @pre The callback can be registered using inputmethod_event_set_option_window_created_cb(). The
 * inputmethod_create_option_window() calls this callback function or Settings application can call this callback function.
 *
 * @see inputmethod_event_set_option_window_created_cb, inputmethod_create_option_window
 */
typedef void (*inputmethod_option_window_created_cb)(Evas_Object *window, inputmethod_option_window_type_e type, void *user_data);

/**
 * @brief Called to destroy the option window.
 *
 * @since_tizen 2.4
 *
 * @param[in] window The window object to destroy
 * @param[in] user_data User data to be passed to the callback function
 *
 * @pre The callback can be registered using inputmethod_event_set_option_window_destroyed_cb().
 * inputmethod_destroy_option_window() calls this callback function.
 *
 * @see inputmethod_event_set_option_window_destroyed_cb
 */
typedef void (*inputmethod_option_window_destroyed_cb)(Evas_Object *window, void *user_data);

/**
 * @brief The structure type to contain the set of the essential callback functions for IME application lifecycle and appearance.
 *
 * @remarks These four callback functions are mandatory for IME application.
 *
 * @since_tizen 2.4
 *
 * @see inputmethod_run
 */
typedef struct
{
    inputmethod_create_cb create;       /**< Called when the input panel is created */
    inputmethod_terminate_cb terminate; /**< Called when the input panel is terminated */
    inputmethod_show_cb show;           /**< Called when the input panel is requested to show itself */
    inputmethod_hide_cb hide;           /**< Called when the input panel is requested to hide itself */
} inputmethod_callback_s;

/**
 * @brief Runs the main loop of IME application.
 *
 * @details This function starts to run IME application's main loop. The inputmethod_create_cb()
 * callback function is called to initialize IME application before the main loop starts up. And
 * the inputmethod_terminate_cb() callback function is called when IME application is terminated.
 *
 * @remarks IME application needs to implement inputmethod_app_main() function which is the main
 * entry point of IME application. In inputmethod_app_main() function, the inputmethod_run()
 * function MUST be called with the necessary callback functions; inputmethod_create_cb(),
 * inputmethod_terminate_cb(), inputmethod_show_cb(), and inputmethod_hide_cb() callback functions
 * are mandatory for IME application.
 *
 * @since_tizen 2.4

 * @param[in] basic_cb The structure pointer of the essential callback functions
 * @param[in] user_data User data to be passed to the callback functions
 *
 * @return 0 if IME application ends successfully, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NO_CALLBACK_FUNCTION Necessary callback function is not set
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The inputmethod_event_set_***() functions can be called to set the event handling callback functions.
 *
 * @see inputmethod_callback_s, inputmethod_event_set_focus_in_cb, inputmethod_event_set_focus_out_cb,
 * inputmethod_event_set_surrounding_text_updated_cb, inputmethod_event_set_input_context_reset_cb,
 * inputmethod_event_set_cursor_position_updated_cb, inputmethod_event_set_language_requested_cb,
 * inputmethod_event_set_language_set_cb, inputmethod_event_set_imdata_set_cb, inputmethod_event_set_layout_set_cb,
 * inputmethod_event_set_return_key_type_set_cb, inputmethod_event_set_return_key_state_set_cb,
 * inputmethod_event_set_geometry_requested_cb, inputmethod_event_set_display_language_changed_cb,
 * inputmethod_event_set_rotation_degree_changed_cb, inputmethod_event_set_accessibility_state_changed_cb,
 * inputmethod_event_set_option_window_created_cb, inputmethod_event_set_option_window_destroyed_cb
 *
 * @code
 static void ime_create_cb(void *user_data);
 static void ime_terminate_cb(void *user_data);
 static void ime_show_cb(int context_id, inputmethod_context_h context, void *user_data);
 static void ime_hide_cb(int context_id, void *user_data);
 static void ime_focus_in_cb(int context_id, void *user_data);
 static void ime_focus_out_cb(int context_id, void *user_data);
 static void ime_cursor_position_updated_cb(int cursor_pos, void *user_data);

 static void ime_create_cb(void *user_data)
 {
     Evas_Object *ime_win = NULL;

     inputmethod_set_size(480, 400, 800, 400);
     ime_win = inputmethod_get_main_window();
     if (ime_win) {
         // Prepare before showing IME window.
     }
 }

 static void ime_show_cb(int context_id, inputmethod_context_h context, void *user_data)
 {
     Ecore_IMF_Input_Panel_Layout layout;
     inputmethod_layout_variation_e layout_variation;
     Evas_Object *ime_win;

     inputmethod_context_get_layout(context, &layout);
     inputmethod_context_get_layout_variation(context, &layout_variation);

     ime_win = inputmethod_get_main_window();
     if (ime_win) {
         // Compose IME UI properly with the context information and show.

         evas_object_show(ime_win);
     }
 }

 static void ime_hide_cb(int context_id, void *user_data)
 {
     Evas_Object *ime_win = inputmethod_get_main_window();
     if (ime_win) {
         evas_object_hide(ime_win);
     }
 }

 void inputmethod_app_main(int argc, char **argv)
 {
     inputmethod_callback_s basic_callback = {
         ime_create_cb,
         ime_terminate_cb,
         ime_show_cb,
         ime_hide_cb,
     };

     inputmethod_event_set_focus_in_cb(ime_focus_in_cb, NULL);
     inputmethod_event_set_focus_out_cb(ime_focus_out_cb, NULL);
     inputmethod_event_set_cursor_position_updated_cb(ime_cursor_position_updated_cb, NULL);

     inputmethod_run(&basic_callback, NULL);
 }
 * @endcode
 */
EXPORT_API int inputmethod_run(inputmethod_callback_s *basic_cb, void *user_data);

/**
 * @brief Sets @c focus_in event callback function.
 *
 * @remarks The inputmethod_focus_in_cb() callback function is called when an associated text input
 * UI control has focus.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c focus_in event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_focus_in_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_focus_in_cb(inputmethod_focus_in_cb callback_func, void *user_data);

/**
 * @brief Sets @c focus_out event callback function.
 *
 * @remarks The inputmethod_focus_out_cb() callback function is called when an associated text input
 * UI control loses focus.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c focus_out event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_focus_out_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_focus_out_cb(inputmethod_focus_out_cb callback_func, void *user_data);

/**
 * @brief Sets @c surrounding_text_updated event callback function.
 *
 * @remarks The inputmethod_surrounding_text_updated_cb() callback function is called when an
 * associated text input UI control responds to a request with the surrounding text.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c surrounding_text_updated event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_surrounding_text_updated_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_surrounding_text_updated_cb(inputmethod_surrounding_text_updated_cb callback_func, void *user_data);

/**
 * @brief Sets #c input_context_reset event callback function.
 *
 * @remarks The inputmethod_input_context_reset_cb() callback function is called to reset the input
 * context of an associated text input UI control.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c input_context_reset event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_input_context_reset_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_input_context_reset_cb(inputmethod_input_context_reset_cb callback_func, void *user_data);

/**
 * @brief Sets @c cursor_position_updated event callback function.
 *
 * @remarks The inputmethod_cursor_position_updated_cb() callback function is called when the position
 * of the cursor in an associated text input UI control changes.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c cursor_position_updated event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_cursor_position_updated_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_cursor_position_updated_cb(inputmethod_cursor_position_updated_cb callback_func, void *user_data);

/**
 * @brief Sets @c language_requested event callback function.
 *
 * @remarks The inputmethod_language_requested_cb() callback function is called when an associated
 * text input UI control requests for the language of the input panel.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c language_requested event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_language_requested_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_language_requested_cb(inputmethod_language_requested_cb callback_func, void *user_data);

/**
 * @brief Sets @c language_set event callback function.
 *
 * @remarks The inputmethod_language_set_cb() callback function is called to set the preferred
 * language to the input panel.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c language_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_language_set_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_language_set_cb(inputmethod_language_set_cb callback_func, void *user_data);

/**
 * @brief Sets @c imdata_set event callback function.
 *
 * @remarks The inputmethod_imdata_set_cb() callback function is called to set the application
 * specific data to deliver to the input panel.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c imdata_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_imdata_set_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_imdata_set_cb(inputmethod_imdata_set_cb callback_func, void *user_data);

/**
 * @brief Sets @c layout_set event callback function.
 *
 * @remarks The inputmethod_layout_set_cb() callback function is called when an associated text input
 * UI control requests the input panel to set its layout.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c layout_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_layout_set_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_layout_set_cb(inputmethod_layout_set_cb callback_func, void *user_data);

/**
 * @brief Sets @c return_key_type_set event callback function.
 *
 * @remarks The inputmethod_return_key_type_set_cb() callback function is called when an associated
 * text input UI control requests the input panel to set the @c Return key label.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c return_key_type_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_return_key_type_set_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_return_key_type_set_cb(inputmethod_return_key_type_set_cb callback_func, void *user_data);

/**
 * @brief Sets @c return_key_state_set event callback function.
 *
 * @remarks The inputmethod_return_key_state_set_cb() callback function is called when an associated
 * text input UI control requests the input panel to enable or disable the @c Return key state.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c return_key_state_set event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_return_key_state_set_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_return_key_state_set_cb(inputmethod_return_key_state_set_cb callback_func, void *user_data);

/**
 * @brief Sets @c geometry_requested event callback function.
 *
 * @remarks The inputmethod_geometry_requested_cb() callback function is called when an associated
 * text input UI control requests for the position and size of the input panel.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c geometry_requested event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_geometry_requested_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_geometry_requested_cb(inputmethod_geometry_requested_cb callback_func, void *user_data);

/**
 * @brief Sets @c display_language_changed event callback function.
 *
 * @remarks The inputmethod_display_language_changed_cb() callback function is called when the system
 * display language is changed.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c display_language_changed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_display_language_changed_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_display_language_changed_cb(inputmethod_display_language_changed_cb callback_func, void *user_data);

/**
 * @brief Sets #c rotation_degree_changed event callback function.
 *
 * @remarks The inputmethod_rotation_degree_changed_cb() callback function is called when the device
 * is rotated.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c rotation_degree_changed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_rotation_degree_changed_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_rotation_degree_changed_cb(inputmethod_rotation_degree_changed_cb callback_func, void *user_data);

/**
 * @brief Sets @c accessibility_state_changed event callback function.
 *
 * @remarks The inputmethod_accessibility_state_changed_cb() callback function is called when
 * Accessibility in Settings application is on or off.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c accessibility_state_changed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_accessibility_state_changed_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_accessibility_state_changed_cb(inputmethod_accessibility_state_changed_cb callback_func, void *user_data);

/**
 * @brief Sets @c option_window_created event callback function.
 *
 * @remarks The inputmethod_option_window_created_cb() callback function is called to create the option window.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c option_window_created event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_option_window_created_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_option_window_created_cb(inputmethod_option_window_created_cb callback_func, void *user_data);

/**
 * @brief Sets @c option_window_destroyed event callback function.
 *
 * @remarks The inputmethod_option_window_destroyed_cb() callback function is called to destroy the option window.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback_func @c option_window_destroyed event callback function
 * @param[in] user_data User data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @post The inputmethod_run() function should be called to start to run IME application's main loop.
 *
 * @see inputmethod_option_window_destroyed_cb, inputmethod_run
 */
EXPORT_API int inputmethod_event_set_option_window_destroyed_cb(inputmethod_option_window_destroyed_cb callback_func, void *user_data);

/**
 * @brief Sends a key event directly to the associated text input UI control.
 *
 * @since_tizen 2.4
 *
 * @param[in] keycode The key code to be sent
 * @param[in] keymask The modifier key mask
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @see inputmethod_key_code_e, inputmethod_key_mask_e
 */
EXPORT_API int inputmethod_send_key_event(inputmethod_key_code_e keycode, inputmethod_key_mask_e keymask);

/**
 * @brief Sends the text to the associated text input UI control.
 *
 * @since_tizen 2.4
 *
 * @param[in] str The UTF-8 string to be committed
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @see inputmethod_show_preedit_string, inputmethod_hide_preedit_string, inputmethod_update_preedit_string
 */
EXPORT_API int inputmethod_commit_string(const char *str);

/**
 * @brief Requests to show preedit string.
 *
 * @since_tizen 2.4
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @see inputmethod_commit_string, inputmethod_hide_preedit_string, inputmethod_update_preedit_string
 */
EXPORT_API int inputmethod_show_preedit_string(void);

/**
 * @brief Requests to hide preedit string.
 *
 * @since_tizen 2.4
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @see inputmethod_commit_string, inputmethod_show_preedit_string, inputmethod_update_preedit_string
 */
EXPORT_API int inputmethod_hide_preedit_string(void);

/**
 * @brief Updates a new preedit string.
 *
 * @since_tizen 2.4
 *
 * @param[in] str The UTF-8 string to be updated in preedit
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @see inputmethod_commit_string, inputmethod_show_preedit_string, inputmethod_hide_preedit_string
 */
EXPORT_API int inputmethod_update_preedit_string(const char *str);

/**
 * @brief Requests the surrounding text from the position of the cursor, asynchronously.
 *
 * @since_tizen 2.4
 *
 * @param[in] maxlen_before The maximum length of string to be retrieved before the cursor; -1 means unlimited
 * @param[in] maxlen_after The maximum length of string to be retrieved after the cursor; -1 means unlimited
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NO_CALLBACK_FUNCTION Necessary callback function is not set
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @pre The inputmethod_surrounding_text_updated_cb() callback function MUST be set by inputmethod_event_set_surrounding_text_updated_cb().
 *
 * @post The requested surrounding text can be received using the inputmethod_surrounding_text_updated_cb() callback function.
 *
 * @see inputmethod_delete_surrounding_text, inputmethod_event_set_surrounding_text_updated_cb, inputmethod_surrounding_text_updated_cb
 */
EXPORT_API int inputmethod_request_surrounding_text(int maxlen_before, int maxlen_after);

/**
 * @brief Requests to delete surrounding text.
 *
 * @since_tizen 2.4
 *
 * @param[in] offset The offset value from the cursor position
 * @param[in] len The length of the text to delete
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @see inputmethod_request_surrounding_text
 */
EXPORT_API int inputmethod_delete_surrounding_text(int offset, int len);

/**
 * @brief This API returns the pointer of input panel main window.
 *
 * @remarks The specific error code can be obtained using the get_last_result() method if this function returns NULL.
 *
 * @since_tizen 2.4
 *
 * @return The input panel main window object on success, otherwise NULL
 *
 * @exception #INPUTMETHOD_ERROR_NONE Successful
 * #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 * #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @see inputmethod_create_cb, inputmethod_terminate_cb, inputmethod_show_cb, inputmethod_hide_cb
 */
EXPORT_API Evas_Object* inputmethod_get_main_window(void);

/**
 * @brief This API updates the input panel window's size information.
 *
 * @since_tizen 2.4
 *
 * @param[in] portrait_width The width in portrait mode
 * @param[in] portrait_height The height in portrait mode
 * @param[in] landscape_width The width in landscape mode
 * @param[in] landscape_height The height in landscape mode
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @see inputmethod_create_cb
 */
EXPORT_API int inputmethod_set_size(int portrait_width, int portrait_height, int landscape_width, int landscape_height);

/**
 * @brief Requests to create an option window from the input panel.
 *
 * @details The input panel can call this function to open the option window. This
 * function calls inputmethod_option_window_created_cb() callback function with
 * #INPUTMETHOD_OPTION_WINDOW_TYPE_KEYBOARD parameter.
 *
 * @since_tizen 2.4
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_NO_CALLBACK_FUNCTION Necessary callback function is not set
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 * @retval #INPUTMETHOD_ERROR_OPERATION_FAILED Operation failed
 *
 * @pre The inputmethod_option_window_created_cb() and inputmethod_option_window_destroyed_cb()
 * callback functions MUST be set by inputmethod_event_set_option_window_created_cb() and
 * inputmethod_event_set_option_window_destroyed_cb() respectively.
 *
 * @post This function calls inputmethod_option_window_created_cb() callback function to
 * create the option window. And inputmethod_destroy_option_window() function can be called
 * to close the option window.
 *
 * @see inputmethod_event_set_option_window_created_cb, inputmethod_option_window_created_cb, inputmethod_destroy_option_window
 */
EXPORT_API int inputmethod_create_option_window(void);

/**
 * @brief Requests to destroy an option window.
 *
 * @details The input panel can call this function to close the option window which
 * is created from either the input panel or Settings application.
 *
 * @since_tizen 2.4
 *
 * @param[in] window The option window to destroy
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NO_CALLBACK_FUNCTION Necessary callback function is not set
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @pre The inputmethod_option_window_created_cb() and inputmethod_option_window_destroyed_cb()
 * callback functions MUST be set by inputmethod_event_set_option_window_created_cb() and
 * inputmethod_event_set_option_window_destroyed_cb() respectively.
 *
 * @post This function calls inputmethod_option_window_destroyed_cb() callback function
 * to destroy the option window.
 *
 * @see inputmethod_event_set_option_window_destroyed_cb, inputmethod_option_window_destroyed_cb, inputmethod_create_option_window
 */
EXPORT_API int inputmethod_destroy_option_window(Evas_Object *window);

/**
 * @brief Gets the layout information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the layout information in inputmethod_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] layout Layout information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb, inputmethod_layout_set_cb
 */
EXPORT_API int inputmethod_context_get_layout(inputmethod_context_h context, Ecore_IMF_Input_Panel_Layout *layout);

/**
 * @brief Gets the layout variation information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the layout variation information in inputmethod_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] layout_variation Layout variation information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb, inputmethod_layout_variation_e
 */
EXPORT_API int inputmethod_context_get_layout_variation(inputmethod_context_h context, inputmethod_layout_variation_e *layout_variation);

/**
 * @brief Gets the cursor position information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the cursor position information in inputmethod_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] cursor_pos Cursor position information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb, inputmethod_cursor_position_updated_cb
 */
EXPORT_API int inputmethod_context_get_cursor_position(inputmethod_context_h context, int *cursor_pos);

/**
 * @brief Gets the autocapital type information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the autocapital type information in inputmethod_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] autocapital_type Autocapital type information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb
 */
EXPORT_API int inputmethod_context_get_autocapital_type(inputmethod_context_h context, Ecore_IMF_Autocapital_Type *autocapital_type);

/**
 * @brief Gets the @c Return key label type information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the @c Return key label type information in inputmethod_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] return_key_type The @c Return key label type information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb, inputmethod_return_key_type_set_cb
 */
EXPORT_API int inputmethod_context_get_return_key_type(inputmethod_context_h context, Ecore_IMF_Input_Panel_Return_Key_Type *return_key_type);

/**
 * @brief Gets the @c Return key state information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the @c Return key state information in inputmethod_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] return_key_state The @c Return key state information \n @c true to enable @c Return key
 * button, @c false to disable @c Return key button
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb, inputmethod_return_key_state_set_cb
 */
EXPORT_API int inputmethod_context_get_return_key_state(inputmethod_context_h context, bool *return_key_state);

/**
 * @brief Gets the prediction mode information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the prediction mode information in inputmethod_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] prediction_mode Prediction mode information \n @c true to allow the predictive
 * text feature if available, @c false to disable the predictive text feature
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb
 */
EXPORT_API int inputmethod_context_get_prediction_mode(inputmethod_context_h context, bool *prediction_mode);

/**
 * @brief Gets the password mode information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the password mode information in inputmethod_show_cb() callback function.
 *
 * @remarks If @a password_mode is @c true, the input panel is advised not to support the predictive text.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] password_mode Password mode information \n @c true to indicate that a password being inputted,
 * @c false to indicate non-password edit field.
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb
 */
EXPORT_API int inputmethod_context_get_password_mode(inputmethod_context_h context, bool *password_mode);

/**
 * @brief Gets the input hint information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the input hint information in inputmethod_show_cb() callback function.
 *
 * @remarks @a input_hint is a bit-wise value which recommends the input panel provide
 * an auto completion and so on if it is capable of supporting such features.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] input_hint Input hint information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb
 */
EXPORT_API int inputmethod_context_get_input_hint(inputmethod_context_h context, Ecore_IMF_Input_Hints *input_hint);

/**
 * @brief Gets the text bidirectional information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the bidirectional information in inputmethod_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] bidi Text bidirectional information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb
 */
EXPORT_API int inputmethod_context_get_bidi_direction(inputmethod_context_h context, Ecore_IMF_BiDi_Direction *bidi);

/**
 * @brief Gets the preferred language information from the given input context.
 *
 * @details Each edit field has various attributes for input panel. This function can be
 * called to get the preferred language information in inputmethod_show_cb() callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] context The input context information of an associated text input UI control
 * @param[out] language Preferred language information
 *
 * @return 0 on success, otherwise a negative error value
 * @retval #INPUTMETHOD_ERROR_NONE No error
 * @retval #INPUTMETHOD_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #INPUTMETHOD_ERROR_NOT_RUNNING IME main loop isn't started yet
 *
 * @post Input panel UI should be drawn or operated by this information accordingly.
 *
 * @see inputmethod_show_cb
 */
EXPORT_API int inputmethod_context_get_language(inputmethod_context_h context, Ecore_IMF_Input_Panel_Lang *language);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // __TIZEN_UIX_INPUTMETHOD_H__

 
