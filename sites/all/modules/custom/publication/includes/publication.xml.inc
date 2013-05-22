<?php

/**
 * 
 * This module uses what it calls "schema array", to represent
 * an XML file schema and describe how to map Drupal fields
 * to the XML format. When the schema is applied there is an associated
 * object, from which fields may be fetched (using #field). The object
 * itself can be changed during the processing (using #load).
 * 
 * The schema array has the following format:
 *
 *  array(
 *    #min_occurence:   Minimum number of occurence. Defaults to 1.
 *                      An exception will be raised if this constraint is not respected
 *    #max_occurence':  Maximum number of occurence. Defaults to 1, use -1 for unlimited
 *                      An expception will be raised if this constraint is not respected
 *    #attributes:      Attributes to add to the element, as an array of name => value
 *    #process:         Optional function to process the value of the node before insertion.
 *                      available functions are:
 *                        - _pensoft_publication_xml_process_html: to insert HTML values
 *    #process arguments : If the process function is called, this is passed as additional argument
 *    
 *    #load:            Optional function to load the object from which fields are fetched, which
 *                      replaces the currently used object. The function is given the current
 *                      value (as defined by #value or #field) as argument.
 *                      
 *                      Note that this only affects the children of the current node, not the current
 *                      node itself or any other node processed subsequently.
 *                      
 *                      If this returns a value that evaluates to FALSE, then the child element is
 *                      not inserted. If an exception was expected instead, then the loader should raise it.
 *                      
 *    #field :         A field on the current mapping object to use as value when generating the XML.
 *    
 *                      The field on the object can be a single value, or a Drupal value array:
 *                      array("und" => array("value" => ...)). If the format is different, use '#process'
 *                      to read the value from the array.
 *                      
 *                      If the field is multi-valued and #merge is not defined, then each value is inserted 
 *                      as a separate entry (as long as the min/max constraints are respected).
 *    
 *    #value :          A static value to insert when generating the XML. This is inserted as many
 *                      times as required by #min_occurence.
 *                      
 *    #merge :          If a field (as defined by #field) is multi valued and '#merge' is defined and is
 *                      an existing function name, then that function is called and only one value (the 
 *                      return value of that function) is inserted.
 *
 *                      If both #value and #field are present, #field is used for constraint checking
 *                      (and for #load), but it's the value of #value that is inserted.
 *     
 *    #condition:       If present, the current node (and it's children) are only processed if the field
 *                      described by #condition is not empty.
 *                      
 *    #condition arguments : If present in combination with #condition, then this modifies the test
 *                           done by #condition. #condition arguments is an array like array($op, $val)
 *                           where the field defined in #condition is tested against $val using operator
 *                           $op.
 *
 *    #error_info:      If present, this will be used instead of the field name in error messages
 *                      (eg. field ... is required)
 *
 *    #error_field      If present, the string '%' in the #error_info will be substituted for the value
 *                      of that field on the current object
 *
 *   <tag name>:        A schema array for child elements. There can be any number of
 *                      such entries. To allow for several of the same tag (for instance, several
 *                      iteration of a tag mapping to different fields) add #<number> at the end
 *                      of the tag.
 *  );
 *
 */
/**
 * This class is used to parse an XSD schema into a schema array
 * 
 */
class PublicationXSDParser{

  /**
   * Construct the parser from the URL
   * 
   */
  function __construct($url){
    $this->schema_url = $url;
  }

  /**
   * Parse the given schema and return the associated schema array.
   * This will throw an Exception on error.
   * 
   */
  function get_schema_array(){
    $dom = simplexml_load_file($this->schema_url);
    if($dom === false){throw new Exception("Could not load/parse schema at " . $this->schema_url);}
    $this->dom = $dom;
    $this->schema = $this->_create_elements($dom);
    return $this->schema;
  }

