<?php
/**
 * @file
 * Default delta templates
 */
/**
 * Implements hook_delta_default_templates()
 */
function npt_blocks_delta_default_templates() {
  $export = array();
  
  $delta = new stdClass();
  $delta->disabled = FALSE; /* Edit this to true to make a default delta disabled initially */
  $delta->api_version = 3;
  $delta->machine_name = 'front_page';
  $delta->name = 'Front page';
  $delta->description = 'Front page for NPT Startup';
  $delta->theme = 'gbifwhite';
  $delta->mode = 'preserve';
  $delta->parent = '';
  $delta->settings = array(
    'theme_gbifwhite_settings' => array(
      'toggle_logo' => 1,
      'toggle_name' => 1,
      'toggle_slogan' => 1,
      'toggle_node_user_picture' => 1,
      'toggle_comment_user_picture' => 1,
      'toggle_comment_user_verification' => 1,
      'toggle_favicon' => 1,
      'toggle_main_menu' => 0,
      'toggle_secondary_menu' => 0,
      'toggle_gbif_footer' => 0,
      'toggle_gbif_credit' => 0,
      'default_logo' => 0,
      'logo_path' => 'sites/all/themes/gbifwhite/logo.svg',
      'logo_upload' => '',
      'default_favicon' => 1,
      'favicon_path' => '',
      'favicon_upload' => '',
      'alpha_grid' => 'alpha_default',
      'alpha_responsive' => 1,
      'alpha_viewport' => 1,
      'alpha_viewport_initial_scale' => '1',
      'alpha_viewport_min_scale' => '1',
      'alpha_viewport_max_scale' => '1',
      'alpha_viewport_user_scaleable' => 0,
      'alpha_primary_alpha_default' => 'normal',
      'alpha_layouts_alpha_default_normal_responsive' => 1,
      'alpha_layouts_alpha_default_normal_weight' => '2',
      'alpha_layouts_alpha_default_normal_media' => 'all and (min-width: 980px) and (min-device-width: 980px), all and (max-device-width: 1024px) and (min-width: 1024px) and (orientation:landscape)',
      'alpha_layouts_alpha_default_fluid_responsive' => 0,
      'alpha_layouts_alpha_default_fluid_weight' => '0',
      'alpha_layouts_alpha_default_fluid_media' => 'all and (min-width: 740px) and (min-device-width: 740px), (max-device-width: 800px) and (min-width: 740px) and (orientation:landscape)',
      'alpha_layouts_alpha_default_narrow_responsive' => 0,
      'alpha_layouts_alpha_default_narrow_weight' => '1',
      'alpha_layouts_alpha_default_narrow_media' => 'all and (min-width: 740px) and (min-device-width: 740px), (max-device-width: 800px) and (min-width: 740px) and (orientation:landscape)',
      'alpha_layouts_alpha_default_wide_responsive' => 0,
      'alpha_layouts_alpha_default_wide_weight' => '3',
      'alpha_layouts_alpha_default_wide_media' => 'all and (min-width: 1220px)',
      'alpha_primary_alpha_fluid' => 'normal',
      'alpha_layouts_alpha_fluid_normal_responsive' => 0,
      'alpha_layouts_alpha_fluid_normal_weight' => '0',
      'alpha_layouts_alpha_fluid_normal_media' => 'all and (min-width: 740px) and (min-device-width: 740px), (max-device-width: 800px) and (min-width: 740px) and (orientation:landscape)',
      'alpha_libraries' => array(
        'omega_formalize' => 'omega_formalize',
        'omega_mediaqueries' => 'omega_mediaqueries',
        'omega_equalheights' => 0,
      ),
      'alpha_css' => array(
        'alpha-reset.css' => 'alpha-reset.css',
        'alpha-mobile.css' => 'alpha-mobile.css',
        'alpha-alpha.css' => 'alpha-alpha.css',
        'omega-text.css' => 'omega-text.css',
        'omega-branding.css' => 'omega-branding.css',
        'omega-menu.css' => 'omega-menu.css',
        'omega-forms.css' => 'omega-forms.css',
        'omega-visuals.css' => 'omega-visuals.css',
        'global.css' => 'global.css',
        'banner.css' => 0,
      ),
      'alpha_exclude' => array(
        'modules/aggregator/aggregator.css' => 0,
        'modules/comment/comment.css' => 0,
        'sites/all/modules/contrib/date/date_api/date.css' => 0,
        'sites/all/modules/custom/field_quick_delete/theme/field.css' => 0,
        'modules/node/node.css' => 0,
        'sites/all/modules/custom/gbifs/npt_setup/css/npt-setup.css' => 0,
        'modules/search/search.css' => 0,
        'sites/all/modules/custom/twitterscript/css/twitterscript.css' => 0,
        'modules/user/user.css' => 0,
        'sites/all/modules/contrib/views/css/views.css' => 0,
        'misc/vertical-tabs.css' => 0,
        'modules/block/block.css' => 0,
        'modules/dblog/dblog.css' => 0,
        'modules/file/file.css' => 0,
        'modules/filter/filter.css' => 0,
        'modules/help/help.css' => 0,
        'modules/menu/menu.css' => 0,
        'modules/openid/openid.css' => 0,
        'modules/profile/profile.css' => 0,
        'modules/statistics/statistics.css' => 0,
        'modules/syslog/syslog.css' => 0,
        'modules/system/admin.css' => 0,
        'modules/system/maintenance.css' => 0,
        'modules/system/system.css' => 0,
        'modules/system/system.admin.css' => 0,
        'modules/system/system.base.css' => 0,
        'modules/system/system.maintenance.css' => 0,
        'modules/system/system.menus.css' => 0,
        'modules/system/system.messages.css' => 0,
        'modules/system/system.theme.css' => 0,
        'modules/taxonomy/taxonomy.css' => 0,
        'modules/tracker/tracker.css' => 0,
        'modules/update/update.css' => 0,
      ),
      'alpha_debug_block_toggle' => 0,
      'alpha_debug_block_active' => 0,
      'alpha_debug_grid_toggle' => 0,
      'alpha_debug_grid_active' => 0,
      'alpha_debug_grid_roles' => array(
        3 => '3',
        1 => 0,
        2 => 0,
        4 => 0,
        5 => 0,
      ),
      'alpha_toggle_messages' => 1,
      'alpha_toggle_action_links' => 1,
      'alpha_toggle_tabs' => 1,
      'alpha_toggle_breadcrumb' => 1,
      'alpha_toggle_page_title' => 1,
      'alpha_toggle_feed_icons' => 1,
      'alpha_hidden_title' => 0,
      'alpha_hidden_site_name' => 0,
      'alpha_hidden_site_slogan' => 0,
      'alpha_region_dashboard_sidebar_force' => 0,
      'alpha_region_dashboard_sidebar_prefix' => '0',
      'alpha_region_dashboard_sidebar_columns' => '1',
      'alpha_region_dashboard_sidebar_suffix' => '0',
      'alpha_region_dashboard_sidebar_weight' => '-50',
      'alpha_region_dashboard_sidebar_position' => '0',
      'alpha_region_dashboard_sidebar_css' => '',
      'alpha_region_dashboard_sidebar_equal_height_element' => FALSE,
      'alpha_region_dashboard_sidebar_equal_height_container' => 0,
      'alpha_region_dashboard_main_force' => 0,
      'alpha_region_dashboard_main_prefix' => '0',
      'alpha_region_dashboard_main_columns' => '1',
      'alpha_region_dashboard_main_suffix' => '0',
      'alpha_region_dashboard_main_weight' => '-50',
      'alpha_region_dashboard_main_position' => '0',
      'alpha_region_dashboard_main_css' => '',
      'alpha_region_dashboard_main_equal_height_element' => FALSE,
      'alpha_region_dashboard_main_equal_height_container' => 0,
      'alpha_region_dashboard_inactive_force' => 0,
      'alpha_region_dashboard_inactive_prefix' => '0',
      'alpha_region_dashboard_inactive_columns' => '1',
      'alpha_region_dashboard_inactive_suffix' => '0',
      'alpha_region_dashboard_inactive_weight' => '-50',
      'alpha_region_dashboard_inactive_position' => '0',
      'alpha_region_dashboard_inactive_css' => '',
      'alpha_region_dashboard_inactive_equal_height_element' => FALSE,
      'alpha_region_dashboard_inactive_equal_height_container' => 0,
      'alpha_region_help_force' => 0,
      'alpha_region_help_prefix' => '0',
      'alpha_region_help_columns' => '1',
      'alpha_region_help_suffix' => '0',
      'alpha_region_help_weight' => '0',
      'alpha_region_help_position' => '0',
      'alpha_region_help_css' => '',
      'alpha_region_help_equal_height_element' => FALSE,
      'alpha_region_help_equal_height_container' => 0,
      'alpha_region_banner_force' => 0,
      'alpha_region_banner_prefix' => '0',
      'alpha_region_banner_columns' => '1',
      'alpha_region_banner_suffix' => '0',
      'alpha_region_banner_weight' => '0',
      'alpha_region_banner_position' => '0',
      'alpha_region_banner_css' => '',
      'alpha_region_banner_equal_height_element' => FALSE,
      'alpha_region_banner_equal_height_container' => 0,
      'alpha_region_credit_fifth_force' => 0,
      'alpha_region_credit_fifth_prefix' => '0',
      'alpha_region_credit_fifth_columns' => '1',
      'alpha_region_credit_fifth_suffix' => '0',
      'alpha_region_credit_fifth_weight' => '0',
      'alpha_region_credit_fifth_position' => '0',
      'alpha_region_credit_fifth_css' => '',
      'alpha_region_credit_fifth_equal_height_element' => FALSE,
      'alpha_region_credit_fifth_equal_height_container' => 0,
      'alpha_region_credit_third_force' => 0,
      'alpha_region_credit_third_prefix' => '0',
      'alpha_region_credit_third_columns' => '1',
      'alpha_region_credit_third_suffix' => '0',
      'alpha_region_credit_third_weight' => '0',
      'alpha_region_credit_third_position' => '0',
      'alpha_region_credit_third_css' => '',
      'alpha_region_credit_third_equal_height_element' => FALSE,
      'alpha_region_credit_third_equal_height_container' => 0,
      'alpha_region_credit_fourth_force' => 0,
      'alpha_region_credit_fourth_prefix' => '0',
      'alpha_region_credit_fourth_columns' => '1',
      'alpha_region_credit_fourth_suffix' => '0',
      'alpha_region_credit_fourth_weight' => '0',
      'alpha_region_credit_fourth_position' => '0',
      'alpha_region_credit_fourth_css' => '',
      'alpha_region_credit_fourth_equal_height_element' => FALSE,
      'alpha_region_credit_fourth_equal_height_container' => 0,
      'alpha_zone_footer_stretch_wrapper' => 0,
      'alpha_zone_footer_stretch_force' => 0,
      'alpha_zone_footer_stretch_order' => 0,
      'alpha_zone_footer_stretch_weight' => '0',
      'alpha_zone_footer_stretch_columns' => '12',
      'alpha_zone_footer_stretch_css' => '',
      'alpha_zone_footer_stretch_wrapper_css' => '',
      'alpha_zone_footer_stretch_equal_height_container' => 0,
      'alpha_zone_user_wrapper' => 1,
      'alpha_zone_user_force' => 1,
      'alpha_zone_user_order' => 0,
      'alpha_zone_user_section' => 'header',
      'alpha_zone_user_weight' => '1',
      'alpha_zone_user_columns' => '24',
      'alpha_zone_user_css' => '',
      'alpha_zone_user_wrapper_css' => '',
      'alpha_zone_user_equal_height_container' => 0,
      'alpha_region_user_first_force' => 0,
      'alpha_region_user_first_zone' => 'user',
      'alpha_region_user_first_prefix' => '0',
      'alpha_region_user_first_columns' => '19',
      'alpha_region_user_first_suffix' => '0',
      'alpha_region_user_first_weight' => '1',
      'alpha_region_user_first_position' => '0',
      'alpha_region_user_first_css' => '',
      'alpha_region_user_first_equal_height_element' => 0,
      'alpha_region_user_first_equal_height_container' => 0,
      'alpha_region_user_second_force' => 1,
      'alpha_region_user_second_zone' => 'user',
      'alpha_region_user_second_prefix' => '0',
      'alpha_region_user_second_columns' => '5',
      'alpha_region_user_second_suffix' => '0',
      'alpha_region_user_second_weight' => '2',
      'alpha_region_user_second_position' => '0',
      'alpha_region_user_second_css' => 'account',
      'alpha_region_user_second_equal_height_element' => 0,
      'alpha_region_user_second_equal_height_container' => 0,
      'alpha_zone_branding_wrapper' => 1,
      'alpha_zone_branding_force' => 0,
      'alpha_zone_branding_order' => 0,
      'alpha_zone_branding_section' => 'header',
      'alpha_zone_branding_weight' => '2',
      'alpha_zone_branding_columns' => '24',
      'alpha_zone_branding_css' => '',
      'alpha_zone_branding_wrapper_css' => '',
      'alpha_zone_branding_equal_height_container' => 0,
      'alpha_region_branding_force' => 1,
      'alpha_region_branding_zone' => 'branding',
      'alpha_region_branding_prefix' => '0',
      'alpha_region_branding_columns' => '19',
      'alpha_region_branding_suffix' => '0',
      'alpha_region_branding_weight' => '-30',
      'alpha_region_branding_position' => '0',
      'alpha_region_branding_css' => '',
      'alpha_region_branding_equal_height_element' => 0,
      'alpha_region_branding_equal_height_container' => 0,
      'alpha_region_search_force' => 1,
      'alpha_region_search_zone' => 'branding',
      'alpha_region_search_prefix' => '0',
      'alpha_region_search_columns' => '5',
      'alpha_region_search_suffix' => '0',
      'alpha_region_search_weight' => '50',
      'alpha_region_search_position' => '0',
      'alpha_region_search_css' => '',
      'alpha_region_search_equal_height_element' => 0,
      'alpha_region_search_equal_height_container' => 0,
      'alpha_zone_menu_wrapper' => 1,
      'alpha_zone_menu_force' => 0,
      'alpha_zone_menu_order' => 0,
      'alpha_zone_menu_section' => 'header',
      'alpha_zone_menu_weight' => '3',
      'alpha_zone_menu_columns' => '24',
      'alpha_zone_menu_css' => '',
      'alpha_zone_menu_wrapper_css' => '',
      'alpha_zone_menu_equal_height_container' => 0,
      'alpha_region_menu_force' => 1,
      'alpha_region_menu_zone' => 'menu',
      'alpha_region_menu_prefix' => '0',
      'alpha_region_menu_columns' => '19',
      'alpha_region_menu_suffix' => '5',
      'alpha_region_menu_weight' => '1',
      'alpha_region_menu_position' => '0',
      'alpha_region_menu_css' => '',
      'alpha_region_menu_equal_height_element' => 0,
      'alpha_region_menu_equal_height_container' => 0,
      'alpha_zone_header_wrapper' => 1,
      'alpha_zone_header_force' => 1,
      'alpha_zone_header_order' => 0,
      'alpha_zone_header_section' => 'header',
      'alpha_zone_header_weight' => '4',
      'alpha_zone_header_columns' => '24',
      'alpha_zone_header_css' => '',
      'alpha_zone_header_wrapper_css' => '',
      'alpha_zone_header_equal_height_container' => 0,
      'alpha_region_header_first_force' => 0,
      'alpha_region_header_first_zone' => 'header',
      'alpha_region_header_first_prefix' => '0',
      'alpha_region_header_first_columns' => '12',
      'alpha_region_header_first_suffix' => '0',
      'alpha_region_header_first_weight' => '1',
      'alpha_region_header_first_position' => '0',
      'alpha_region_header_first_css' => '',
      'alpha_region_header_first_equal_height_element' => 0,
      'alpha_region_header_first_equal_height_container' => 0,
      'alpha_region_header_second_force' => 0,
      'alpha_region_header_second_zone' => 'header',
      'alpha_region_header_second_prefix' => '0',
      'alpha_region_header_second_columns' => '12',
      'alpha_region_header_second_suffix' => '0',
      'alpha_region_header_second_weight' => '2',
      'alpha_region_header_second_position' => '0',
      'alpha_region_header_second_css' => '',
      'alpha_region_header_second_equal_height_element' => 0,
      'alpha_region_header_second_equal_height_container' => 0,
      'alpha_zone_preface_wrapper' => 1,
      'alpha_zone_preface_force' => 0,
      'alpha_zone_preface_order' => 0,
      'alpha_zone_preface_section' => 'content',
      'alpha_zone_preface_weight' => '1',
      'alpha_zone_preface_columns' => '24',
      'alpha_zone_preface_css' => '',
      'alpha_zone_preface_wrapper_css' => '',
      'alpha_zone_preface_equal_height_container' => 0,
      'alpha_region_preface_first_force' => 0,
      'alpha_region_preface_first_zone' => 'preface',
      'alpha_region_preface_first_prefix' => '0',
      'alpha_region_preface_first_columns' => '8',
      'alpha_region_preface_first_suffix' => '0',
      'alpha_region_preface_first_weight' => '1',
      'alpha_region_preface_first_position' => '0',
      'alpha_region_preface_first_css' => '',
      'alpha_region_preface_first_equal_height_element' => 0,
      'alpha_region_preface_first_equal_height_container' => 0,
      'alpha_region_preface_second_force' => 0,
      'alpha_region_preface_second_zone' => 'preface',
      'alpha_region_preface_second_prefix' => '0',
      'alpha_region_preface_second_columns' => '8',
      'alpha_region_preface_second_suffix' => '0',
      'alpha_region_preface_second_weight' => '2',
      'alpha_region_preface_second_position' => '0',
      'alpha_region_preface_second_css' => '',
      'alpha_region_preface_second_equal_height_element' => 0,
      'alpha_region_preface_second_equal_height_container' => 0,
      'alpha_region_preface_third_force' => 0,
      'alpha_region_preface_third_zone' => 'preface',
      'alpha_region_preface_third_prefix' => '0',
      'alpha_region_preface_third_columns' => '8',
      'alpha_region_preface_third_suffix' => '0',
      'alpha_region_preface_third_weight' => '3',
      'alpha_region_preface_third_position' => '0',
      'alpha_region_preface_third_css' => '',
      'alpha_region_preface_third_equal_height_element' => 0,
      'alpha_region_preface_third_equal_height_container' => 0,
      'alpha_zone_content_wrapper' => 1,
      'alpha_zone_content_force' => 1,
      'alpha_zone_content_order' => 0,
      'alpha_zone_content_section' => 'content',
      'alpha_zone_content_weight' => '2',
      'alpha_zone_content_columns' => '24',
      'alpha_zone_content_primary' => 'content',
      'alpha_zone_content_css' => '',
      'alpha_zone_content_wrapper_css' => '',
      'alpha_zone_content_equal_height_container' => 0,
      'alpha_region_sidebar_first_zone' => NULL,
      'alpha_region_sidebar_first_force' => 0,
      'alpha_region_sidebar_first_prefix' => '0',
      'alpha_region_sidebar_first_columns' => '6',
      'alpha_region_sidebar_first_suffix' => '0',
      'alpha_region_sidebar_first_weight' => '1',
      'alpha_region_sidebar_first_position' => '0',
      'alpha_region_sidebar_first_css' => '',
      'alpha_region_sidebar_first_equal_height_element' => 0,
      'alpha_region_sidebar_first_equal_height_container' => 0,
      'alpha_region_content_force' => 0,
      'alpha_region_content_zone' => 'content',
      'alpha_region_content_prefix' => '0',
      'alpha_region_content_columns' => '16',
      'alpha_region_content_suffix' => '0',
      'alpha_region_content_weight' => '2',
      'alpha_region_content_position' => '0',
      'alpha_region_content_css' => '',
      'alpha_region_content_equal_height_element' => 0,
      'alpha_region_content_equal_height_container' => 0,
      'alpha_region_sidebar_second_force' => 0,
      'alpha_region_sidebar_second_zone' => 'content',
      'alpha_region_sidebar_second_prefix' => '0',
      'alpha_region_sidebar_second_columns' => '8',
      'alpha_region_sidebar_second_suffix' => '0',
      'alpha_region_sidebar_second_weight' => '3',
      'alpha_region_sidebar_second_position' => '0',
      'alpha_region_sidebar_second_css' => '',
      'alpha_region_sidebar_second_equal_height_element' => 0,
      'alpha_region_sidebar_second_equal_height_container' => 0,
      'alpha_zone_postscript_wrapper' => 1,
      'alpha_zone_postscript_force' => 1,
      'alpha_zone_postscript_order' => 0,
      'alpha_zone_postscript_section' => 'footer',
      'alpha_zone_postscript_weight' => '-10',
      'alpha_zone_postscript_columns' => '24',
      'alpha_zone_postscript_css' => '',
      'alpha_zone_postscript_wrapper_css' => '',
      'alpha_zone_postscript_equal_height_container' => 0,
      'alpha_region_postscript_first_force' => 1,
      'alpha_region_postscript_first_zone' => 'postscript',
      'alpha_region_postscript_first_prefix' => '0',
      'alpha_region_postscript_first_columns' => '8',
      'alpha_region_postscript_first_suffix' => '0',
      'alpha_region_postscript_first_weight' => '1',
      'alpha_region_postscript_first_position' => '0',
      'alpha_region_postscript_first_css' => '',
      'alpha_region_postscript_first_equal_height_element' => 0,
      'alpha_region_postscript_first_equal_height_container' => 0,
      'alpha_region_postscript_second_force' => 1,
      'alpha_region_postscript_second_zone' => 'postscript',
      'alpha_region_postscript_second_prefix' => '0',
      'alpha_region_postscript_second_columns' => '8',
      'alpha_region_postscript_second_suffix' => '0',
      'alpha_region_postscript_second_weight' => '2',
      'alpha_region_postscript_second_position' => '0',
      'alpha_region_postscript_second_css' => '',
      'alpha_region_postscript_second_equal_height_element' => 0,
      'alpha_region_postscript_second_equal_height_container' => 0,
      'alpha_region_postscript_third_force' => 1,
      'alpha_region_postscript_third_zone' => 'postscript',
      'alpha_region_postscript_third_prefix' => '0',
      'alpha_region_postscript_third_columns' => '8',
      'alpha_region_postscript_third_suffix' => '0',
      'alpha_region_postscript_third_weight' => '3',
      'alpha_region_postscript_third_position' => '0',
      'alpha_region_postscript_third_css' => '',
      'alpha_region_postscript_third_equal_height_element' => 0,
      'alpha_region_postscript_third_equal_height_container' => 0,
      'alpha_zone_footer_wrapper' => 1,
      'alpha_zone_footer_force' => 1,
      'alpha_zone_footer_order' => 0,
      'alpha_zone_footer_section' => 'footer',
      'alpha_zone_footer_weight' => '-5',
      'alpha_zone_footer_columns' => '24',
      'alpha_zone_footer_css' => '',
      'alpha_zone_footer_wrapper_css' => '',
      'alpha_zone_footer_equal_height_container' => 0,
      'alpha_region_footer_first_force' => 1,
      'alpha_region_footer_first_zone' => 'footer',
      'alpha_region_footer_first_prefix' => '0',
      'alpha_region_footer_first_columns' => '6',
      'alpha_region_footer_first_suffix' => '0',
      'alpha_region_footer_first_weight' => '-20',
      'alpha_region_footer_first_position' => '0',
      'alpha_region_footer_first_css' => '',
      'alpha_region_footer_first_equal_height_element' => 0,
      'alpha_region_footer_first_equal_height_container' => 0,
      'alpha_region_footer_second_force' => 1,
      'alpha_region_footer_second_zone' => 'footer',
      'alpha_region_footer_second_prefix' => '0',
      'alpha_region_footer_second_columns' => '6',
      'alpha_region_footer_second_suffix' => '0',
      'alpha_region_footer_second_weight' => '-15',
      'alpha_region_footer_second_position' => '0',
      'alpha_region_footer_second_css' => '',
      'alpha_region_footer_second_equal_height_element' => 0,
      'alpha_region_footer_second_equal_height_container' => 0,
      'alpha_region_footer_third_force' => 1,
      'alpha_region_footer_third_zone' => 'footer',
      'alpha_region_footer_third_prefix' => '0',
      'alpha_region_footer_third_columns' => '6',
      'alpha_region_footer_third_suffix' => '0',
      'alpha_region_footer_third_weight' => '-5',
      'alpha_region_footer_third_position' => '6',
      'alpha_region_footer_third_css' => '',
      'alpha_region_footer_third_equal_height_element' => 0,
      'alpha_region_footer_third_equal_height_container' => 0,
      'alpha_region_footer_fourth_force' => 1,
      'alpha_region_footer_fourth_zone' => 'footer',
      'alpha_region_footer_fourth_prefix' => '0',
      'alpha_region_footer_fourth_columns' => '6',
      'alpha_region_footer_fourth_suffix' => '0',
      'alpha_region_footer_fourth_weight' => '10',
      'alpha_region_footer_fourth_position' => '8',
      'alpha_region_footer_fourth_css' => '',
      'alpha_region_footer_fourth_equal_height_element' => 0,
      'alpha_region_footer_fourth_equal_height_container' => 0,
      'alpha_region_footer_stretch_force' => 0,
      'alpha_region_footer_stretch_zone' => 'footer',
      'alpha_region_footer_stretch_prefix' => '0',
      'alpha_region_footer_stretch_columns' => '24',
      'alpha_region_footer_stretch_suffix' => '0',
      'alpha_region_footer_stretch_weight' => '30',
      'alpha_region_footer_stretch_position' => '0',
      'alpha_region_footer_stretch_css' => '',
      'alpha_region_footer_stretch_equal_height_element' => 0,
      'alpha_region_footer_stretch_equal_height_container' => 0,
      'alpha_zone_credit_wrapper' => 1,
      'alpha_zone_credit_force' => 1,
      'alpha_zone_credit_order' => 1,
      'alpha_zone_credit_section' => 'footer',
      'alpha_zone_credit_weight' => '0',
      'alpha_zone_credit_columns' => '24',
      'alpha_zone_credit_css' => '',
      'alpha_zone_credit_wrapper_css' => '',
      'alpha_zone_credit_equal_height_container' => 0,
      'alpha_region_credit_first_force' => 1,
      'alpha_region_credit_first_zone' => 'credit',
      'alpha_region_credit_first_prefix' => '0',
      'alpha_region_credit_first_columns' => '20',
      'alpha_region_credit_first_suffix' => '0',
      'alpha_region_credit_first_weight' => '0',
      'alpha_region_credit_first_position' => '0',
      'alpha_region_credit_first_css' => '',
      'alpha_region_credit_first_equal_height_element' => 0,
      'alpha_region_credit_first_equal_height_container' => 0,
      'alpha_region_credit_second_force' => 1,
      'alpha_region_credit_second_zone' => 'credit',
      'alpha_region_credit_second_prefix' => '0',
      'alpha_region_credit_second_columns' => '4',
      'alpha_region_credit_second_suffix' => '0',
      'alpha_region_credit_second_weight' => '1',
      'alpha_region_credit_second_position' => '4',
      'alpha_region_credit_second_css' => '',
      'alpha_region_credit_second_equal_height_element' => 0,
      'alpha_region_credit_second_equal_height_container' => 0,
      'alpha_zone_credit_stretch_wrapper' => 1,
      'alpha_zone_credit_stretch_force' => 1,
      'alpha_zone_credit_stretch_order' => 0,
      'alpha_zone_credit_stretch_section' => 'footer',
      'alpha_zone_credit_stretch_weight' => '5',
      'alpha_zone_credit_stretch_columns' => '24',
      'alpha_zone_credit_stretch_css' => '',
      'alpha_zone_credit_stretch_wrapper_css' => '',
      'alpha_zone_credit_stretch_equal_height_container' => 0,
      'alpha_region_credit_stretch_force' => 1,
      'alpha_region_credit_stretch_zone' => 'credit_stretch',
      'alpha_region_credit_stretch_prefix' => '0',
      'alpha_region_credit_stretch_columns' => '24',
      'alpha_region_credit_stretch_suffix' => '0',
      'alpha_region_credit_stretch_weight' => '2',
      'alpha_region_credit_stretch_position' => '6',
      'alpha_region_credit_stretch_css' => '',
      'alpha_region_credit_stretch_equal_height_element' => 0,
      'alpha_region_credit_stretch_equal_height_container' => 0,
    ),
  );
  $export[$delta->name] = $delta;
  
  return $export;
}