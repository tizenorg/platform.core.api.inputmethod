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

#ifndef __TIZEN_UI_INPUTMETHOD_H__
#define __TIZEN_UI_INPUTMETHOD_H__

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
 * @addtogroup CAPI_UI_INPUTMETHOD_MODULE
 * @{
 */

/**
 * @brief Enumeration for inputmethod function error.
 *
 * @since_tizen 2.4
 */
typedef enum
{
    INPUTMETHOD_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
    INPUTMETHOD_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
    INPUTMETHOD_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
    INPUTMETHOD_ERROR_OPERATION_FAILED = TIZEN_ERROR_UI_CLASS | 0x0001, /**< Operation failed */
} inputmethod_error_e;

/**
 * @brief Enumeration of the option window type.
 *
 * @since_tizen 2.4
 */
typedef enum
{
    INPUTMETHOD_OPTION_WINDOW_TYPE_KEYBOARD,  /**< Open from Keyboard */
    INPUTMETHOD_OPTION_WINDOW_TYPE_SETTING_APPLICATION, /**< Open from Setting application */
} inputmethod_option_window_type_e;

/**
 * @brief Structure of the edit field's input context.
 *
 * @details This is one of parameters of inputmethod_show_cb() callback function. IME application
 * should configure its keyboard panel with this structure information.
 *
 * @since_tizen 2.4
 */
typedef struct
{
    Ecore_IMF_Input_Panel_Layout layout;    /**< Input panel (keyboard) layout types */
    int layout_variation;                   /**< Layout variation */
    Ecore_IMF_Input_Panel_Return_Key_Type return_key_type;  /**< "Return" key types */
    Eina_Bool return_key_disabled;          /**< The state of "Return" key */
    Ecore_IMF_Autocapital_Type autocapital_type;    /**< Auto capital mode */
    int cursor_pos;                         /**< Cursor position in edit field */
    Eina_Bool prediction_allow;             /**< Boolean to allow predictive text */
    Eina_Bool password_mode;                /**< Password input mode of edit field */
    int imdata_size;                        /**< The size of application specific data to input panel */
    Ecore_IMF_Input_Hints input_hint;       /**< Edit field hint */
    Ecore_IMF_BiDi_Direction bidi_direction;/**< Bidirectional mode */
    Ecore_IMF_Input_Panel_Lang language;    /**< Preferred input language */
    unsigned int client_window;             /**< Client application window object */
} input_context_s;

/**
 * @brief Called when keyboard is created.
 *
 * @since_tizen 2.4
 *
 * @pre The callback must be registered using inputmethod_set_primitive_cb().
 *
 * @see inputmethod_set_primitive_cb, inputmethod_unset_primitive_cb
 */
typedef void (*inputmethod_create_cb)(void);

/**
 * @brief Called when keyboard is terminated.
 *
 * @since_tizen 2.4
 *
 * @pre The callback must be registered using inputmethod_set_primitive_cb().
 *
 * @see inputmethod_set_primitive_cb, inputmethod_unset_primitive_cb
 */
typedef void (*inputmethod_terminate_cb)(void);

/**
 * @brief Called when keyboard appears.
 *
 * @remarks IME application should configure its keyboard panel with #input_context_s structure information.
 *
 * @since_tizen 2.4
 *
 * @param[in] ic The input context identification value of each client edit field
 * @param[in] context The input context information
 *
 * @pre The callback must be registered using inputmethod_set_primitive_cb().
 *
 * @see inputmethod_set_primitive_cb, inputmethod_unset_primitive_cb
 */
typedef void (*inputmethod_show_cb)(int ic, input_context_s context);

/**
 * @brief Called when keyboard information requested.
 *
 * @since_tizen 2.4
 *
 * @param[in] ic The input context identification value of each client edit field
 *
 * @pre The callback must be registered using inputmethod_set_primitive_cb().
 *
 * @see inputmethod_set_primitive_cb, inputmethod_unset_primitive_cb
 */
typedef void (*inputmethod_hide_cb)(int ic);

/**
 * @brief Called when the input context has focus.
 *
 * @since_tizen 2.4
 *
 * @param[in] ic The input context identification value of each client edit field
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_focus_in_cb)(int ic);

/**
 * @brief Called when the input context loses focus.
 *
 * @since_tizen 2.4
 *
 * @param[in] ic The input context identification value of each client edit field
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_focus_out_cb)(int ic);

/**
 * @brief Called when the surrounding text is requested, asynchronously.
 *
 * @since_tizen 2.4
 *
 * @param[in] ic The input context identification value of each client edit field
 * @param[in] text The UTF-8 string requested
 * @param[in] cursor The cursor position
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 * inputmethod_get_surrounding_text() must be called to invoke this callback, asynchronously.
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb, inputmethod_get_surrounding_text
 */
typedef void (*inputmethod_update_surrounding_text_cb)(int ic, const char *text, int cursor);

/**
 * @brief Called to reset input context.
 *
 * @since_tizen 2.4
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_reset_input_context_cb)(void);

/**
 * @brief Called when cursor position is updated.
 *
 * @since_tizen 2.4
 *
 * @param[in] cursor_pos The cursor position
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_update_cursor_position_cb)(int cursor_pos);

/**
 * @brief Called to get input language locale from the input panel.
 *
 * @since_tizen 2.4
 *
 * @param[out] locale Input panel's current input language code
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_get_language_locale_cb)(char **locale);

/**
 * @brief Called to set the preferred language
 *
 * @remarks @language information is already set to the input panel when it is shown
 * through #input_context_s. This callback function will be only called when the client
 * application changes the edit field's language attribute after the input panel is shown.
 *
 * @since_tizen 2.4
 *
 * @param[in] language The preferred language that the client wants
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_set_language_cb)(Ecore_IMF_Input_Panel_Lang language);

/**
 * @brief Called to set the application specific data to deliver to the input panel.
 *
 * @details This API is used by applications to deliver specific data to the input panel.
 * The data format MUST be negotiated by both application and the input panel.
 *
 * @since_tizen 2.4
 *
 * @param[in] data The specific data to be set to the input panel
 * @param[in] len The length of data, in bytes, to send to the input panel
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_set_imdata_cb)(void *data, unsigned int len);

/**
 * @brief Called to set the layout of the input panel.
 *
 * @remarks @layout information is already set to the input panel when it is shown
 * through #input_context_s. This callback function will be only called when the client
 * application changes the edit field's layout attribute after the input panel is shown.
 *
 * @since_tizen 2.4
 *
 * @param[in] layout The input panel layout
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_set_layout_cb)(Ecore_IMF_Input_Panel_Layout layout);

/**
 * @brief Called to set return key type to the input panel.
 *
 * @remarks @type information is already set to the input panel when it is shown
 * through #input_context_s. This callback function will be only called when the client
 * application changes the edit field's return key type attribute after the input panel
 * is shown.
 *
 * @since_tizen 2.4
 *
 * @param[in] type The type of "return" key on the input panel
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_set_return_key_type_cb)(Ecore_IMF_Input_Panel_Return_Key_Type type);

/**
 * @brief Called to disable or enable return key to the input panel.
 *
 * @remarks @disabled information is already set to the input panel when it is shown
 * through #input_context_s. This callback function will be only called when the client
 * application changes the edit field's return key disable attribute after the input panel
 * is shown.
 *
 * @since_tizen 2.4
 *
 * @param[in] disabled The Boolean state to disable "return" key. The "return" key is enabled by default
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_set_return_key_disable_cb)(bool disabled);

/**
 * @brief Called to get the input panel window geometry.
 *
 * @since_tizen 2.4
 *
 * @param[in] x The x position in screen
 * @param[in] y The y position in screen
 * @param[in] w The window width
 * @param[in] h The window height
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_get_geometry_cb)(int *x, int *y, int *w, int *h);

/**
 * @brief Called when the system display language is changed.
 *
 * @since_tizen 2.4
 *
 * @param[in] language The language code
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_set_display_language_cb)(const char *language);

/**
 * @brief Called when the device is rotated.
 *
 * @since_tizen 2.4
 *
 * @param[in] degree The rotation degree
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_set_rotation_degree_cb)(int degree);

/**
 * @brief Called when Accessibility in Settings application is on or off.
 *
 * @since_tizen 2.4
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_set_accessibility_state_cb)(bool state);

/**
 * @brief Called to create keyboard option menu if available.
 *
 * @since_tizen 2.4
 *
 * @param[in] window The created window object
 * @param[in] type The type of option window
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 * inputmethod_create_option_window() calls this callback.
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb, inputmethod_create_option_window
 */
typedef void (*inputmethod_create_option_window_cb)(Evas_Object *window, inputmethod_option_window_type_e type);

/**
 * @brief Called to destroy keyboard option menu.
 *
 * @since_tizen 2.4
 *
 * @param[in] window The window object to destroy
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 * inputmethod_destroy_option_window() calls this callback.
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef void (*inputmethod_destroy_option_window_cb)(Evas_Object *window);

/**
 * @brief The structure type to contain the set of callback functions for handling application events.
 *
 * @since_tizen 2.4
 *
 * @pre The callback must be registered using inputmethod_set_extended_cb().
 *
 * @see inputmethod_set_primitive_cb, inputmethod_unset_primitive_cb
 */
typedef struct
{
    inputmethod_create_cb create;
    inputmethod_terminate_cb terminate;
    inputmethod_show_cb show;
    inputmethod_hide_cb hide;
} inputmethod_primitive_callback_s;

/**
 * @brief The structure type to contain the set of callback functions for handling application events.
 *
 * @since_tizen 2.4
 *
 * @pre It is one of the input parameters of the inputmethod_set_extended_cb() function.
 *
 * @see inputmethod_set_extended_cb, inputmethod_unset_extended_cb
 */
typedef struct
{
    inputmethod_focus_in_cb focus_in;
    inputmethod_focus_out_cb focus_out;
    inputmethod_update_surrounding_text_cb update_surrounding_text;
    inputmethod_reset_input_context_cb reset_input_context;
    inputmethod_update_cursor_position_cb update_cursor_position;
    inputmethod_get_language_locale_cb get_language_locale;
    inputmethod_set_language_cb set_language;
    inputmethod_set_imdata_cb set_imdata;
    inputmethod_set_layout_cb set_layout;
    inputmethod_set_return_key_type_cb set_return_key_type;
    inputmethod_set_return_key_disable_cb set_return_key_disable;
    inputmethod_get_geometry_cb get_geometry;
    inputmethod_set_display_language_cb set_display_language;
    inputmethod_set_rotation_degree_cb set_rotation_degree;
    inputmethod_set_accessibility_state_cb set_accessibility_state;
    inputmethod_create_option_window_cb create_option_window;
    inputmethod_destroy_option_window_cb destroy_option_window;
} inputmethod_extended_callback_s;

/**
 * @brief This function is the main entry point of the Tizen Native IME application.
 *
 * @details IME application should implement this function. In this function, the necessary
 * inputmethod callback functions need to be set by inputmethod_set_primitive_cb() and
 * inputmethod_set_extended_cb(), then inputmethod_run() must be called to start the main
 * loop of IME application.
 *
 * @remarks The inputmethod_create_cb() callback function is called by inputmethod_run()
 * before starting the main loop.
 *
 * @since_tizen 2.4
 *
 * @param[in] argc The argument count
 * @param[in] argv The argument vector
 *
 * @see inputmethod_run, inputmethod_set_primitive_cb, inputmethod_set_extended_cb, inputmethod_unset_primitive_cb, inputmethod_unset_extended_cb
 * @code
 static void ime_create_cb(void);
 static void ime_terminate_cb(void);
 static void ime_show_cb(int ic, input_context_s context);
 static void ime_hide_cb(int ic);
 static void ime_focus_out_cb(int ic);
 static void ime_focus_in_cb(int ic);

 inputmethod_primitive_callback_s inputmethod_primitive_callback = {
     ime_create_cb,
     ime_terminate_cb,
     ime_show_cb,
     ime_hide_cb
 };

 inputmethod_extended_callback_s inputmethod_extended_callback = {
     ime_focus_in_cb,
     ime_focus_out_cb,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL
 };

 void inputmethod_efl_main(int argc, char **argv)
 {
     inputmethod_set_primitive_cb(inputmethod_primitive_callback);
     inputmethod_set_extended_cb(inputmethod_extended_callback);

     inputmethod_run();

     inputmethod_unset_primitive_cb();
     inputmethod_unset_extended_cb();
 }
 * @endcode
 */
extern void inputmethod_efl_main(int argc, char **argv);

/**
 * @brief Registers primitive callback functions to be invoked when the inputmethod state changes.
 *
 * @remarks This function must be called before inputmethod_run(). The registered callback
 * functions can be unregistered by inputmethod_unset_primitive_cb().
 *
 * @since_tizen 2.4
 *
 * @param[in] callback The callback function to register
 *
 * @see inputmethod_unset_primitive_cb
 */
EXPORT_API void inputmethod_set_primitive_cb(inputmethod_primitive_callback_s callback);

/**
 * @brief Unregisters the primitive callback functions.
 *
 * @remarks This function must be called after inputmethod_run() and unregisters the callback
 * functions by inputmethod_set_primitive_cb().
 *
 * @since_tizen 2.4
 *
 * @see inputmethod_set_primitive_cb
 */
EXPORT_API void inputmethod_unset_primitive_cb(void);

/**
 * @brief Registers extended callback functions to be invoked when the inputmethod state changes.
 *
 * @remarks This function must be called before inputmethod_run(). The registered callback
 * functions can be unregistered by inputmethod_unset_extended_cb().
 *
 * @since_tizen 2.4
 *
 * @param[in] callback The callback function to register
 *
 * @see inputmethod_unset_extended_cb
 */
EXPORT_API void inputmethod_set_extended_cb(inputmethod_extended_callback_s callback);

/**
 * @brief Unregisters the extended callback function.
 *
 * @remarks This function must be called after inputmethod_run() and unregisters the callback
 * functions by inputmethod_set_extended_cb().
 *
 * @since_tizen 2.4
 *
 * @see inputmethod_set_extended_cb
 */
EXPORT_API void inputmethod_unset_extended_cb(void);

/**
 * @brief Runs the main loop.
 *
 * @details This function should be called just after all necessary callback functions are set by
 * inputmethod_set_primitive_cb() and inputmethod_set_extended_cb(). Then inputmethod_create_cb()
 * callback function will be called to initialize and inputmethod_terminate_cb() callback function
 * will be called when IME application is terminated.
 *
 * @since_tizen 2.4
 *
 * @see inputmethod_create_cb, inputmethod_terminate_cb, inputmethod_set_primitive_cb, inputmethod_set_extended_cb
 */
EXPORT_API void inputmethod_run(void);

/**
 * @brief Sends a KeyEvent to client application.
 *
 * @since_tizen 2.4
 *
 * @param[in] keycode The key event to be sent
 * @param[in] keymask The modifier key mask
 */
EXPORT_API void inputmethod_send_key_event(inputmethod_key_code_e keycode, inputmethod_key_mask_e keymask);

/**
 * @brief Commits a string to client application directly.
 *
 * @since_tizen 2.4
 *
 * @param[in] str The UTF-8 string to be committed
 */
EXPORT_API void inputmethod_commit_string(const char *str);

/**
 * @brief Requests to show preedit string.
 *
 * @since_tizen 2.4
 *
 * @see inputmethod_hide_preedit_string, inputmethod_update_preedit_string
 */
EXPORT_API void inputmethod_show_preedit_string(void);

/**
 * @brief Requests to hide preedit string.
 *
 * @since_tizen 2.4
 *
 * @see inputmethod_show_preedit_string, inputmethod_update_preedit_string
 */
EXPORT_API void inputmethod_hide_preedit_string(void);

/**
 * @brief Updates a new preedit string.
 *
 * @since_tizen 2.4
 *
 * @param[in] str The UTF-8 string to be updated in preedit
 *
 * @see inputmethod_show_preedit_string, inputmethod_hide_preedit_string
 */
EXPORT_API void inputmethod_update_preedit_string(const char *str);

/**
 * @brief Requests to get surrounding text, asynchronously.
 *
 * @since_tizen 2.4
 *
 * @param[in] maxlen_before The maximum length of context string to be retrieved before the cursor
 * @param[in] maxlen_after The maximum length of context string to be retrieved after the cursor
 *
 * @post inputmethod_update_surrounding_text_cb() callback function will be called with the requested text.
 *
 * @see inputmethod_delete_surrounding_text, inputmethod_update_surrounding_text_cb
 */
EXPORT_API void inputmethod_get_surrounding_text(int maxlen_before, int maxlen_after);

/**
 * @brief Requests to delete surrounding text.
 *
 * @since_tizen 2.4
 *
 * @param[in] offset The offset for cursor position
 * @param[in] len The length for delete text
 *
 * @see inputmethod_get_surrounding_text
 */
EXPORT_API void inputmethod_delete_surrounding_text(int offset, int len);

/**
 * @brief This API returns the pointer to main keyboard window
 *
 * @since_tizen 2.4
 *
 * @return The keyboard main window object on success, otherwise NULL
 */
EXPORT_API Evas_Object* inputmethod_get_main_window(void);

/**
 * @brief This API updates the keyboard window's geometry information
 *
 * @since_tizen 2.4
 *
 * @param[in] portrait_width The width in portrait mode
 * @param[in] portrait_height The height in portrait mode
 * @param[in] landscape_width The width in landscape mode
 * @param[in] landscape_height The height in landscape mode
 */
EXPORT_API void inputmethod_set_size_hints(int portrait_width, int portrait_height, int landscape_width, int landscape_height);

/**
 * @brief Requests to create an option window
 *
 * @details The input panel can call this function to open the option window. This
 * function calls inputmethod_create_option_window_cb() callback function with
 * #INPUTMETHOD_OPTION_WINDOW_TYPE_KEYBOARD parameter.
 *
 * @since_tizen 2.4
 *
 * @return The option window on success, otherwise NULL
 *
 * @post inputmethod_destroy_option_window() function can be called to close the
 * option window.
 *
 * @see inputmethod_destroy_option_window, inputmethod_create_option_window_cb
 */
EXPORT_API Evas_Object* inputmethod_create_option_window(void);

/**
 * @brief Requests to destroy an option window
 *
 * @details The input panel can call this function to close the option window which
 * is created from both the input panel and Settings application.
 *
 * @since_tizen 2.4
 *
 * @param[in] window The option window to destroy
 *
 * @post This function calls inputmethod_destroy_option_window_cb() callback function
 * to destroy the option window.
 *
 * @see inputmethod_create_option_window, inputmethod_destroy_option_window_cb
 */
EXPORT_API void inputmethod_destroy_option_window(Evas_Object *window);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // __TIZEN_UI_INPUTMETHOD_H__

