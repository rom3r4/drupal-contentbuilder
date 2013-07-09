<?php

/**
 * @file drush_updater.drush.inc
 * TODO: Enter file description here.
 */


/**
* Implements hook_drush_command().
*/
function drush_updater_drush_command() {
  $items = array();
        // callback is the function that will be called when the command is executed
  $items['load-rep-data'] = array(
	    'callback' => 'drush_updater_helper',
            'description' => 'Loads data into VOA3R database',
            'aliases' => array('vap'),
            'examples' => array(
                'drush vap --data=UUENCODED-JSON-DATA-XXXXXXXX' => 'loads data into VOA3R',
            ),
            'options' => array(
                'data' => 'the data in UUECODED JSON format.',
            ),
            'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
	);
	
  return $items;
}

/**
* Helper function to detect a serialized JSON is valid.
* @param JSON 
*/
function isJson($string) {
 json_decode($string);
  return (json_last_error() == JSON_ERROR_NONE);
}


/**
* Helper drush callback.
* @param 
*/
function drush_updater_helper(){
  
  // you need this to autoload the functions to create nodes
  module_load_include('inc', 'node', 'node.pages');
  
  $data = drush_get_option('data') ? 
     explode(',', drush_get_option('data')) : array();
  
  // drush_print("[+] parsing data string...");
  
  drush_print(count($data));
  /*
  if (!$data) {
    $arr = array();  
  }
  else {
  
    $arr = array();
  }
  */
  
  
  // $size = count($arr);
  // drush_print(" |- number of items to parse: " . $size);
    
     
  // if ($size) {
  
    // drush_print("[+] inserting data...");
  // }
   
  
  // do some business logic to load your data from where it is
  foreach( $data as $res ){
    
    drush_print($res);
    $decoded = base64_decode($res);
    if ($decoded) {
       drush_print($decoded);
    }
     
    /*
    // define the custom node type
    $node = array('type' => 'representative');
   
   // set up the form array 
   $form_state = array();
 
   // set the title of the node   
   $form_state['values']['title'] = $res["title"];
 
   // set a custom field that is a text type
   $form_state['values']['field_first_name']['und']['0']['value'] = $res["first_name"];
 
   // set a long text field and enable full_html - NOTE you'll need to allow anonymous users to use this for Drush to work
   $form_state['values']['body']['und']['0']['format'] = 'full_html';
   $form_state['values']['body']['und']['0']['value'] = $res["bio"];
 
  // set some custom select fields
  $form_state['values']['field_house_committees']['und'][ 0 ] = 34;
  $form_state['values']['field_house_committees']['und'][ 0 ] = 37;
 
  // can't leave this out or the form wont save
  $form_state['values']['op'] = t('Save');
 
  // actually try and "submit" the form
  drupal_form_submit('representative_node_form', $form_state, (object)$node);
  */

  
  // printing from Drush is easy 
  // drush_print( $res["title"] );
  }  
  
  drush_log(dt('done.'), 'success');
}


