/*
 * A dummy radio notification handler.
*/

#pragma once

#include "srsran/radio/radio_notification_handler.h"

namespace srsran{

class radio_notification_handler_dummy : public radio_notification_handler
{
public:
    void on_radio_rt_event(const event_description& description) override { return; }
};

}