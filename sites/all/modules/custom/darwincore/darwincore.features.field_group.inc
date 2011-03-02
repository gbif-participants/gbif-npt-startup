<?php

/**
 * Implementation of hook_field_group_default_field_groups().
 */
function darwincore_field_group_default_field_groups() {
  $field_groups = array();

  // Exported field_group: 'node-specimen_observation-form-group_collection'
  $field_groups['node-specimen_observation-form-group_collection'] = array(
    'group_name' => 'group_collection',
    'entity_type' => 'node',
    'bundle' => 'specimen_observation',
    'mode' => 'form',
    'parent_name' => 'group_tabs',
    'label' => 'Collection',
    'weight' => '3',
    'children' => array(
      '0' => 'field_collector',
      '1' => 'field_collector_number',
      '2' => 'field_date_collected',
      '3' => 'field_notes',
      '4' => 'field_number',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-specimen_observation-form-group_location'
  $field_groups['node-specimen_observation-form-group_location'] = array(
    'group_name' => 'group_location',
    'entity_type' => 'node',
    'bundle' => 'specimen_observation',
    'mode' => 'form',
    'parent_name' => 'group_tabs',
    'label' => 'Location',
    'weight' => '5',
    'children' => array(
      '0' => 'field_location',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-specimen_observation-form-group_miscellaneous'
  $field_groups['node-specimen_observation-form-group_miscellaneous'] = array(
    'group_name' => 'group_miscellaneous',
    'entity_type' => 'node',
    'bundle' => 'specimen_observation',
    'mode' => 'form',
    'parent_name' => 'group_tabs',
    'label' => 'Miscellaneous',
    'weight' => '4',
    'children' => array(
      '0' => 'field_count',
      '1' => 'field_genbank_number',
      '2' => 'field_lifestage',
      '3' => 'field_other_catalogue_numbers',
      '4' => 'field_remarks',
      '5' => 'field_sex',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-specimen_observation-form-group_required'
  $field_groups['node-specimen_observation-form-group_required'] = array(
    'group_name' => 'group_required',
    'entity_type' => 'node',
    'bundle' => 'specimen_observation',
    'mode' => 'form',
    'parent_name' => 'group_tabs',
    'label' => 'Required',
    'weight' => '1',
    'children' => array(
      '0' => 'field_basis_of_record',
      '1' => 'field_catalogue_number',
      '2' => 'field_collection_code',
      '3' => 'field_institution_code',
      '4' => 'title',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-specimen_observation-form-group_tabs'
  $field_groups['node-specimen_observation-form-group_tabs'] = array(
    'group_name' => 'group_tabs',
    'entity_type' => 'node',
    'bundle' => 'specimen_observation',
    'mode' => 'form',
    'parent_name' => '',
    'label' => 'Tabs',
    'weight' => '0',
    'children' => array(
      '0' => 'group_required',
      '1' => 'group_taxonomy',
      '2' => 'group_collection',
      '3' => 'group_miscellaneous',
      '4' => 'group_location',
    ),
    'format_type' => 'tabs',
    'format_settings' => array(
      'formatter' => NULL,
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  // Exported field_group: 'node-specimen_observation-form-group_taxonomy'
  $field_groups['node-specimen_observation-form-group_taxonomy'] = array(
    'group_name' => 'group_taxonomy',
    'entity_type' => 'node',
    'bundle' => 'specimen_observation',
    'mode' => 'form',
    'parent_name' => 'group_tabs',
    'label' => 'Taxonomy',
    'weight' => '2',
    'children' => array(
      '0' => 'field_date_identified',
      '1' => 'field_identification_qualifier',
      '2' => 'field_identified_by',
      '3' => 'field_taxonomic_name',
      '4' => 'field_type_status',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );

  return $field_groups;
}