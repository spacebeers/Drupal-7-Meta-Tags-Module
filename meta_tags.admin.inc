<?php

/**
* @file
* Administration page callbacks for the meta tags module.
*/

/**
* Form builder. Configure annotations.
*
* @ingroup forms
* @see system_settings_form().
*/


function meta_tags_admin_settings() {
  // Get an array of node types with internal names as keys and
  // "friendly names" as values. E.g.,
  // array('page' => Basic Page, 'article' => 'Articles')
  $types = node_type_get_types();
  foreach($types as $node_type) {
    $options[$node_type->type] = $node_type->name;
  }

  $form['meta_tags_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Users may add meta tags these content types'),
    '#options' => $options,
    '#default_value' => variable_get('meta_tags_node_types', array('page')),
    '#description' => t('Meta tag fields will be available on these content types.'),
  );

  $form['meta_tags_default_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default page title'),
    '#description' => t('Enter the default page title here. If no content specific page title is set this will be the fall back title.'),
    '#default_value' => variable_get('meta_tags_default_title', 'Enter the default title here...'),
  );
  
  $form['meta_tags_default_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Default page description'),
    '#description' => t('Enter the default page description here. If no content specific page title is set this will be the fall back title.'),
    '#default_value' => variable_get('meta_tags_default_desc', 'Enter the default description here...'),
    '#cols' => 40,
	'#rows' => 3,
	'#resizeable' => false
  );
  
  $form['meta_tags_default_keywords'] = array(
    '#type' => 'textarea',
    '#title' => t('Default page keywords'),
    '#description' => t('Enter the default page keywords here. Any additional keywords will be added to this list.'),
    '#default_value' => variable_get('meta_tags_default_keywords', 'Enter the default keywords here...'),
    '#cols' => 40,
	'#rows' => 3,
	'#resizeable' => false
  );
  
  $form['#submit'][] = 'meta_tags_admin_settings_submit';
  return system_settings_form($form, TRUE);

}

/**
 * Validate annotation settings submission.
 */
/*function meta_tags_admin_settings_validate($form, &$form_state) {
  $limit = $form_state['values']['meta_tags_limit_per_node'];
  if (!is_numeric($limit)) {
    form_set_error('meta_tags_limit_per_node', t('Please enter number.'));
  }
}*/

/**
* Process annotation settings submission.
*/
function meta_tags_admin_settings_submit($form, $form_state) {
  // Loop through each of the content type checkboxes shown on the form.
  foreach ($form_state['values']['meta_tags_node_types'] as $key => $value) {
    // If the check box for a content type is unchecked, look to see whether
    // this content type has the annotation field attached to it using the
    // field_info_instance function. If it does then we need to remove the
    // annotation field as the administrator has unchecked the box.
    if (!$value) {
      $instance = field_info_instance('node', 'meta_tags_content_title', $key);
      if (!empty($instance)) {
        field_delete_instance($instance, FALSE);
        watchdog("Meta tags module", 'Deleted meta tags fields from content type:
          %key', array('%key' => $key));
      }
	  
	  $instance = field_info_instance('node', 'meta_tags_content_desc', $key);
      if (!empty($instance)) {
        field_delete_instance($instance, FALSE);
        watchdog("Meta tags module", 'Deleted meta tags fields from content type:
          %key', array('%key' => $key));
      }
	  
	  $instance = field_info_instance('node', 'meta_tags_default_desc', $key);
      if (!empty($instance)) {
        field_delete_instance($instance, FALSE);
        watchdog("Meta tags module", 'Deleted meta tags fields from content type:
          %key', array('%key' => $key));
      }
    } else {
      // If the check box for a content type  is checked, look to see whether
      // the field is associated with that content type. If not then add the
      // annotation field to the content type.
      $instance = field_info_instance('node', 'meta_title', $key);
      if (empty($instance)) {
        $instance = array(
          'field_name' => 'meta_title',
          'entity_type' => 'node',
          'bundle' => $key,
          'label' => t('Meta tags Title'),
          'widget_type' => 'text',
          'display' => array(
            'default' => array(
			  'label' => 'hidden',
              'type' => 'hidden',
            ),
		 ),	
      );
      $instance = field_create_instance($instance);
      watchdog('Meta tags module', 'Added meta tags title field to content type: %key',
        array('%key' => $key));
      }
	  
	  $instance = field_info_instance('node', 'meta_desc', $key);
      if (empty($instance)) {
        $instance = array(
          'field_name' => 'meta_desc',
          'entity_type' => 'node',
          'bundle' => $key,
          'label' => t('Meta tags Description'),
			'widget' => array(
				'type' => 'text_textarea',
				'settings' => array(
				  'rows' => 4,
				),

			  ),
			  'settings' => array(
				'text_processing' => 0, // @TODO Find out the name
			  ),
			  'default_value' => array(),
			  'display' => array(
				'default' => array(
				  'label' => 'hidden',
				  'type' => 'hidden',
				  'settings' => array(),

				),
			  ), 	       
      );
      $instance = field_create_instance($instance);
      watchdog('Meta tags module', 'Added meta tags description field to content type: %key',
        array('%key' => $key));
      }
	  
	  $instance = field_info_instance('node', 'meta_keywords', $key);
      if (empty($instance)) {
        $instance = array(
          'field_name' => 'meta_keywords',
          'entity_type' => 'node',
          'bundle' => $key,
          'label' => t('Meta tags Keywords'),
			'widget' => array(
				'type' => 'text_textarea',
				'settings' => array(
				  'rows' => 4,
				),

			  ),
			  'settings' => array(
				'text_processing' => 0, // @TODO Find out the name
			  ),
			  'default_value' => array(),
			  'display' => array(
				'default' => array(
				  'label' => 'hidden',
				  'type' => 'hidden',
				  'settings' => array(),

				),
			  ), 	
      );
      $instance = field_create_instance($instance);
      watchdog('Meta tags module', 'Added meta tags keywords field to content type: %key',
        array('%key' => $key));
      }
    }
  } // End foreach loop.
}