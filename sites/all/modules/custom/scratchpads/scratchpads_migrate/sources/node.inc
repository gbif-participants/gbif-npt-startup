<?php

/**
 * Base class for all node migrations - handles commonalities across all
 * supported source Drupal versions.
 *
 * In addition to the arguments supported by DrupalMigration, the following
 * must be passed in the $arguments array:
 *
 * source_type - Drupal 6 content type machine name.
 * destination_type - Drupal 7 content type machine name (bundle).
 *
 * The following optional arguments may be passed:
 *
 * user_migration - Machine name of a user migration, used to establish
 * dependencies and a sourceMigration for the uid mapping.
 * default_uid - Drupal 7 (destination) uid of the user account to use as
 * the default.
 */
class ScratchpadsNodeMigration extends DrupalNode6Migration{

  protected $longTextFields = array();

  public function getLongTextFields(){
    return $this->longTextFields;
  }

  protected $manualFieldMapping = array();

  public function getManualFieldMapping($field_name = null){
    if($field_name){
      if(isset($this->manualFieldMapping[$field_name])){
        return $this->manualFieldMapping[$field_name];
      }else{
        return false;
      }
    }else{
      return $this->manualFieldMapping;
    }
  }

  public function sourceFieldList(){
    $fields = parent::sourceFieldList();
    $fields['file_attachment'] = t('Attached files');
    return $fields;
  }

  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->fieldSuffixes['link'] = 'url';
    $this->destination = new ScratchpadsMigrateDestinationNode($this->destinationType);
    $destination_fields = $this->destination->fields();
    $fields = $this->sourceFieldList();
    $fields += $this->getManualFieldMapping();
    foreach($fields as $delta => $field){
      // Numeric keys are vocabulary fields
      if(is_numeric($delta)){
        $vocabulary = taxonomy_vocabulary_load($delta);
        $vids = array_keys(variable_get('biological_vids', array()));
        if($vocabulary){
          if($vocabulary->module != 'forum'){
            // Is this a biological classification
            if(in_array($delta, $vids)){
              $field_name = 'field_taxonomic_name';
            }else{
              $field_name = DrupalFieldTaxonomyMigration::getFieldName($vocabulary->name);
            }
          }else{
            $field_name = 'taxonomy_forums';
          }
          $sourceMigration = scratchpads_migrate_term_machine_name($vocabulary->name);
          // Remove any existing mapping
          $this->removeFieldMapping($field_name);
          $this->addFieldMapping($field_name, $delta)->sourceMigration($sourceMigration);
          $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
          $this->addFieldMapping($field_name . ':create_term')->defaultValue(false);
        }else{
          // Do nothing with Image Galleries vocab
          $this->addFieldMapping(NULL, $delta);
        }
      }elseif($delta == 'file_attachment'){
        // Does this node type have a file_field?
        if(array_key_exists('field_file', $destination_fields)){
          // Does have field file mapping so add a simple mapping. 
          // We'll get the files in prepareRow
          $this->addFieldMapping('field_file', $delta)->sourceMigration('File');
          $this->addFieldMapping('field_file:file_class')->defaultValue('MigrateFileFid');
          $this->addFieldMapping('field_file:file_replace')->defaultValue(FILE_EXISTS_REPLACE);
          $this->addUnmigratedDestinations(array(
            'field_file:language',
            'field_file:description',
            'field_file:display',
            'field_file:destination_dir',
            'field_file:destination_file',
            'field_file:preserve_files',
            'field_file:source_dir'
          ));
        }else{
          $this->addUnmigratedSources(array(
            'file_attachment'
          ));
        }
      }else{ // Other field types
        // Load the field
        $field = field_info_field($delta);
        // If we don't have a field with the same name, check if it's a media field
        if($field){
          $type = $field['type'];
        }else{
          $type = $this->getFieldType($delta);
        }
        if(isset($type)){
          switch($type){
            case 'image':
            case 'file':
            case 'filefield':
              $this->addFieldMapping('field_media', $delta)->sourceMigration('File');
              $this->addFieldMapping('field_media:file_class')->defaultValue('MigrateFileFid');
              $this->addFieldMapping('field_media:file_replace')->defaultValue(FILE_EXISTS_REPLACE);
              $this->addUnmigratedDestinations(array(
                'field_media:language',
                'field_media:description',
                'field_media:display',
                'field_media:destination_dir',
                'field_media:destination_file',
                'field_media:preserve_files',
                'field_media:source_dir'
              ));
              break;
            default:
              if($type == 'text_long'){
                // Keep a record of long text fields, as we need to extract files
                $this->longTextFields[] = $delta;
              }
              if(!$destination_field = $this->getManualFieldMapping($delta)){
                $destination_field = $delta;
              }
              $delta = strtolower($delta);
              $this->addFieldMapping($destination_field, $delta);
              foreach(array(
                'format',
                'language'
              ) as $subfield_type){
                $subfield = $destination_field . ':' . $subfield_type;
                if(array_key_exists($subfield, $this->destination->fields())){
                  if($subfield_type == 'format'){
                    $this->addFieldMapping($subfield)->defaultValue('filtered_html');
                  }else{
                    $this->addFieldMapping($subfield, NULL);
                  }
                }
              }
              break;
          }
        }
      }
    }
    // Use english as the default language
    $this->removeFieldMapping('language');
    $this->addFieldMapping('language', 'language')->defaultValue('en');
    $this->addFieldMapping('nid', 'nid');
    $this->removeFieldMapping('is_new');
    $this->addFieldMapping('is_new', NULL)->defaultValue(true);
    $this->addFieldMapping('group_audience', 'group_nid'); // Remap this in prepare row - needs to use OG id
    $this->addFieldMapping('group_content_access', 'og_public')->defaultValue(0);
    // Does this node have a body field?
    if(field_read_instance('node', 'body', $this->destinationType)){
      $this->longTextFields[] = 'body';
      $this->addUnmigratedDestinations(array(
        'body:summary',
        'body:format',
        'body:language'
      ));
    }else{
      $this->removeFieldMapping('body');
      $this->addUnmigratedSources(array(
        'body'
      ));
    }
    // Overide auto node title - we want to use the old titles
    $this->addFieldMapping('auto_nodetitle_applied')->defaultValue(true);
  }

  protected function nodeQuery(){
    // Only import nodes which are in the forum vocab
    $query = parent::nodeQuery();
    $query->leftJoin('og_ancestry', 'oga', 'oga.nid = n.nid');
    $query->addField('oga', 'group_nid', 'group_nid');
    $query->leftJoin('og_access_post', 'ogap', 'ogap.nid = n.nid');
    $query->addField('ogap', 'og_public', 'og_public');
    $query->condition('n.title', 'Taskguide', '!=');
    return $query;
  }

  protected function getFieldType($field_name){
    return Database::getConnection('default', $this->sourceConnection)->select('content_node_field', 'cnf')->fields('cnf', array(
      'type'
    ))->condition('field_name', $field_name)->execute()->fetchField();
  }

  public function prepareRow($row){
    parent::prepareRow($row);
    // Doesn't seem to use the default value, unless it's unset
    if(empty($row->language)){
      unset($row->language);
    }
    // Get the group ID
    if(!empty($row->group_nid)){
      $group_id = db_query('SELECT gid FROM {og} WHERE og.etid = :group_nid', array(
        ':group_nid' => $row->group_nid
      ))->fetchField();
      if($group_id){
        $row->group_nid = $group_id;
      }else{
        watchdog('scratchpads_migrate', 'Could not find group id for group node %group_nid', array(
          '%group_nid' => $row->group_nid
        ), WATCHDOG_ERROR);
      }
    }
    if(isset($row->og_public)){
      switch($row->og_public){
        case 0:
          $row->og_public = 2; // Private
          break;
        case 1:
          break;
        default:
          $row->og_public = 0; // Group defaults
          break;
      }
    }
    // Add the url alias
    $query = Database::getConnection('default', $this->sourceConnection)->select('url_alias', 'a');
    $query->addField('a', 'dst');
    $query->condition('a.src', 'node/' . $row->nid);
    $row->path = $query->execute()->fetchField();
    // Does this node have files attached?
    $query = Database::getConnection('default', $this->sourceConnection)->select('upload', 'u');
    $query->addField('u', 'fid');
    $query->condition('u.vid', $row->vid);
    $row->file_attachment = $query->execute()->fetchField();
    // Unset null rows, necessary for dwc location
    $properties = array_keys((array)$row);
    foreach($properties as $property){
      if(empty($row->{$property})){
        unset($row->{$property});
      }
    }
    // Extract any files
    foreach($this->getLongTextFields() as $longTextField){
      if(isset($row->{$longTextField})){
        $row->{$longTextField} = $this->parseImages($row->{$longTextField});
      }
    }
  }

  protected function parseImages($html){
    if(strpos($html, '<img') !== false){
      $dom = new DOMDocument();
      $dom->loadHTML($html);
      $dom->normalizeDocument();
      $items = $dom->getElementsByTagName('img');
      for($i = 0; $i < $items->length; $i++){
        // Does this img have a parent anchor tag
        $parent_node = $items->item($i)->parentNode;
        if($parent_node->tagName == 'a'){
          $href = $parent_node->getAttribute('href');
          if(strpos($href, 'files') !== false){
            if($new_href = $this->saveFile($href)){
              $new_href .= '?width=600&height=600';
              $parent_node->removeAttribute('href');
              $parent_node->setAttribute('href', $new_href);
              $parent_node->setAttribute('class', 'colorbox-inline');
            }
          }
        }
        $src = $items->item($i)->getAttribute('src');
        if($new_href = $this->saveFile($src)){
          $items->item($i)->removeAttribute('src');
          $items->item($i)->setAttribute('src', $new_href);
        }
      }
      $html = $dom->saveHTML();
    }
    return $html;
  }

  /**
   * Copy across a file
   */
  protected function saveFile($src){
    global $s1_domain;
    $parsed_url = parse_url($src);
    // If this is an external file, leave it alone
    if(isset($parsed_url['host']) && $parsed_url['host'] != $s1_domain){return;}
    $file_name = basename($parsed_url['path']);
    $source = 'http://' . $s1_domain . '/' . ltrim($parsed_url['path'], '/');
    $destination = file_default_scheme() . '://' . $file_name;
    if(file_exists($destination)){
      $html_img_src = $destination;
    }else if($file_contents = @file_get_contents($source)){
      if($file = file_save_data($file_contents, $destination, FILE_EXISTS_REPLACE)){
        $html_img_src = $destination;
      }
    }
    if(isset($html_img_src)){
      return $this->fileCreateUrl($html_img_src);
    }else{
      watchdog('scratchpads_migrate', t('Could not save file for %source'), array(
        '%source' => $source
      ), WATCHDOG_ERROR);
    }
  }

  /**
   * Create a full path URL for a file (rewrite public:// etc.,)
   * Suitable for an html img src
   */
  protected function fileCreateUrl($file){
    $wrapper = file_stream_wrapper_get_instance_by_uri($file);
    $url = base_path() . $wrapper->getDirectoryPath() . '/' . basename($file);
    return $url;
  }

  /**
   * Over-write DrupalNode6Migration::handle_type_filefield()
   * We want to use the fid, not filename
   */
  public function handle_type_filefield($fid){
    return $fid;
  }

  /**
   * Build a basic stub node
   */
  protected function buildStubNode($migration, $nid){
    $node = new stdClass();
    $node->title = t('Stub for %source_id', array(
      '%source_id' => $nid
    ));
    $node->type = $this->destination->getBundle();
    $node->uid = 1;
    $node->status = 0;
    $node->nid = $nid;
    $node->vid = $nid;
    $node->is_new = true;
    // Crap OG requires the original uid
    $node->original->uid = $node->uid;
    return $node;
  }

  /**
   * Create stubs (for node reference fields)
   */
  protected function createStub($migration){
    // Get source_id from args
    $args = func_get_args();
    $nid = $args[1][0];
    // Check the node doesn't already exist - otherwise as we're passing in nid and create new, we'll get integrity constraint violations
    if($this->nodeExists($nid)){
      return array(
        $nid
      );
    }else{
      $node = $this->buildStubNode($migration, $nid);
      node_save($node);
      if(isset($node->nid)){
        return array(
          $node->nid
        );
      }else{
        return FALSE;
      }
    }
  }

  protected function nodeExists($nid){
    $result = db_query('SELECT COUNT(*) FROM {node} n WHERE n.nid = :nid', array(
      ':nid' => $nid
    ));
    return $result->fetchField() > 0;
  }
}