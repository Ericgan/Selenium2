// Copyright 2011 Software Freedom Conservancy
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WEBDRIVER_IE_CLOSEWINDOWCOMMANDHANDLER_H_
#define WEBDRIVER_IE_CLOSEWINDOWCOMMANDHANDLER_H_

#include "../Browser.h"
#include "../IECommandHandler.h"
#include "../IECommandExecutor.h"

namespace webdriver {

class CloseWindowCommandHandler : public IECommandHandler {
 public:
  CloseWindowCommandHandler(void) {
  }

  virtual ~CloseWindowCommandHandler(void) {
  }

 protected:
  void ExecuteInternal(const IECommandExecutor& executor,
                       const LocatorMap& locator_parameters,
                       const ParametersMap& command_parameters,
                       Response* response) {
    // The session should end if the user sends a quit command,
    // or if the user sends a close command with exactly 1 window
    // open, per spec. Removing the window from the managed browser
    // list depends on events, which may be asynchronous, so cache
    // the window count *before* closing the current window.
    size_t current_window_count = executor.managed_window_count();

    // TODO: Check HRESULT values for errors.
    BrowserHandle browser_wrapper;
    int status_code = executor.GetCurrentBrowser(&browser_wrapper);
    if (status_code != SUCCESS) {
      response->SetErrorResponse(status_code, "Unable to get browser");
      return;
    }
    browser_wrapper->Close();

    if (current_window_count == 1) {
      IECommandExecutor& mutable_executor = const_cast<IECommandExecutor&>(executor);
      mutable_executor.set_is_valid(false);
      mutable_executor.set_is_quitting(true);
    }
    response->SetSuccessResponse(Json::Value::null);
  }
};

} // namespace webdriver

#endif // WEBDRIVER_IE_CLOSEWINDOWCOMMANDHANDLER_H_
