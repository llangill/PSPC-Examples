<?php
/**
 * @file
 * Runs the deen_sosa_exporter through a basic PHP script.
 */

/**
 * Implements hook_drush_help().
 */

 function local_generate_report_drush_help($command) {
  switch ($command) {
    case 'drush:generate-report':
      return dt('Generate nightly report (csv).');
  }
}

/**
 * Implements hook_drush_command().
 */
function local_generate_report_drush_command() {
  $items = array();
  $items['generate-report'] = array(
    'description' => dt('Generate nightly report.'),
    'arguments'   => array(
      'arg1'    => dt('File type?'),
      'arg2'    => dt('File name?'),
      'arg3'    => dt('Memory limit?'),
    ),
    'examples' => array(
      'Standard example' => 'drush generate-report',
      'Argument example' => 'drush generate-report csv aocama_accords-sosaa_agreements',
    ),
    'aliases' => array('gen-rep'),
  );
  return $items;
}

/**
 * Callback function for drush generate-report. 
 *
 * @param $arg1
 * @param $arg1
 *   An optional argument
 */
function drush_local_generate_report_generate_report($arg1 = 'csv', $arg2 = 'aocama_accords-sosaa_agreements', $arg3 = NULL) {
  if($arg3 === NULL) {
    $arg3 = ini_get('memory_limit');
  }

  //Set up timestamp for logging.
  date_default_timezone_set('America/Toronto');
  $start_time = time();
  $start_date_time = date('d-m-Y H:i:s', $start_time);

  set_time_limit(0);
  ini_set('memory_limit', $arg3);

  print getGenerateReportOutputPrefix();
  print "With arguments: Output[$arg1], Filename[$arg2], Memory[$arg3]\n";

  $method = $arg1;
  $private_path = variable_get('file_private_path');
  $filename = $private_path . '/nightly-report/' . $arg2 . '.' . $arg1;

  print getGenerateReportOutputPrefix();
  print "Loading nodes.\n";
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'sosa')
    ->execute();
  $nodes = entity_load('node', array_keys($entities['node']));
  $total = count($nodes);

  $context = array(
    'progress' => array(
      'total' => $total,
    ),
    'view_info' => array(
      'name' => 'wetkit_og_all_user_group_content',
      'display' => 'wetkit_og_my_workspace',
      'arguments' => array('7183'),
    ),
    'entity_type' => 'node',
    'settings' => array(),
    'method' => $method,
    'fileName' => $filename,
    'report_variant_name' => 'nightly_report',
  );
  $current = 1;

  print getGenerateReportOutputPrefix();
  print "Writing export. (watch for progress update)\n";
  $increment = 5; // Percentage increment to progress message.
  $progressIncrement = floor($total / (100 / $increment));
  $nextIncrement = $progressIncrement;

  foreach ($nodes as $node){
    $context['node'] = $node;
    $context['progress']['current'] = $current;
    $context['view_info']['exposed_input']['title_field_value'] =  $node->field_procurement_number['und'][0]['value'];
    $context['view_info']['exposed_input']['state'][0] =  $node->workbench_moderation->current['from_state'];
    deen_sosa_exporter_action_export_sosa_nodes($node, $context);
    if ($current > $nextIncrement) {
      $completePercentage = ($nextIncrement/$progressIncrement) * $increment;
      print getGenerateReportOutputPrefix();
      print "$completePercentage% completed.\n";
      $nextIncrement += $progressIncrement;
    }
    $current++;
  }
  drupal_chmod($filename, 0666);
  $took = round((time()-$start_time)/60);

  watchdog('local_generate_report', 'The report started at %start Eastern took %took minutes to complete.', array(
    '%start' => $start_date_time,
    '%took' => $took,
  ), WATCHDOG_INFO);

  print getGenerateReportOutputPrefix();
  print "Done!\n";
}

function getGenerateReportOutputPrefix(){
  return date("[Y-m-d H:i:s] - ");
}