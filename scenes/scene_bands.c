#include "../flipper_region_unlock.h"

static void update_status_text(FlippeUnlockRegionApp *app) {

  const FuriHalRegion *currentRegion = furi_hal_region_get();

  furi_string_reset(app->bandsScreenStatusString);

  furi_string_cat(app->bandsScreenStatusString, "Region settings:\n");
  furi_string_cat_printf(app->bandsScreenStatusString, "  Region: %s\n",
                         currentRegion->country_code);
  furi_string_cat_printf(app->bandsScreenStatusString, "  Bands: %d",
                         currentRegion->bands_count);

  for (int i = 0; i < currentRegion->bands_count; i++) {
    furi_string_cat_printf(app->bandsScreenStatusString, "\n    - %lu-%lukhz",
                           currentRegion->bands[i].start / 1000,
                           currentRegion->bands[i].end / 1000);
  }

  text_box_set_text(app->bandsScreen,
                    furi_string_get_cstr(app->bandsScreenStatusString));
}

void flipper_region_unlock_scene_bands_on_enter(void *context) {
  FlippeUnlockRegionApp *app = context;

  update_status_text(app);

  view_dispatcher_switch_to_view(app->view_dispatcher,
                                 FlipperRegionUnlockAppViewBands);
}

bool flipper_region_unlock_scene_bands_on_event(void *context,
                                                SceneManagerEvent event) {
  UNUSED(context);
  UNUSED(event);

  return false;
}

void flipper_region_unlock_scene_bands_on_exit(void *context) {
  UNUSED(context);
}
