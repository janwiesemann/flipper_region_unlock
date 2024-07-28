#include <power/power_service/power.h>

#include "../flipper_region_unlock.h"

static void
flipper_region_unlock_scene_main_confirm_dialog_callback(DialogExResult result,
                                                         void *context) {
  FlippeUnlockRegionApp *app = context;

  view_dispatcher_send_custom_event(app->view_dispatcher, result);
}

static void update_status_text(FlippeUnlockRegionApp *app,
                               const FuriHalRegion *currentRegion,
                               const bool isUnlocked) {
  furi_string_reset(app->mainScreenStatusString);

  furi_string_cat_printf(app->mainScreenStatusString, "Country: %s\n",
                         currentRegion->country_code);
  furi_string_cat_printf(app->mainScreenStatusString, "Bands: %u",
                         currentRegion->bands_count);

  if (isUnlocked)
    furi_string_cat_str(app->mainScreenStatusString,
                        "\nReboot Flipper to reset.");

  dialog_ex_set_text(app->mainScreen,
                     furi_string_get_cstr(app->mainScreenStatusString), 0, 29,
                     AlignLeft, AlignCenter);
}

static void update_status(FlippeUnlockRegionApp *app) {

  const FuriHalRegion *currentRegion = furi_hal_region_get();
  const bool isUnlocked =
      strcmp(currentRegion->country_code, UNLOCKED_REGION_CODE) == 0;

  dialog_ex_set_header(app->mainScreen, "Flipper Region Unlock", 64, 0,
                       AlignCenter, AlignTop);

  update_status_text(app, currentRegion, isUnlocked);

  if (isUnlocked) {
    dialog_ex_set_left_button_text(app->mainScreen, "Reboot Flipper");
    dialog_ex_set_center_button_text(app->mainScreen, NULL);
  } else {
    dialog_ex_set_left_button_text(app->mainScreen, NULL);
    dialog_ex_set_center_button_text(app->mainScreen, "Region");
  }

  dialog_ex_set_right_button_text(app->mainScreen, "Bands");

  dialog_ex_set_result_callback(
      app->mainScreen,
      flipper_region_unlock_scene_main_confirm_dialog_callback);
  dialog_ex_set_context(app->mainScreen, app);
}

void flipper_region_unlock_scene_main_on_enter(void *context) {
  FlippeUnlockRegionApp *regionUnlock = context;

  update_status(regionUnlock);

  view_dispatcher_switch_to_view(regionUnlock->view_dispatcher,
                                 FlipperRegionUnlockAppViewMain);
}

bool flipper_region_unlock_scene_main_on_event(void *context,
                                               SceneManagerEvent event) {
  FlippeUnlockRegionApp *app = context;

  if (event.type == SceneManagerEventTypeCustom) {
    if (event.event == DialogExResultLeft) {
      power_reboot(PowerBootModeNormal);
      return true;
    } else if (event.event == DialogExResultCenter) {
      const size_t regionStructSize =
          sizeof(FuriHalRegion) +
          sizeof(FuriHalRegionBand) * unlockedRegion.bands_count;
      FuriHalRegion *region = malloc(regionStructSize);
      memcpy(region, &unlockedRegion, regionStructSize);
      furi_hal_region_set(region);
      update_status(app);

      return true;
    } else if (event.event == DialogExResultRight) {
      scene_manager_next_scene(app->scene_manager,
                               FlipperRegionUnlockAppViewBands);
      return true;
    }
  }
  return false;
}

void flipper_region_unlock_scene_main_on_exit(void *context) {
  UNUSED(context);
}
