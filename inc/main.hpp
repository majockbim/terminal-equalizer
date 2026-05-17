/*
    spectrum - A real-time command line audio visualizer
    Copyright (C) 2026 Majock Bim

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <cstddef>
#include <csignal>
#include <atomic>
#include <windows.h>
#include <consoleapi3.h>
#include <thread>

#include "../inc/ui/visualizer.hpp"
#include "../inc/processing/signal_processor.hpp"
#include "../inc/math/fft.hpp"
#include "../inc/settings/settings.hpp"