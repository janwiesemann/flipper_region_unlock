#include "flipper_region_unlock.h"

#include <extra_beacon.h>
#include <furi_hal_version.h>

#include <string.h>

#define TAG "FlipperRegionUnlock "
const FuriHalRegion unlockedRegion = {
    .country_code = UNLOCKED_REGION_CODE,
    .bands_count = 3,
    .bands =
        {
            {
                .start = 299999755,
                .end = 348000000,
                .power_limit = 12,
                .duty_cycle = 50,
            },
            {
                .start = 386999938,
                .end = 464000000,
                .power_limit = 12,
                .duty_cycle = 50,
            },
            {
                .start = 778999847,
                .end = 928000000,
                .power_limit = 12,
                .duty_cycle = 50,
            },
        },
};

static bool flipper_region_unlock_custom_event_callback(void *context,
                                                        uint32_t event) {
  furi_assert(context);
  FlippeUnlockRegionApp *app = context;
  return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool flipper_region_unlock_back_event_callback(void *context) {
  furi_assert(context);
  FlippeUnlockRegionApp *app = context;
  return scene_manager_handle_back_event(app->scene_manager);
}

static void flipper_region_unlock_tick_event_callback(void *context) {
  furi_assert(context);
  FlippeUnlockRegionApp *app = context;
  scene_manager_handle_tick_event(app->scene_manager);
}

static FlippeUnlockRegionApp *flipper_region_unlock_alloc(void) {
  FlippeUnlockRegionApp *app = malloc(sizeof(FlippeUnlockRegionApp));

  app->gui = furi_record_open(RECORD_GUI);

  app->scene_manager =
      scene_manager_alloc(&flipper_region_unlock_scene_handlers, app);
  app->view_dispatcher = view_dispatcher_alloc();

  view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
  view_dispatcher_set_custom_event_callback(
      app->view_dispatcher, flipper_region_unlock_custom_event_callback);
  view_dispatcher_set_navigation_event_callback(
      app->view_dispatcher, flipper_region_unlock_back_event_callback);
  view_dispatcher_set_tick_event_callback(
      app->view_dispatcher, flipper_region_unlock_tick_event_callback, 100);
  view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui,
                                ViewDispatcherTypeFullscreen);
  view_dispatcher_enable_queue(app->view_dispatcher);

  app->mainScreen = dialog_ex_alloc();
  app->mainScreenStatusString = furi_string_alloc();
  view_dispatcher_add_view(app->view_dispatcher, FlipperRegionUnlockAppViewMain,
                           dialog_ex_get_view(app->mainScreen));

  app->bandsScreen = text_box_alloc();
  app->bandsScreenStatusString = furi_string_alloc();
  view_dispatcher_add_view(app->view_dispatcher,
                           FlipperRegionUnlockAppViewBands,
                           text_box_get_view(app->bandsScreen));

  return app;
}

static void flipper_region_unlock_free(FlippeUnlockRegionApp *app) {
  view_dispatcher_remove_view(app->view_dispatcher,
                              FlipperRegionUnlockAppViewMain);

  free(app->mainScreen);
  free(app->mainScreenStatusString);

  free(app->bandsScreen);
  free(app->bandsScreenStatusString);

  free(app->scene_manager);
  free(app->view_dispatcher);

  furi_record_close(RECORD_NOTIFICATION);
  furi_record_close(RECORD_GUI);
  app->gui = NULL;

  free(app);
}

int32_t flipper_region_unlock_main(void *args) {
  UNUSED(args);

  FlippeUnlockRegionApp *app = flipper_region_unlock_alloc();

  scene_manager_next_scene(app->scene_manager, FlipperRegionUnlockAppViewMain);

  view_dispatcher_run(app->view_dispatcher);

  flipper_region_unlock_free(app);
  return 0;
}
