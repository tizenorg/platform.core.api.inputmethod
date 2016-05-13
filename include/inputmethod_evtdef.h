/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_UIX_INPUTMETHOD_EVTDEF_H__
#define __TIZEN_UIX_INPUTMETHOD_EVTDEF_H__

/**
 * @file inputmethod_evtdef.h
 * @brief This file contains definitions for unconventional input device events.
 */

/**
 * @addtogroup CAPI_UIX_INPUTMETHOD_MODULE
 * @{
 */

/**
 * @brief Enumeration of directions for rotaty input device's rotation event
 *
 * @since_tizen 3.0
 *
 * @see ime_input_device_event_rotary_s
 */
typedef enum
{
    IME_INPUT_DEVICE_ROTARY_DIRECTION_CLOCKWISE, /**< Rotary is rotated clockwise direction */
    IME_INPUT_DEVICE_ROTARY_DIRECTION_COUNTER_CLOCKWISE /**< Rotary is rotated counter clockwise direction */
} ime_input_device_rotary_direction_e;

/**
 * @brief Rotary input device event information structure
 *
 * If the dev_type parameter of the ime_process_input_device_event_cb function indicates
 * the current input device type is IME_INPUT_DEVICE_TYPE_ROTARY, then the input_data parameter
 * should be casted into ime_rotary_input_device_event_s type, as shown in the example code.
 *
 * @since_tizen 3.0
 *
 * @see ime_process_input_device_event_cb, ime_rotary_input_device_direction_e
 *
 * @code
 static bool inputmethod_process_input_device_event_cb(ime_input_device_type_e dev_type, void *input_data);
 {
    if (dev_type == IME_INPUT_DEVICE_TYPE_ROTARY) {
        ime_input_device_event_rotary_s *rotary_event = static_cast<ime_input_device_event_rotary_s*>(input_data);
        if (rotary_event) {
            printf("%d %d", rotary_event->direction, rotary_event->timestamp);
        }
        return true;
    }
    return false;
 }
 * @endcode
 */
typedef struct
{
    ime_input_device_rotary_direction_e direction; /**< Direction of rotary input device event */
    unsigned int time_stamp; /**< Timestamp of rotary input device event */
} ime_input_device_event_rotary_s;

/**
 * @}
 */

#endif /* __TIZEN_UIX_INPUTMETHOD_EVTDEF_H__ */