  /**
   * Create the schema array representing the xsd:elements under
   * the current element.
   */
  function _create_elements($current){
    $schema = array();
    $elements = $current->xpath('xsd:element|xsd:sequence');
    foreach($elements as $element){
      if($element->getName() == 'element'){
        $vars = get_object_vars($element);
        $attributes = $vars['@attributes'];
        if(isset($attributes['type'])){
          $schema[$attributes['name']] = $this->_get_type($attributes['type']);
        }else{
          // Look for in-line type definition
          $inline = $element->xpath('xsd:complexType');
          if(count($inline) > 0){
            if(count($inline) > 1){throw new Exception("Invalid XSD schema: more than one inline complexType");}
            $inline_type = array_shift($inline);
            $schema[$attributes['name']] = $this->_parse_complex_type($inline_type, $attributes['name']);
          }else{
            // Simple value
            $schema[$attributes['name']] = array();
          }
        }
        if(isset($attributes['minOccurs']) && $attributes['minOccurs'] != 1){
          $schema[$attributes['name']]['#min_occurence'] = $attributes['minOccurs'] == "unbounded" ? -1 : intval($attributes['minOccurs']);
        }
        if(isset($attributes['maxOccurs']) && $attributes['maxOccurs'] != 1){
          $schema[$attributes['name']]['#max_occurence'] = $attributes['maxOccurs'] == "unbounded" ? -1 : intval($attributes['maxOccurs']);
        }
      }else if($element->getName() == 'sequence'){
        // A sequence of elements, indicating a repeat within the parent tag.
        // XXX we need to handle this properly - as this is actually wrong. The sequence has it's own minOccurs/maxOccurs.
        // We'd need a '#sequence' element.
        $schema = array_merge($schema, $this->_create_elements($element));
      }else{
        throw new Exception('Unsuported element type within flow : ' . $element->getName());
      }
    }
    return $schema;
  }

  /**
   * Create the schema array representing the given xsd:simple/complexType element
   * 
   */
  function _get_type($type){
    // Handle inbuild types
    switch($type){
      case 'xsd:string':
        return array();
    }
    // Look for a simple/complexType definition
    $types = $this->dom->xpath('xsd:simpleType[@name="' . $type . '"]');
    if(count($types) > 0){
      // Handle simple types
      if(count($types) > 1){throw new Exception("Invalid XSD schema: more than one definition of type " . $type);}
      $type_element = array_shift($types);
      $element = array();
    }else{
      // Handle complex types
      $types = $this->dom->xpath('xsd:complexType[@name="' . $type . '"]');
      if(count($types) == 0){
        throw new Exception("Parse error: could not find type " . $type);
      }else if(count($types) > 1){throw new Exception("Invalid XSD schema: more than one definition of type " . $type);}
      $type_element = array_shift($types);
      $element = $this->_parse_complex_type($type_element, $type);
    }
    // XXX TODO parse restrictions.
    return $element;
  }

  /**
   * Parse a complexType element, and return the schema array
   */
  function _parse_complex_type($element, $display_name){
    $result = array();
    // Look for attributes
    $attributes = $element->xpath('xsd:attribute');
    foreach($attributes as $attribute){
      $vars = get_object_vars($attribute);
      $name = $vars['@attributes']['name'];
      // XXX Does this mean the attribute is required ? How do we mark this ?
      //$result['#attributes'] = array($name => NULL);
    }
    // Look for a sequence
    $sequences = $element->xpath('xsd:sequence');
    if(count($sequences) > 1){
      throw new Exception("XSD Schema parsing: we only support complex types with one sequence for type/inline $display_name");
    }else if(count($sequences) == 1){
      $sequence = array_shift($sequences);
      $result = array_merge($result, $this->_create_elements($sequence));
    }
    return $result;
  }

  /**
   * Merge two schema arrays into one
   */
  function merge_schemas($a, $b, $diff = FALSE){
    // Base case if either $a or $b is not an array
    if(!is_array($a)){
      if(is_array($b) && $diff){
        $b['#comment'] .= " DIFF:Type mismatch, this value was not an array in the source schema;";
      }
      return $b;
    }
    if(!is_array($b)){
      $a[] = $b;
      if($diff){
        $a['#comment'] .= " DIFF:Type mismatch, this value was not an array in the provided schema;";
      }
      return $a;
    }
    // Prepare for processing
    $r = array();
    $b_keys = array_keys($b);
    // Start with $a, and look for equivalents in $b
    foreach(array_keys($a) as $a_key){
      // Match same keys
      if(!isset($b[$a_key])){
        $r[$a_key] = $a[$a_key];
        if($diff && strpos($a_key, '#') === FALSE){
          $r['#comment'] .= " DIFF: $a_key missing in provided schema;";
        }
      }else{
        $r[$a_key] = $this->merge_schemas($a[$a_key], $b[$a_key], $diff);
        unset($b[$a_key]);
      }
      // Match number #x keys
      foreach($b_keys as $b_key){
        if(preg_match('/^' . preg_quote($a_key, '/') . '#\d+$/', $b_key)){
          $r[$b_key] = $this->merge_schemas($a[$a_key], $b[$b_key], $diff);
          unset($b[$b_key]);
        }
      }
    }
    // Now add what is left in $b
    if($diff){
      foreach($b as $b_key => $b_val){
        if(strpos($b_key, '#') === FALSE){
          $b['#comment'] .= " DIFF:$b_key missing in source schema;";
        }
      }
    }
    $r = array_merge($r, $b);
    return $r;
  }
}

