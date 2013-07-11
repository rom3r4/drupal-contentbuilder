<?php

/**
 * @file drush_updater.drush.inc
 * TODO: Enter file description here.
 */

DEFINE('AGRISAP_KEY', 'AGRISAP_FIELDS_STORAGE');
DEFINE('DC_KEY', 'DC_FIELDS_STORAGE');


DEFINE('AGRISAP_TITLE_FIELD', 'field_title');
DEFINE('DC_TITLE_FIELD', 'field_title');

DEFINE('AGRISAP_BODY_FIELD', 'field_title');
DEFINE('DC_BODY_FIELD', 'field_title');


/**
 * Implements hook_drush_command().
 */
function drush_updater_drush_command()
{
  $items = array();
  // callback is the function that will be called when the command is executed
  $items['load-data'] = array(
    'callback' => 'drush_drush_updater_helper',
    'description' => 'Loads data into VOA3R database',
    'aliases' => array('vupd'),
    'examples' => array(
      'drush upd --data=UUENCODED-JSON-DATA-XXXXXXXX' => 'loads data into VOA3R',
    ),
    'options' => array(
      'data' => 'the data in UUECODED JSON format.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  $items['create-data-structure'] = array(
    'callback' => 'drush_drush_updater_creator_helper',
    'description' => 'Creates the initial VOA3R content types staructures(test-fail strategy)',
    'aliases' => array('vcre'),
    'examples' => array(
      'drush creat --data=UUENCODED-JSON-DATA-XXXXXXXX' => 'create data content type into VOA3R database',
    ),
    'options' => array(
      'data' => 'the data in UUECODED JSON format.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  return $items;
}

/**
 * Helper function to store current fields in database (as a serialized string)
 * @param array/hash
 */
function store_serialized_helper($array)
{
  variable_set(AGRISAP_KEY, $array);
}

/**
 * Helper function to store current fields in database (as a serialized string)
 * @param array/hash
 */
function retrieve_unserialized_helper()
{

  return variable_get(AGRISAP_KEY, array());
}

/**
 * Gets an allowed drupal field name
 *
 * @param $field
 * @return string
 */
function get_key_helper($field)
{

  $field = strtolower($field);
  // drush_print($field);
  // return;
  if (strlen($field) > 0) {
    return $field_field = ($field[0] == '_') ? "field" . $field : "field_" . $field;
  } else {
    return "";
  }
}

/**
 * this Helper function created one content-type fiel per each time it is called
 *
 * @param $field
 */
function field_create_helper($field)
{

  $field = strtolower($field);
  $field_field = get_key_helper($field);

  // Check if our field is not already created.
  if (!field_info_field($field_field)) {
    $new_field = array(
      'active' => 1,
      // 'cardinality' => -1,
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => $field_field,
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => 0,
      'module' => 'text',
      'settings' => array(
        'max_length' => 512,
      ),
      'translatable' => 0,
      'type' => 'text',
    );
    field_create_field($new_field);

// Create the instance on the bundle.
    $instance = array(
      'field_name' => $field_field,
      'label' => $field,
      'bundle' => 'resource_agrisap',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 1,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'required' => 0,
      'settings' => array(
        'text_processing' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => 60,
        ),
        'type' => 'text_textfield',
        'weight' => 41,
      ),

    );
    field_create_instance($instance);
  }
}

/**
 * Helper function to detect a serialized JSON is valid.
 * @param JSON
 */
function isJson($string)
{
  json_decode($string);
  return (json_last_error() == JSON_ERROR_NONE);
}

/**
 * Helper drush callback.
 * @param
 */
function drush_drush_updater_creator_helper()
{
  // you need this to autoload the functions to create nodes
  module_load_include('inc', 'node', 'node.pages');

  $data = drush_get_option('data') ?
    explode(',', drush_get_option('data')) : array();

  $i = 0;
  // do some business logic to load your data from where it is
  foreach ($data as $res) {

    // drush_print($res);
    $decoded = base64_decode($res);
    if ($decoded) {
      if ($i == 0) {

        $array = (array)json_decode($decoded);

        // reset key storage
        // variable_del(AGRISAP_KEY);

        // $arr = array();
        $arr = retrieve_unserialized_helper();
        foreach ($array as $key => $val) {

          if (array_key_exists($key, $arr)) {
            // drush_print("hit!");
          } else {
            field_create_helper($key);
            // drush_print("miss");
          }

          $arr[$key] = $key;
        }
        store_serialized_helper($arr);
        // drush_print(print_r($arr, TRUE));
        // drush_print("---> ".count($arr));
        // TODO: PHPUnit: assert count($arr) is bigger or equal on each iteration
      }
    }
    $i++;
  }

  drush_log(dt('done.'), 'success');
}

/**
 * Helper drush callback.
 * @param
 */
function drush_drush_updater_helper()
{
  // drush_drush_updater_creator_helper();
  // you need this to autoload the functions to create nodes
  module_load_include('inc', 'node', 'node.pages');
  global $user;

  $data = drush_get_option('data') ?
    explode(',', drush_get_option('data')) : array();

  $i = 0;
  // do some business logic to load your data from where it is
  foreach ($data as $res) {

    // drush_print($res);
    $decoded = base64_decode($res);
    if ($decoded) {
      if ($i == 0) {

        $array = (array)json_decode($decoded);

        // reset key storage
        // variable_del(AGRISAP_KEY);


        $node = new stdClass();
        $node->type = 'resource_agrisap';
        $node->title = "Title";
        node_object_prepare($node);

        $node->language = LANGUAGE_NONE;

        // admin
        $node->uid = 1;
        // $node->uid = $user->uid;

        $node->status = 1; //(1 or 0): published or not
        $node->promote = 0; //(1 or 0): promoted to front page
        $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write

        $node->body[$node->language][0]['value'] = 'text';
        $node->body[$node->language][0]['summary'] = 'summary';
        // $node->body['und'][0]['format'] = 'filtered_html';


        // drush_print(print_r($node, TRUE));

        $arr = retrieve_unserialized_helper();
        foreach ($array as $key => $val) {

          if (array_key_exists($key, $arr)) {
            // drush_print("hit!");
          } else {
            field_create_helper($key);
            // drush_print("miss");
          }


          $j = 0;
          $key_name = get_key_helper($key);
          foreach ((array)$val as $k => $v) {

            // Try to set your custom field
            $node->{$key_name}[$node->language][$j]['value'] = $v;

            // since there is only 1 title --> O(1)
            if (AGRISAP_TITLE_FIELD == $key_name) {
              $node->title = $v;
            }


            $j++;
          }
          $arr[$key] = $key;
        }
        store_serialized_helper($arr);
        // drush_print(print_r($arr, TRUE));
        // TODO: PHPUnit: assert count($arr) is bigger or equal on each iteration


        if ($node = node_submit($node)) { // Prepare node for saving
          node_save($node);
          // drush_print("saved!");
        }

      }
    }
    $i++;
  }

  drush_log(dt('done.'), 'success');
}
