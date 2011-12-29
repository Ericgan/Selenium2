/*
Copyright 2011 WebDriver committers
Copyright 2011 Software Freedom Conservancy

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package org.openqa.selenium.testing.drivers;

import com.google.common.base.Supplier;
import com.google.common.base.Throwables;

import org.openqa.selenium.Capabilities;
import org.openqa.selenium.WebDriver;

import java.lang.reflect.InvocationTargetException;
import java.util.logging.Logger;

import static org.openqa.selenium.testing.DevMode.isInDevMode;

public class DefaultDriverSupplier implements Supplier<WebDriver> {

  private static final Logger log = Logger.getLogger(DefaultDriverSupplier.class.getName());
  private Class<? extends WebDriver> driverClass;
  private final Capabilities capabilities;

  public DefaultDriverSupplier(Capabilities capabilities) {
    this.capabilities = capabilities;

    try {
      // Only support a default driver if we're actually in dev mode.
      if (isInDevMode()) {
        driverClass = Class.forName("org.openqa.selenium.firefox.SynthesizedFirefoxDriver")
            .asSubclass(WebDriver.class);
      } else {
        driverClass = null;
      }
    } catch (ClassNotFoundException e) {
      log.severe("Unable to find the default class on the classpath. Tests will fail");
    }
  }

  public WebDriver get() {
    log.info("Providing default driver instance");

    try {
      return driverClass.getConstructor(Capabilities.class).newInstance(capabilities);
    } catch (InstantiationException e) {
      throw Throwables.propagate(e);
    } catch (IllegalAccessException e) {
      throw Throwables.propagate(e);
    } catch (NoSuchMethodException e) {
      throw Throwables.propagate(e);
    } catch (InvocationTargetException e) {
      throw Throwables.propagate(e);
    }
  }
}