/**
 * This class is used to generate a readable output of how Drupal
 * fields are mapped to an XML type by parsing an array schema
 */
class PublicationXMLSchemaExplain{

  /**
   * Create a new schema parser from
   * an array schema
   */
  function __construct($schema){
    $this->schema = $schema;
  }

  /**
   * Generates XML-like data which shows the strcuture of the
   * generated XML and the associated Drupal fields/functions
   */
  function explain_with_xml(){
    $output = array();
    $this->_path = array();
    foreach($this->schema as $tag => $child_schema){
      $output[] = "<$tag>";
      $output[] = $this->_explain_with_xml_generate($child_schema);
      $output[] = "</$tag>";
    }
    return $this->_implode_indent($output);
  }

  /**
   * Helper function to build the XML explanation
   */
  function _explain_with_xml_generate($schema){
    $output = array();
    // Show comments
    if(!empty($schema['#comment'])){
      $output[] = "COMMENT: " . $schema['#comment'];
    }
    // Add attributes
    if(!empty($schema['#attributes'])){
      foreach($schema['#attributes'] as $attr_name => $attr_value){
        $output[] = "SET ATTR $attr_name = $attr_value;";
      }
    }
    // Add values
    $value_to_set = '';
    if(isset($schema['#value'])){
      $value_to_set = $schema['#value'];
    }else if(!empty($schema['#field'])){
      $value_to_set = $this->_field_name($schema['#field']);
    }
    if($value_to_set){
      if(!isset($schema['#value']) && isset($schema['#merge'])){
        $value_to_set = $schema['#merge'] . '(' . $value_to_set . ')';
      }
      if(isset($schema['#process'])){
        // XXX need to display process arguments
        $value_to_set = $schema['#process'] . '(' . $value_to_set . ')';
      }
      $output[] = "SET VALUE = " . $value_to_set . ";";
    }
    // Add conditions
    $condition = FALSE;
    if(isset($schema['#condition'])){
      $elem = $this->_field_name($schema['#condition']);
      if(isset($schema['#condition arguments'])){
        $condition = "$elem " . $schema['#condition arguments'][0] . ' ' . $schema['#condition arguments'][1];
      }else{
        $condition = "$elem";
      }
      $output[] = "IF ($condition) {";
    }
    // Load new object
    $has_load = FALSE;
    if(isset($schema['#load'])){
      $elem = $this->_field_name($schema['#field']);
      $path = $schema['#load'] . '(' . $elem;
      if(isset($schema['#load arguments'])){
        $arguments = array();
        foreach($schema['#load arguments'] as $argument){
          if (is_string($argument) && preg_match('/^field:(.+)$/', $argument, $matches)) {
            $load_elem = $this->_field_name($matches[1]);
            $arguments[] = $load_elem;
          } else {
            $arguments[] = 'unserialize("' . serialize($argument) . '")';
          }
        }
        $path .= ', ' . implode(', ', $arguments);
      }
      $this->_path[] = $path . ')';
      $has_load = TRUE;
    }
    // Recurse
    $sub = array();
    foreach($schema as $tag => $child_schema){
      if(strpos($tag, '#') === 0){
        continue;
      }
      $sub[] = "<$tag>";
      $sub[] = $this->_explain_with_xml_generate($child_schema);
      $sub[] = "</$tag>";
    }
    if($condition){
      $output[] = $sub;
      $output[] = "} /* $condition */";
    }else{
      $output = array_merge($output, $sub);
    }
    if($has_load){
      array_pop($this->_path);
    }
    return $output;
  }

