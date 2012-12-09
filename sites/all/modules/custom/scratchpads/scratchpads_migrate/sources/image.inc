<?php

class ScratchpadsNodeImageMigration extends DrupalNode6Migration{

  public function getSourceFieldTypes(){
    return $this->sourceFieldTypes;
  }
}

class DrupalImageMigration extends DrupalFileMigration{

  /**
   * Allow for multiple biological classifications which need to be merged into
   * the taxon name field
   *
   * @var array
   */
  protected $biologicalClassifications = array();

  /**
   * Mapping of each source CCK field to its type.
   *
   * @var array
   */
  protected $sourceFieldTypes = array();

  /**
   * Dummy node migration to use for a lot of the formatting
   */
  protected $nodeMigration;

  /**
   *
   * @param array $arguments          
   */
  public function __construct(array $arguments){
    parent::__construct($arguments);
    $this->destination = new MigrateDestinationFile('image', 'MigrateRemoteFileUri');
    $fields = $this->sourceFieldList();
    $fields['nid'] = 'nid';
    $fields['vid'] = 'vid';
    $fields['title'] = 'title';
    $fields['body'] = 'body';
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $fields, $this->countQuery(), $this->sourceOptions);
    $destination_fields = $this->destination->fields();
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
            }elseif($vocabulary->machine_name == 'image_keywords'){
              $field_name = 'field_keywords';
              $vocabulary->name = 'keywords';
            }else{
              $field_name = DrupalFieldTaxonomyMigration::getFieldName($vocabulary->name);
            }
          }else{
            $field_name = 'taxonomy_forums';
          }
          $source_migration = scratchpads_migrate_get_machine_name('Term', $vocabulary->name);
          // Handle multiple biological classifications
          if($field_name == 'field_taxonomic_name'){
            $this->biologicalClassifications[] = $delta;
            // If we already have a mapped biological classification, add as
            // unmapped source, we'll merge in perpareRow
            if(count($this->biologicalClassifications) > 1){
              $this->addUnmigratedSources(array(
                $delta
              ));
              continue;
            }
          }
          // Remove any existing mapping
          $this->removeFieldMapping($field_name);
          $this->removeFieldMapping($field_name . ':source_type');
          $this->removeFieldMapping($field_name . ':create_term');
          $this->addFieldMapping($field_name, $delta)->sourceMigration($source_migration);
          $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
          $this->addFieldMapping($field_name . ':create_term')->defaultValue(false);
        }else{ // Image gallery
          $this->removeFieldMapping('field_smg_galleries');
          $this->addFieldMapping('field_smg_galleries', $delta)->sourceMigration('ImageGallery');
        }
      }else{
        if(array_key_exists($delta, $destination_fields)){
          $this->addFieldMapping($delta, $delta);
        }
      }
    }
    $this->addUnmigratedDestinations(array(
      'field_sticky',
      'field_creator',
      'field_creator:format',
      'field_creator:language',
      'field_promote_to_home_page',
      'field_cc_licence'
    ));
    // Can't be bothered to write a destination handler as it's always the same
    // value - see prepare()
    $this->addUnmigratedSources(array(
      'field_imagefield',
      'image_publication',
      'image_specimen',
      // 'field_img_location',
      'nid',
      'vid'
    ));
    // Images get filename from the image node title
    $this->removeFieldMapping('filename');
    $this->addFieldMapping('filename', 'title');
    // Add body field if it exists
    $this->addFieldMapping('field_description', 'body');
    $this->addFieldMapping('field_description:format')->defaultValue('filtered_html');
    $this->addUnmigratedDestinations(array(
      'field_description:language'
    ));
  }

  public function sourceFieldList(){
    $this->nodeMigration = new ScratchpadsNodeImageMigration(array(
      'source_connection' => $this->sourceConnection,
      'source_version' => $this->sourceVersion,
      'source_type' => 'image',
      'destination_type' => 'image',
      'machine_name' => 'DummyImageMigration',
      'description' => 'Dummy migration used to get access to functions'
    ));
    $fields = $this->nodeMigration->sourceFieldList();
    $this->sourceFieldTypes = $this->nodeMigration->getSourceFieldTypes();
    return $fields;
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('files', 'f');
    $query->fields('f', array(
      'fid',
      'uid',
      'filepath',
      'timestamp',
      'filesize',
      'filename',
      'filemime',
      'status'
    ));
    $query->orderBy('f.fid');
    $query->condition('filemime', $this->imageFileMime(), 'IN');
    $query->condition('filename', array(
      'thumbnail',
      'preview'
    ), 'NOT IN');
    return $query;
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function countQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('files', 'f');
    $query->addExpression('COUNT(DISTINCT(f.fid))');
    $query->condition('filemime', $this->imageFileMime(), 'IN');
    $query->condition('filename', array(
      'thumbnail',
      'preview'
    ), 'NOT IN');
    return $query;
  }

  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Is there an image node for this image? If there is, add it!
    $query = Database::getConnection('default', $this->sourceConnection)->select('node', 'n');
    $query->fields('n', array(
      'nid',
      'vid',
      'title'
    ));
    $query->innerJoin('node_revisions', 'nr', 'nr.nid = n.nid');
    $query->fields('nr', array(
      'body'
    ));
    $query->innerJoin('content_field_imagefield', 'i', 'i.vid = n.vid');
    $query->condition('i.field_imagefield_fid', $row->fid);
    $result = $query->execute();
    if($result->rowCount()){
      $node = $result->fetchObject();
      $row->nid = $node->nid;
      $row->vid = $node->vid;
      $row->title = $node->title;
      $row->body = $node->body;
    }else{
      $row->nid = 0;
      $row->vid = 0;
    }
    $this->nodeMigration->prepareRow($row);
  }

  public function prepare($file, $row){
    $file->field_cc_licence['und'][0]['licence'] = CC_BY_NC;
    if(empty($file->field_smg_galleries['und'][0]['nid'])){
      unset($file->field_smg_galleries);
    }
  }

  static public function imageFileMime(){
    return array(
      'image/tiff',
      'image/jpeg',
      'image/gif',
      'image/png'
    );
  }
}