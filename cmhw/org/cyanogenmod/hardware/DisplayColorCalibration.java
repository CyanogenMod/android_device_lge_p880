/*
 * Copyright (C) 2013 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.cyanogenmod.hardware;

import org.cyanogenmod.hardware.util.FileUtils;

public class DisplayColorCalibration {
    private static final String GAMMA_TUNING_FILE = "/sys/devices/platform/spi_tegra.4/spi4.2/gamma_tuning";

    public static boolean isSupported() {
        return true;
    }

    public static int getMaxValue()  {
        return 255;
    }
    public static int getDefValue()  {
        return getMaxValue();
    }
    public static int getMinValue()  {
        return 0;
    }
    public static String getCurColors()  {
        return FileUtils.readOneLine(GAMMA_TUNING_FILE).replaceAll(","," ");
    }
    public static boolean setColors(String colors)  {
        if (!FileUtils.writeLine(GAMMA_TUNING_FILE, colors.replaceAll(" ",","))) {
            return false;
        }
        return true;
    }
}