  /**
   * Helper function to produce field name from series or loads
   */
  function _field_name($field){
    if(count($this->_path)){
      return $this->_path[count($this->_path) - 1] . '->' . $field;
    }else{
      return '$publication->' . $field;
    }
  }

  /**
   * Helper function for indenting text
   */
  function _implode_indent($struct, $indent = 0){
    $out = "";
    foreach($struct as $val){
      if(is_array($val)){
        $sub = $this->_implode_indent($val, $indent + 1);
        if(trim($sub)){
          $out .= $sub;
        }
      }else{
        $out .= str_repeat('  ', $indent) . $val . "\n";
      }
    }
    return $out;
  }
}

/**
 * This class is used to build XML from an array schema
 * 
 */
class PublicationXMLBuilder{

  /**
   * Creatre a new builder from a schema and settings and
   * a number of modifiers (objects that implement PublicationXMLModifierInterface)
   * 
   * The settings is an associative array which may define:
   * 'force-empty-values' : If the schema defines that a tag associated with
   *   a #field should be present (#min_occurence > 0) but the associated
   *   field is empty, setting this to TRUE will insert a blank value in it's
   *   place (the default is FALSE, which would raise an error)
   *   
   * 'no-error' : Ignore contraint errors (no exception thrown). Default
   *              is FALSE
   */
  function __construct($publication_name, $array_schema, $settings, $modifiers = array()){
    $this->publication_name = $publication_name;
    $this->array_schema = $array_schema;
    $this->settings = $settings;
    $this->modifiers = $modifiers;
  }

  /**
   * This function builds the XML from the given object.
   * 
   * This function will throw an exception on errors
   * (unless 'no-error' was set), and return
   * the DOM document.
   * 
   */
  function build_xml($object, $xsd_url = NULL, $version = '1.0', $encoding = 'UTF-8'){
    $this->_root_object = $object;
    $this->_dom = new DOMDocument($version, $encoding);
    foreach($this->modifiers as $modifier){
      $modifier->start_building($this->array_schema, $this->_root_object, $this->_dom);
    }
    $this->path = array();
    $this->_build_xml_iteration($this->array_schema, $this->_root_object, $this->_dom);
    $xml = $this->_dom->saveXML();
    if($xsd_url && empty($this->settings['no-error'])){
      $this->_validate_xml($xml, $xsd_url, $version, $encoding);
    }
    return $xml;
  }

  /**
   * From a schema array, object and xml element, populates the XML
   * with the computed values.
   * 
   * This function returns TRUE if some nodes were inserted because
   * they had an actual value (defined by #field or #value). This is
   * used for backtracking and removing un-needed parent elements.
   *  
   */
  function _build_xml_iteration($schema, $object, $xml_element){
    $inserted = FALSE;
    foreach($schema as $tag => $child_schema){
      // Ignore properties, they should be dealt with at the level above
      if(preg_match('/^#/', $tag)){
        continue;
      }
      // Remove tag numbers
      if(preg_match('/^(.+)#\d+$/', $tag, $matches)){
        $tag = $matches[1];
      }
      // Check if there is a conditional
      if(isset($child_schema['#condition'])){
        $field = $child_schema['#condition'];
        if(!empty($child_schema['#condition arguments'])){
          $cond_op = $child_schema['#condition arguments'][0];
          $cond_val = $child_schema['#condition arguments'][1];
          if(empty($object->$field)){
            continue;
          }
          $field_val = $object->$field;
          if($cond_op == '=' && $field_val[LANGUAGE_NONE][0]['value'] != $cond_val){
            continue;
          }
        }else if(empty($object->$field)){
          continue;
        }
      }
      // Check the occurence constraint works
      $this->_build_xml_check_constraint($child_schema, $tag, $object);
      // Insert the value and recurse
      $this->path[] = $tag;
      if(isset($child_schema['#field'])){
        $insert_child = $this->_build_schema_insert_field($child_schema, $tag, $object, $xml_element);
        $inserted = $inserted || $insert_child;
      }else if(isset($child_schema['#value'])){
        $insert_child = $this->_build_schema_insert_value($child_schema, $tag, $object, $xml_element);
        $inserted = $inserted || $insert_child;
      }else{
        $insert_child = $this->_build_schema_insert_blank($child_schema, $tag, $object, $xml_element);
        $inserted = $inserted || $insert_child;
      }
      array_pop($this->path);
    }
    return $inserted;
  }

