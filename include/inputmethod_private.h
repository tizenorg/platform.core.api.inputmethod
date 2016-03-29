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

#ifndef __TIZEN_UIX_INPUTMETHOD_PRIVATE_H__
#define __TIZEN_UIX_INPUTMETHOD_PRIVATE_H__

struct _ime_context {
	Ecore_IMF_Input_Panel_Layout layout;    /**< Input panel (keyboard) layout types */
	int layout_variation;                   /**< Layout variation */
	int cursor_pos;                         /**< Cursor position in edit field */
	Ecore_IMF_Autocapital_Type autocapital_type;    /**< Auto capital mode */
	Ecore_IMF_Input_Panel_Return_Key_Type return_key_type;  /**< "Return" key types */
	Eina_Bool return_key_disabled;          /**< The state of "Return" key */
	Eina_Bool prediction_allow;             /**< Boolean to allow predictive text */
	Eina_Bool password_mode;                /**< Password input mode of edit field */
	int imdata_size;                        /**< The size of application specific data to input panel */
	Ecore_IMF_Input_Hints input_hint;       /**< Edit field hint */
	Ecore_IMF_BiDi_Direction bidi_direction;/**< Bidirectional mode */
	Ecore_IMF_Input_Panel_Lang language;    /**< Preferred input language */
	unsigned int client_window;             /**< Client application window object */
};

struct _ime_device_info {
	const char *dev_name;                   /**< The device name */
	Ecore_IMF_Device_Class dev_class;       /**< The device class */
	Ecore_IMF_Device_Subclass dev_subclass; /**< The device subclass */
};

#endif /* __TIZEN_UIX_INPUTMETHOD_PRIVATE_H__ */

