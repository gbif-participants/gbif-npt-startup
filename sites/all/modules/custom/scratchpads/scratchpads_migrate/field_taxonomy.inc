<?php

/*
 * Class for content type migrations.
 */
abstract class DrupalFieldTaxonomyMigration extends DrupalMigration{

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // Create our three main objects - source, destination, and map    
    $this->source = new MigrateSourceSQL($this->fieldQuery(), $this->sourceFields, NULL, $this->sourceOptions);
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Is this a taxa field?
    if(!empty($row->classification_type)){
      $row->field_name = 'field_taxonomic_name';
      $row->label = 'Classification';
    }else{
      $row->field_name = $this->getFieldName($row->name);
      $row->label = $row->name;
    }
    if(field_read_instance('node', $row->field_name, $row->type)){ // Instance already exists, so skip it
      watchdog('scratchpads_migrate', t('Field %name already exists for %type'), array(
        '%name' => $row->field_name,
        '%type' => $row->type
      ), WATCHDOG_NOTICE);
      return false;
    }
  }

  /**
   * @abstract
   * Version-specific classes must implement contentTypeQuery, returning a query
   * that provides all the basic fields for each node type.
   */
  abstract protected function fieldQuery();

  /**
   * Generate a field name from the vocabulary name
   */
  static public function getFieldName($name){
    return 'field_' . ereg_replace('[^a-z0-9]', '_', strtolower($name));
  }
}

class MigrateDestinationFieldTaxonomy extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'type' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Create taxonomy fields');
  }

  public function import(stdClass $object, stdClass $row){
    $field = field_read_field($row->field_name);
    // Load the vocabulary
    $vocabulary = taxonomy_vocabulary_load($row->vid);
    if(!is_object($vocabulary)){throw new MigrateException(t('Vocabulary %vid does not exist', array(
        '%vid' => $row->vid
      )));}
    if(empty($field)){
      // Create the field if it doesn't exist
      $field_config = array(
        'active' => 1,
        'cardinality' => ($object->multiple == 0 ? 1 : -1),
        'deleted' => '0',
        'entity_types' => array(
          'node'
        ),
        'field_name' => $row->field_name,
        'module' => 'taxonomy',
        'translatable' => '1',
        'type' => 'taxonomy_term_reference',
        'settings' => array(
          'allowed_values' => array(
            array(
              'vocabulary' => $vocabulary->machine_name,
              'parent' => 0
            )
          )
        )
      );
      try{
        field_create_field($field_config);
      }
      catch(FieldException $e){
        throw new MigrateException($e->getMessage());
        return false;
      }
    }else{
      // Update field with VID
      foreach($field['settings']['allowed_values'] as $delta => $allowed_value){
        if($allowed_value['vocabulary'] == $vocabulary->machine_name){
          $exists = true;
        }
      }
      if(!isset($exists)){
        $field['settings']['allowed_values'][] = array(
          'vocabulary' => $vocabulary->machine_name,
          'parent' => 0
        );
      }
      field_update_field($field);
    }
    $field_instance = array(
      'required' => $object->required,
      'entity_type' => 'node',
      'bundle' => $object->bundle,
      'field_name' => $row->field_name,
      'module' => 'taxonomy',
      'label' => $row->label,
      'deleted' => '0'
    );
    if($row->field_name == 'field_taxonomic_name'){
      $field_instance['widget'] = array(
        'type' => 'tree_widget',
        'weight' => -2,
        'active' => 1
      );
    }else{
      // How many terms in this vocabulary?
      $term_count = Database::getConnection('default', 'scratchpad_1')->query('SELECT count(vid) FROM {term_data} WHERE vid = :vid', array(
        ':vid' => $vocabulary->vid
      ))->fetchField();
      // If greater than 100, use an autocomplete widget
      if($term_count > 100){
        $field_instance['widget'] = array(
          'type' => 'taxonomy_autocomplete',
          'module' => 'taxonomy',
          'active' => 1,
          'settings' => array(
            'autocomplete_path' => 'taxonomy/autocomplete',
            'size' => 60
          ),
          'weight' => 1
        );
      }else{
        $field_instance['widget'] = array(
          'type' => 'options_select',
          'module' => 'options',
          'active' => 1,
          'weight' => 1
        );
      }
    }
    try{
      $field_instance = field_create_instance($field_instance);
    }
    catch(FieldException $e){
      throw new MigrateException($e->getMessage());
      return false;
    }
    // Has the field been created?
    if(isset($field_instance['field_id'])){
      $this->numCreated++;
      watchdog('scratchpads_migrate', t('Created taxonomy field %name for %type'), array(
        '%name' => $row->field_name,
        '%type' => $object->bundle
      ), WATCHDOG_INFO);
      return array(
        $object->bundle . ':' . $row->field_name
      );
    }
    return false;
  }

  public function fields(){
    return array(
      'vid' => t('Vocabulary ID'),
      'bundle' => t('Bundle'),
      'name' => t('Vocabulary name'),
      'multiple' => t('Multiple'),
      'required' => t('Required'),
      'tags' => t('Tags')
    );
  }

  public function rollback(array $ids){
    list($bundle, $field_name) = explode(':', $ids['destid1']);
    // Delete the instances
    $instance = field_read_instance('node', $field_name, $bundle);
    field_delete_instance($instance);
  }
}