  /**
   * Insert a #field value, and recurse through the schema as appropriate
   * 
   */
  function _build_schema_insert_field($schema, $tag, $object, $xml_element){
    $field = $schema['#field'];
    if(isset($schema['#merge']) && function_exists($schema['#merge'])){
      $merge_function = $schema['#merge'];
      $values = array(
        $merge_function($object->$field)
      );
    }else{
      $values = $this->_read_values($object->$field);
    }
    foreach($this->modifiers as $modifier){
      $values = $modifier->insert_value_array($schema, $tag, $values);
    }
    $inserted = FALSE;
    foreach($values as $value){
      $value_to_insert = $value;
      if(isset($schema['#value'])){
        $value_to_insert = $schema['#value'];
      }
      try{
        $child_object = $this->_load_child_object($schema, $object, $value);
      }
      catch(Exception $e){
        $this->_generate_error($e->getMessage());
        $child_object = NULL;
      }
      if(!$child_object){
        // It's up to the loader to raise errors if need be. Here an empty return means skip it.
        // XXX doing this test here means we're not checking against min/max occurs.
        continue;
      }
      // XXX The new Modifier functionality means we might end up with a NULL value, and thus
      // should set $inserted to FALSE (depending on min_ocurrence) to provide adequate backtracking.
      $child_element = $this->_insert_xml_element($schema, $tag, $value_to_insert, $xml_element);
      $inserted = TRUE;
      $this->_build_xml_iteration($schema, $child_object, $child_element);
    }
    return $inserted;
  }

  /**
   * Insert a #value, and recurse through the schema as appropriate
   * 
   */
  function _build_schema_insert_value($schema, $tag, $object, $xml_element){
    $inserted = FALSE;
    $count = (!isset($schema['#min_occurence']) || $schema['#min_occurence'] == 0) ? 1 : $schema['#min_occurence'];
    $value = $schema['#value'];
    for($i = 0; $i < $count; $i++){
      // XXX The new Modifier functionality means we might end up with a NULL value, and thus
      // should set $inserted to FALSE (depending on min_ocurrence) to provide adequate backtracking.
      $child_element = $this->_insert_xml_element($schema, $tag, $value, $xml_element);
      $inserted = TRUE;
      $this->_build_xml_iteration($schema, $object, $child_element);
    }
    return $inserted;
  }

  /**
   * Insert a blank value if the element has #min_occurence > 0 or if one of the
   * children inserts a real (#value of #field) value.
   *
   */
  function _build_schema_insert_blank($schema, $tag, $object, $xml_element){
    $min_occurence = isset($schema['#min_occurence']) ? $schema['#min_occurence'] : 1;
    $count = ((!isset($schema['#min_occurence']) || $min_occurence == 0) ? 1 : $schema['#min_occurence']);
    $inserted = FALSE;
    for($i = 0; $i < $count; $i++){
      // XXX The new Modifier functionality means we might end up with a non NULL , and thus
      // should set $inserted to TRUE to provide adequate backtracking.
      $child_element = $this->_insert_xml_element($schema, $tag, NULL, $xml_element);
      $insert_child = $this->_build_xml_iteration($schema, $object, $child_element);
      if(!$insert_child && $min_occurence == 0){
        $xml_element->removeChild($child_element);
      }else if($insert_child){
        $inserted = TRUE;
      }
    }
    return $inserted;
  }

  /**
   * Load and return the object to be used by a node's children.
   * 
   */
  function _load_child_object($schema, $object, $value){
    if(isset($schema['#load'])){
      $arguments = array(
        $value
      );
      if(isset($schema['#load arguments'])){
        foreach ($schema['#load arguments'] as $argument) {
          if (is_string($argument) && preg_match('/^field:(.+)$/', $argument, $matches)) {
            $arg_field = $matches[1];
            if (isset($object->$arg_field)) {
              $arguments[] = $object->$arg_field;
            }
          } else {
            $arguments[] = $argument;
          }
        }
      }
      $object = call_user_func_array($schema['#load'], $arguments);
    }
    return $object;
  }

