/**
 * @file ble_beacon_app.h
 * @brief BLE beacon example.
 */
#pragma once

#include "extra_beacon.h"
#include <furi.h>
#include <gui/gui.h>
#include <gui/scene_manager.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>

#include <gui/modules/dialog_ex.h>
#include <gui/modules/text_box.h>
#include <gui/modules/widget.h>

#include <notification/notification_messages.h>
#include <rpc/rpc_app.h>

#include <furi_hal_bt.h>

#include "scenes/scenes.h"
#include <stdint.h>

#include <furi_hal_region.h>

#define UNLOCKED_REGION_CODE "HAX"

typedef struct {
  Gui *gui;
  SceneManager *scene_manager;
  ViewDispatcher *view_dispatcher;

  DialogEx *mainScreen;
  FuriString *mainScreenStatusString;

  TextBox *bandsScreen;
  FuriString *bandsScreenStatusString;
} FlippeUnlockRegionApp;

typedef enum {
  FlipperRegionUnlockAppViewMain,
  FlipperRegionUnlockAppViewBands,
} FlipperRegionUnlockAppView;

extern const FuriHalRegion unlockedRegion;