  /**
   * Insert an element given it's array schema into the DOM.
   * 
   */
  function _insert_xml_element($schema, $tag, $value, $xml_element){
    // Apply process function
    if(isset($schema['#process'])){
      $f = $schema['#process'];
      if(!function_exists($f)){throw new Exception("Process function $f for tag $tag does not exist");}
      $args = array(
        $value
      );
      if(!empty($schema['#process arguments'])){
        $args = array_merge($args, $schema['#process arguments']);
      }
      $value = call_user_func_array($f, $args);
    }
    // Check for standard drupal field value format. Anything that is a direct value, or
    // an array definined 'value' will need a 'process' function defined.
    if(is_array($value) && isset($value['value'])){
      $value = $value['value'];
    }
    // Create the xml element
    $child_element = $xml_element->appendChild($this->_dom->createElement($tag));
    // Add attributes
    if(!empty($schema['#attributes'])){
      foreach($schema['#attributes'] as $attr_name => $attr_value){
        $child_element->setAttribute($attr_name, $attr_value);
      }
    }
    // Add value
    foreach($this->modifiers as $modifier){
      $value = $modifier->insert_value($tag, $schema, $value);
    }
    if($value !== NULL){
      // simpleXML escapes HTML tags, but not HTML entities ; however not all entities (such as &nbsp;) are supported
      // in XML. All numerical entities are fine, so we only need to escape named entities.
      $escaped_value = preg_replace_callback('/&([a-zA-Z]+;)?/', array(
        $this,
        '_escape_html_entities'
      ), (string)$value);
      $child_element->nodeValue = (string)$escaped_value;
    }
    return $child_element;
  }

  /**
   * A function for escaping html entities called from preg_replace_callback
   */
  function _escape_html_entities($matches){
    $entity = '';
    if(!empty($matches[1]) && $matches[1] != 'amp;'){
      $entity = $matches[1];
    }
    return '&amp;' . $entity;
  }

  /**
   * This method checks that the constraint of a given schema array are
   * fullfilled by the given object.
   * 
   * Depending on the settings, this might throw an exception or modify the
   * schema array's value to ensure that the constraint are respected.
   */
  function _build_xml_check_constraint(&$schema, $tag, $object){
    $min_occurence = isset($schema['#min_occurence']) ? $schema['#min_occurence'] : 1;
    $max_occurence = isset($schema['#max_occurence']) ? $schema['#max_occurence'] : 1;
    if(!empty($schema['#field'])){
      $field = $schema['#field'];
      if(isset($object->$field)){
        if(isset($schema['#merge'])){
          $count = 1;
        }else{
          $field_value = $this->_read_values($object->$field);
          $count = count($field_value);
        }
      }else{
        $count = 0;
      }
      if($count < $min_occurence || ($max_occurence >= 0 && $count > $max_occurence)){
        $message_prepare = "the field %field";
        if($count == 0){
          $message_prepare .= " is required.";
        }else if($count < $min_occurence){
          $message_prepare .= " must be present at least %min times, but is only here %count times.";
        }else{
          $message_prepare .= " can be present at most %max times, but is here %count times.";
        }
        $error_field = $field;
        if(isset($schema['#error_info'])){
          $error_field = $schema['#error_info'];
          if(isset($schema['#error_field'])){
            $error_context_field = $schema['#error_field'];
            if(isset($object->$error_context_field)){
              $error_context = reset($this->_read_values($object->$error_context_field));
              if(is_array($error_context) && isset($error_context['value'])){
                $error_context = $error_context['value'];
              }
              $error_field = str_replace('%', $error_context, $error_field);
            }
          }
        }
        $message = t($message_prepare, array(
          '%field' => $error_field,
          '%min' => $min_occurence,
          '%max' => $max_occurence,
          '%count' => $count,
          '%tag' => $tag
        ));
        $this->_generate_error($message);
        if($this->settings['force-empty-values'] && $count == 0){
          // If no exception was raised and we are asked to enter empty values, make sure we adapt the schema
          $schema['#value'] = '';
          unset($schema['#field']);
        }
      }
    }
  }

  /**
   * This function registers an error and either interupts processing by
   * throwing an exception, or logs the error and returns
   */
  function _generate_error($message){
    $message = t('The publication %publication could not be generated, because !message. (XML Path where error occured: %path)', array(
      '%publication' => $this->publication_name,
      '!message' => $message,
      '%path' => implode(' >> ', $this->path)
    ));
    if($this->settings['force-empty-values'] && $count == 0){
      drupal_set_message(t("The following error happened but was ignored due to settings, and an empty value was set in it's place: ") . $message, 'warning');
    }else if($this->settings['no-error']){
      drupal_set_message(t("The following error happened but was ignored due to settings: ") . $message, 'warning');
    }else{
      throw new Exception($message);
    }
  }

  /**
   * Given a value, taken from a field or a static value,
   * return it in a consistent manner as an array
   * containing all the values.
   * 
   * If this comes from a Drupal field and contains a language
   * array, this is removed to return the values directly
   * 
   */
  function _read_values($value, $language = LANGUAGE_NONE){
    if(is_array($value) && isset($value[$language])){
      return $this->_read_values($value[$language], $language);
    }else if(is_array($value)){
      return $value;
    }else{
      return array(
        $value
      );
    }
  }

  /**
   * Validates generated XML against an XSD schema, and throw
   * an Excpetion if the validation fails
   */
  function _validate_xml($xml, $xsd_url, $version = '1.0', $encoding = 'UTF-8'){
    // Try to break the XML down in lines such that we don't disturb content by adding unwanted
    // carriage returns (that would affect enumerations for instance).
    $xml_per_line = preg_replace('/(<\/.*?>)/', "$1\n", $xml);
    $parsed_dom = new DOMDocument($version, $encoding);
    $parsed_dom->loadXML($xml_per_line);
    libxml_use_internal_errors(true);
    if(!$parsed_dom->schemaValidate($xsd_url)){
      $lines = explode("\n", $xml_per_line);
      $errors = libxml_get_errors();
      $error_messages = array();
      foreach($errors as $error){
        $type = 'error';
        if($error->level == LIBXML_ERR_WARNING){
          $type = 'warning';
        }
        // Get some context.
        $context = array();
        $num = 5;
        if($error->line > 2){
          $count = ($error->line > $num) ? $num : ($error->line - 1);
          $context = array_slice($lines, $error->line - $count - 1, $count);
          foreach($context as $key => $value){
            $context[$key] = htmlspecialchars($value);
          }
        }
        $context[] = '<strong>' . htmlspecialchars($lines[$error->line - 1]) . '</strong>';
        $error_messages[] = t("XML validation error: @code - @message !context", array(
          '@code' => $error->code,
          '@message' => $error->message,
          '!context' => '<br/>' . implode('<br/>', $context)
        ));
      }
      libxml_clear_errors();
      libxml_use_internal_errors(false);
      throw new Exception('<ul><li>' . implode('</li><li>', $error_messages) . '</li></ul>');
    }
    libxml_clear_errors();
    libxml_use_internal_errors(false);
  }
}

/**
 * This interface defines a publication xml modifier
 *
 * Such objects are given to the PublicationXMLBuilder,
 * and are called at every iteration. They can be used
 * to:
 * - Track references ;
 * - Order fields ;
 * - Modify/filter content
 *
 * Note: we use registered objects rather than hooks,
 * because the main purpose of this is to rename
 * citations and order cited objects accordingly
 * (eg. Table 1, Table 2, etc.) - this requires that
 * we keep context throughout the process which is
 * not always practical with hooks.
 */
interface PublicationXMLModifierInterface{

  /**
   * function start_building
   *
   * This is called before the XML building starts.
   *
   * $schema: The array schema used for building the XML
   *
   * $object: the object from which the XML
   * will be build
   *
   * $dom: The empty DomDocument object
   */
  public function start_building($schema, $object, $dom);

  /**
   * function insert_value
   *
   * This is called when an actual value is about to be
   * inserted into the DOM, and should return the
   * modified value to insert. Classes implementing
   * this interface should at least return $value here.
   *
   * $tag: The tag of the value being inserted 
   * $schema: The schema for the tag
   * $value: The value itself
   */
  public function insert_value($schema, $tag, $value);

  /**
   * function insert_value_array
   * 
   * This is called when a number of values comming from the
   * same field are going to be inserted sequencially with the
   * same tag. This should return the array of values, and
   * can be used to filter out some values or to order the values.
   * Classes implementing this interface should at least return $values
   * here.
   *
   * Note that each individual value will still go throug 'insert_value'
   * 
   * $schema: The schema for the tag
   * $tag: The tag of the value being inserted
   * $values: The array of values
   */
  public function insert_value_array($schema, $tag, $values);
}

/**
 * Process a country field to return either the iso2 code or
 * the country name
 *
 */
function _publication_xml_process_country($value, $type = 'name'){
  if($type == 'iso2'){
    return $value['iso2'];
  }else{
    $country = country_load($value['iso2']);
    return $country->name;
  }
}

/**
 * Process a map field to return just one property (by default the latitude)
 */
function _publication_xml_process_map($value, $type = 'latitude'){
  if (isset($value[LANGUAGE_NONE][0])) {
    $map = (array)($value[LANGUAGE_NONE][0]);
  } else {
    $map = (array)$value;
  }
  return $map[$type];
}

/**
 * Loader function which simply returns the value.
 *
 * This has the effect of replacing the current object
 * with the current value
 */
function _publication_xml_load_replace($value){
  return $value;
}

/**
 * Loader function for the author/contributor fields
 */
function _publication_xml_load_user($value){
  if(is_array($value) && isset($value['uid'])){
    $value = $value['uid'];
  }
  $user = user_load($value);
  if(!$user){throw new Exception(t("No such user id %uid - it looks like a user referenced from your publication has been deleted from the site", array(
      '%uid' => $value
    )));}
  $wrapper = entity_metadata_wrapper('user', $user);
  $given_names = explode(' ', $wrapper->field_user_given_names->value());
  $user->processed_full_name = implode(' ', array(
    $wrapper->field_user_title->value(),
    $wrapper->field_user_given_names->value(),
    $wrapper->field_user_family_name->value()
  ));
  $user->processed_first_name = array_shift($given_names);
  if(count($given_names)){
    $user->processed_middle_names = implode(' ', $given_names);
  }
  return $user;
}

/**
 * Merge function for merging multiple users into one
 */
function _publication_xml_merge_user($field){
  $values = $field[LANGUAGE_NONE];
  $output = array();
  foreach($values as $value){
    $user = _publication_xml_load_user($value);
    $output[] = $user->processed_full_name;
  }
  $obj = new StdClass();
  $obj->merged_full_name = implode(', ', $output);
  return $obj;
}

/**
 * Loader function for files
 */
function _publication_xml_load_file($value){
  $value['url'] = url($value['uri'], array(
    'absolute' => TRUE
  ));
  return ((object)$value);
}

/**
 * Loads the given term
 */
function _publication_xml_load_term($value, $final_name = NULL){
  $term = taxonomy_term_load($value['tid']);
  if ($final_name) {
    $term->name = $final_name;
  }
  return $term;
}

/**
 * Loads the parent of the given term
 */
function _publication_xml_load_parent_term($value, $rank = NULL, $default = NULL){
  if (is_array($value) && isset($value['tid'])) {
    $value = $value['tid'];
  }
  $terms = taxonomy_get_parents($value);
  while(!empty($terms)){
    $current = array_shift($terms);
    if($rank === NULL || $current->field_rank[LANGUAGE_NONE][0]['value'] == $rank){return $current;}
    $parents = taxonomy_get_parents($current->tid);
    $terms = array_merge($terms, $parents);
  }
  return $default;
}

/**
 * Loader function for materials
 */
function _publication_xml_load_specimen($value){
  $specimen = node_load($value['nid']);
  if(!empty($specimen->field_location[$specimen->language][0]['nid'])){
    $location = node_load($specimen->field_location[$specimen->language][0]['nid']);
    $location->location_title = $location->title;
    unset($location->title);
    $specimen = ((object)(array_merge((array)$specimen, (array)($location))));
  }
  $specimen->field_collector[LANGUAGE_NONE] = array();
  $specimen->field_identified_by[LANGUAGE_NONE] = array();
  return $specimen;
}